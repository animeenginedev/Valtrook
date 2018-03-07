#include "TextureSheet.h"

namespace Val {
	TextureSheet::TextureSheet() {
	}

	TextureSheet::~TextureSheet() {
	}

	unsigned int TextureSheet::getIndexForFrameName(std::string name) {
		auto it = frameNameMap.find(name);
		return it == frameNameMap.end() ? 0 : it->second;
	}

	std::string TextureSheet::getFrameForIndexName(unsigned int index) {
		auto it = nameFrameMap.find(index);
		return it == nameFrameMap.end() ? "" : it->second;
	}

	TextureResource TextureSheet::getResource(unsigned int index) {
		return frameResources[index];
	}

	TextureResource TextureSheet::getNext(unsigned int index, bool wrap) {
		if (wrap)
			return frameResources[(index + 1) % frameResources.size()];
		return frameResources[index + 1 >= frameResources.size() ? frameResources.size() - 1 : index + 1];
	}

	TextureResource TextureSheet::getPrevious(unsigned int index, bool wrap) {
		if (wrap)
			frameResources[(index + frameResources.size() - 1) % frameResources.size()];
		return frameResources[index - 1 < 0 ? 0 : index - 1];
	}

	unsigned int TextureSheet::addFrame(std::string name, TextureResource frameData) {
		frameNameMap.insert(std::make_pair(name, frameResources.size()));
		nameFrameMap.insert(std::make_pair(frameResources.size(), name));
		frameResources.push_back(frameData);
		return frameResources.size() - 1;
	}

	unsigned int TextureSheet::getNumberOfFrames() {
		return frameResources.size();
	}
}