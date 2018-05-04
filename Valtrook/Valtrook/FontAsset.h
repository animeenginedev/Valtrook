#pragma once

#include "ResourceLocation.h"
#include <unordered_map>
#include <sdl_ttf.h>

namespace Val {
	class FontAsset {
	public:
		FontAsset(unsigned int fontSize, TTF_Font* font);
		~FontAsset();

		static FontAsset* getFont(ResourceLocation resource, unsigned int fontSize);
		
		unsigned int getFontSize();
		TTF_Font* getTTF();
	protected:
		static std::unordered_map<std::string, FontAsset*> loadedFonts;

		unsigned int fontSize;
		TTF_Font* font;
	};

}