#include "AudioManager.h"

#include "TaskMaster.h"
#include <thread>
#include <SDL2\SDL.h>
#include <SDL2\SDL_audio.h>

namespace Val {
	std::unordered_map<int, AudioDelegate*> AudioDelegate::channelDelegates;
	std::unordered_map<int, std::function<void(AudioDelegate*)>> AudioDelegate::callbacks;

	AudioAsset::AudioAsset() : resource("", "", "") {
	}

	void AudioAsset::AsyncLoadWav(std::string loc) {
		loadedAsset = Mix_LoadWAV(loc.c_str());
		if (loadedAsset == nullptr) {
			fprintf(stderr, "Failed to load audio asset @ %s :: Err %s\n", loc.c_str(), Mix_GetError());
		}
	}

	void AudioAsset::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::user_type<AudioAsset>(), "AudioAsset");

		script->add(chaiscript::var(&AudioAsset::resource), "resource");

		script->add(chaiscript::fun(&AudioAsset::createPlayDelegate), "createPlayDelegate");
	}

	//Asset
	void AudioAsset::load(ResourceLocation res, bool allowAsyncLoad) {
		auto ext = res.getExtension();
		if (ext == ".wav" || ext == ".ogg" || ext == ".mp3" || ext == ".mod" || ext == ".midi") {
			if (allowAsyncLoad) {
				TaskMaster::addTask(MakeTask(std::bind(&AudioAsset::AsyncLoadWav, this, res.getLocation())));				
			} else {
				this->loadedAsset = Mix_LoadWAV(res.getLocation().c_str());
				if (loadedAsset == nullptr) {
					fprintf(stderr, "Failed to load audio asset @ %s :: Err %s\n", res.getLocation().c_str(), Mix_GetError());
				}
			}
		} else {
			fprintf(stderr, "Failed to load audio asset unsupported type@ %s\n", resource.getLocation().c_str());
		}
	}
	std::shared_ptr<AudioDelegate> AudioAsset::createPlayDelegate() {
		return std::make_shared<AudioDelegate>(this);		
	}
	Owner<AudioDelegate*> AudioAsset::createPlayDelegateO() {
		return new AudioDelegate(this);
	}
	void AudioAsset::cleanup() {
		if (loadedAsset != nullptr) {
			Mix_FreeChunk(loadedAsset);
		}
	}
	//Delegate
	AudioDelegate::AudioDelegate(AudioAsset * asset) : asset(asset), audioChannel(-1) {
	};
	AudioDelegate::~AudioDelegate() {
		if (validDelegate()) {
			channelDelegates.erase(audioChannel);
			callbacks.erase(audioChannel);
		}
	}
	bool AudioDelegate::isLoaded() {
		return asset->loadedAsset != nullptr;
	}
	void AudioDelegate::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::user_type<AudioDelegate>(), "AudioDelegate");

		script->add(chaiscript::constructor<AudioDelegate(AudioAsset*)>(), "AudioDelegate");

		script->add(chaiscript::fun(&AudioDelegate::isLoaded), "isLoaded");
		script->add(chaiscript::fun(&AudioDelegate::playFadeIn), "playFadeIn");
		script->add(chaiscript::fun(&AudioDelegate::playFadeInTimed), "playFadeInTimed");
		script->add(chaiscript::fun(&AudioDelegate::play), "play");
		script->add(chaiscript::fun(&AudioDelegate::playTimed), "playTimed");
		script->add(chaiscript::fun(&AudioDelegate::setVolume), "setVolume");
		script->add(chaiscript::fun(&AudioDelegate::pause), "pause");
		script->add(chaiscript::fun(&AudioDelegate::resume), "resume");
		script->add(chaiscript::fun(&AudioDelegate::fadeOut), "fadeOut");
		script->add(chaiscript::fun(&AudioDelegate::expire), "expire");
		script->add(chaiscript::fun(&AudioDelegate::halt), "halt");
		script->add(chaiscript::fun(&AudioDelegate::getAudioAsset), "getAudioAsset");
		script->add(chaiscript::fun(&AudioDelegate::getAudioChannel), "getAudioChannel");
		script->add(chaiscript::fun(&AudioDelegate::isValidDelegate), "isValidDelegate");
		script->add(chaiscript::fun([](AudioDelegate& lhs, const AudioDelegate rhs) {return (lhs = rhs); }), "=");
	}
	bool AudioDelegate::playFadeIn(int milliseconds, std::function<void(AudioDelegate*)> callback, int repeats) {
		if (!isLoaded())
			return false;

		if ((audioChannel = Mix_FadeInChannel(-1, asset->loadedAsset, repeats, milliseconds)) == -1) {
			return false;
		}
		channelDelegates.erase(audioChannel);
		callbacks.erase(audioChannel);

		channelDelegates.insert(std::make_pair(audioChannel, this));
		if (callback.operator bool()) {
			AudioDelegate::callbacks.insert(std::make_pair(audioChannel, callback));
			Mix_ChannelFinished([](int c) { audioFinished(c); }
			);
		}
		return true;
	}
	bool AudioDelegate::playFadeInTimed(int fadeInMilliseconds, int timedMilliseconds, std::function<void(AudioDelegate*)> callback, int repeats) {
		if (!isLoaded())
			return false;

		if ((audioChannel = Mix_FadeInChannelTimed(-1, asset->loadedAsset, repeats, fadeInMilliseconds, timedMilliseconds)) == -1) {
			return false;
		}
		channelDelegates.erase(audioChannel);
		callbacks.erase(audioChannel);

		channelDelegates.insert(std::make_pair(audioChannel, this));
		if (callback.operator bool()) {
			AudioDelegate::callbacks.insert(std::make_pair(audioChannel, callback));
			Mix_ChannelFinished([](int c) { audioFinished(c); }
			);
		}
		return true;
	}
	bool AudioDelegate::play(std::function<void(AudioDelegate*)> callback, int repeats) {
		if (!isLoaded())
			return false;

		if ((audioChannel = Mix_PlayChannel(-1, asset->loadedAsset, repeats)) == -1) {
			return false;
		}
		channelDelegates.erase(audioChannel);
		callbacks.erase(audioChannel);

		channelDelegates.insert(std::make_pair(audioChannel, this));
		if (callback.operator bool()) {
			AudioDelegate::callbacks.insert(std::make_pair(audioChannel, callback));
			Mix_ChannelFinished([](int c) { audioFinished(c); }
			);
		}
		return true;
	}
	bool AudioDelegate::playTimed(int milliseconds, std::function<void(AudioDelegate*)> callback, int repeatsMax) {
		if (!isLoaded())
			return false;

		if ((audioChannel = Mix_PlayChannelTimed(-1, asset->loadedAsset, repeatsMax, milliseconds)) == -1) {
			return false;
		}
		channelDelegates.erase(audioChannel);
		callbacks.erase(audioChannel);

		channelDelegates.insert(std::make_pair(audioChannel, this));
		if (callback.operator bool()) {
			AudioDelegate::callbacks.insert(std::make_pair(audioChannel, callback));
			Mix_ChannelFinished([](int c) { audioFinished(c); }
			);
		}
		return true;
	}
	void AudioDelegate::setVolume(float volume) {
		if (!isLoaded())
			return;
		if (validDelegate()) {
			Mix_Volume(audioChannel, static_cast<int>(volume * 128));
		}
	}
	void AudioDelegate::pause() {
		if (!isLoaded())
			return;
		if (validDelegate()) {
			Mix_Pause(audioChannel);
		}
	}
	void AudioDelegate::resume() {
		if (!isLoaded())
			return;
		if (validDelegate()) {
			Mix_Resume(audioChannel);
		}
	}
	void AudioDelegate::fadeOut(int milliseconds) {
		if (!isLoaded())
			return;
		if (validDelegate())
			Mix_FadeOutChannel(audioChannel, milliseconds);
	}
	void AudioDelegate::expire(int milliseconds) {
		if (!isLoaded())
			return;
		if (validDelegate())
			Mix_ExpireChannel(audioChannel, milliseconds);
	}

	void AudioDelegate::halt() {
		if (!isLoaded())
			return;
		if (validDelegate()) 
			Mix_HaltChannel(audioChannel);
	}

	AudioAsset * AudioDelegate::getAudioAsset() const {
		return asset;
	}

	int AudioDelegate::getAudioChannel() const {
		return audioChannel;
	}

	bool AudioDelegate::isValidDelegate() const {
		return validDelegate();
	}

	bool AudioDelegate::validDelegate() const {
		if (audioChannel != -1 && channelDelegates.find(audioChannel) != channelDelegates.end()) {
			if (channelDelegates.at(audioChannel) == this) {
				return true;
			}
		}
		return false;
	}

	void AudioDelegate::audioFinished(int channel) {
		if (callbacks.find(channel) == callbacks.end()) {
			return;
		}
		callbacks.find(channel)->second(channelDelegates.at(channel));
	}

	AudioManager::AudioManager() {
	}

	AudioManager::~AudioManager() {
	}

	void AudioManager::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::user_type<AudioManager>(), "AudioManager");

		script->add(chaiscript::fun(&AudioManager::preloadMusic), "preloadMusic");
		script->add(chaiscript::fun(&AudioManager::unloadMusic), "unloadMusic");
		script->add(chaiscript::fun(&AudioManager::playMusic), "playMusic");
		script->add(chaiscript::fun(&AudioManager::pauseMusic), "pauseMusic");
		script->add(chaiscript::fun(&AudioManager::resumeMusic), "resumeMusic");
		script->add(chaiscript::fun(&AudioManager::haltMusic), "haltMusic");
		script->add(chaiscript::fun(&AudioManager::setMusicVolume), "setMusicVolume");
		script->add(chaiscript::fun(&AudioManager::getAudioPlayer), "getAudioPlayer");

		script->add(chaiscript::fun(&AudioManager::allocateChannels), "allocateChannels");
		script->add(chaiscript::fun(&AudioManager::reserveChannels), "reserveChannels");
		script->add(chaiscript::fun(&AudioManager::uniqueChannels), "uniqueChannels");
	}

	void AudioManager::initialise() {
		if (SDL_Init(SDL_INIT_AUDIO) < 0) {
			fprintf(stderr, "Audio initialization failed @SDL2: %s\n", SDL_GetError());
		}
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
			fprintf(stderr, "Audio initialization failed @SDL_Mixer\n");
		}
		auto successful = Mix_Init(MIX_INIT_FLAC | MIX_INIT_MID | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
		if (successful == 0) {
			fprintf(stderr, "Failed to load SDL_Mixer decoders\n");
		}
		Mix_Volume(-1, MIX_MAX_VOLUME);
		channels = Mix_AllocateChannels(32);
	}

	void AudioManager::preloadMusic(ResourceLocation resource) {
		auto ext = resource.getExtension();
		if (ext == ".wav" || ext == ".ogg" || ext == ".mp3" || ext == ".mod" || ext == ".midi") {
			Mix_Music* asset = Mix_LoadMUS(resource.getLocation().c_str());
			if (asset == nullptr) {
				fprintf(stderr, "Failed to load music asset @ %s :: Err %s\n", resource.getLocation().c_str(), Mix_GetError());
			}

			MusicAssets.insert(std::make_pair(resource, asset));
		} else {
			fprintf(stderr, "Failed to load music asset unsupported type@ %s\n", resource.getLocation().c_str());
		}
	}

	void AudioManager::unloadMusic(ResourceLocation resource) {
		if (MusicAssets.find(resource) != MusicAssets.end()) {
			Mix_FreeMusic(MusicAssets.at(resource));
			MusicAssets.erase(resource);
		}
	}

	bool AudioManager::playMusic(ResourceLocation resource, int loops) {
		if (MusicAssets.find(resource) != MusicAssets.end()) {
			if (Mix_PlayMusic(MusicAssets.at(resource), loops) == -1) {
				return false;
			}
			return true;
		}
		return false;
	}

	void AudioManager::pauseMusic() {
		Mix_PauseMusic();
	}

	void AudioManager::resumeMusic() {
		Mix_ResumeMusic();
	}

	void AudioManager::haltMusic() {
		Mix_HaltMusic();
	}

	void AudioManager::setMusicVolume(float vol) {
		Mix_VolumeMusic(static_cast<int>(static_cast<float>(128) * vol));
	}

	std::shared_ptr<AudioDelegate> AudioManager::getAudioPlayer(ResourceLocation loc, bool allowAsyncLoad) {
		if (Assets.find(loc) == Assets.end()) {
			auto asset = new AudioAsset();
			asset->load(loc, allowAsyncLoad);
			if (!allowAsyncLoad) {
				if (asset == nullptr)
					return nullptr;
			}
			Assets.insert(std::make_pair(loc, asset));
		}
		return Assets.at(loc)->createPlayDelegate();
	}
	Owner<AudioDelegate*> AudioManager::getAudioPlayerO(ResourceLocation loc, bool allowAsyncLoad) {
		if (Assets.find(loc) == Assets.end()) {
			auto asset = new AudioAsset();
			asset->load(loc, allowAsyncLoad);
			if (!allowAsyncLoad) {
				if (asset == nullptr)
					return nullptr;
			}
			Assets.insert(std::make_pair(loc, std::move(asset)));
		}
		return Assets.at(loc)->createPlayDelegateO();
	}

	int AudioManager::allocateChannels(int number) {
		channels = Mix_AllocateChannels(number);
		return channels;
	}

	void AudioManager::reserveChannels(int number) {
		Mix_ReserveChannels(number);
	}

	int AudioManager::uniqueChannels() const {
		return channels;
	}

}