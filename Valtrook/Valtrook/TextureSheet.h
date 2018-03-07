#pragma once

#include "Texture.h"

#include <unordered_map>
#include <vector>
#include <string>

namespace Val {
	class TextureSheet {
	public:
		TextureSheet();
		~TextureSheet();

		unsigned int getIndexForFrameName(std::string name);
		std::string getFrameForIndexName(unsigned int index);

		TextureResource getResource(unsigned int index);
		TextureResource getNext(unsigned int index, bool wrap);
		TextureResource getPrevious(unsigned int index, bool wrap);

		unsigned int addFrame(std::string name, TextureResource frameData);

		unsigned int getNumberOfFrames();
	protected:
		std::unordered_map<std::string, unsigned int> frameNameMap;
		std::unordered_map<unsigned int, std::string> nameFrameMap;
		std::vector<TextureResource> frameResources;
	};

}