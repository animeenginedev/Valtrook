#pragma once

#include "ResourceLocation.h"

#include <unordered_map>
#include <SDL2\SDL_mixer.h>
#include <functional>
#include <memory>

namespace Val {
	class AudioDelegate;

	class AudioAsset {
	public:
		AudioAsset();

		ResourceLocation resource;
		Mix_Chunk* loadedAsset;

		void load(ResourceLocation res);
		std::shared_ptr<AudioDelegate> createPlayDelegate();

		void cleanup();
	};

	class AudioDelegate {
	public:
		AudioDelegate(AudioAsset* asset);
		~AudioDelegate();

		//Play
		bool playFadeIn(int milliseconds, std::function<void(AudioDelegate*)> callback = nullptr, int repeats = 0);
		bool playFadeInTimed(int fadeInMilliseconds, int timedMilliseconds, std::function<void(AudioDelegate*)> callback = nullptr, int repeats = 0);
		bool play(std::function<void(AudioDelegate*)> callback= nullptr, int repeats = 0);
		bool playTimed(int milliseconds, std::function<void(AudioDelegate*)> callback = nullptr, int repeatsMax = 0);

		void setVolume(float volume);

		//pause, keeps channel locked
		void pause();
		//resume
		void resume();
		
		//Fade sound out over milliseconds
		void fadeOut(int milliseconds);
		//Halt sound in x milliseconds
		void expire(int milliseconds);
		//Halt invalidates the sound and frees the channel (early finish); calls the callback function
		void halt();

		AudioAsset* getAudioAsset() const;
		int getAudioChannel() const;
		bool isValidDelegate() const;
	protected:
		AudioAsset* asset;
		int audioChannel;

		bool validDelegate() const;
		//We can't do any nice callback system, so we have this piece of autisim 
		static void audioFinished(int channel);
		static std::unordered_map<int, AudioDelegate*> channelDelegates;
		static std::unordered_map<int, std::function<void(AudioDelegate*)>> callbacks;
	};

	class AudioManager {
	public:
		AudioManager();
		~AudioManager();

		void initialise();

		void preloadMusic(ResourceLocation resource);
		void unloadMusic(ResourceLocation resource);

		//loops -1 repeat forever, 0 play once, >0 loop x times
		bool playMusic(ResourceLocation resource, int loops = -1);

		void pauseMusic();
		void resumeMusic();
		void haltMusic();

		void setMusicVolume(float vol);

		std::shared_ptr<AudioDelegate> getAudioPlayer(ResourceLocation loc);

		//Input: The amount you want; Output the amount you got
		int allocateChannels(int number);
		//Reserve channels for your own use, starts from 0 -> channelX. Cannot go over the maximum number of channels, if you reserver all channels no sounds will automatically play with the audiodelegate.
		void reserveChannels(int number);
		int uniqueChannels() const;
	protected:
		int channels;
		std::unordered_map<ResourceLocation, Mix_Music*> MusicAssets;
		std::unordered_map<ResourceLocation, AudioAsset> Assets;
	};

}