#include "FontAsset.h"

#include "RuntimeConstants.h"
#include "Logger.h"

namespace Val {
	std::unordered_map<std::string, FontAsset*> FontAsset::loadedFonts;

	FontAsset::FontAsset(unsigned int fontSize, TTF_Font * font) {
		this->font = font;
		this->fontSize = fontSize;
	}

	FontAsset::~FontAsset() {
		TTF_CloseFont(font);
	}

	FontAsset * FontAsset::getFont(ResourceLocation resource, unsigned int fontSize) {
		std::string fontName = resource.getLocation() + ":" + std::to_string(fontSize);

		auto it = loadedFonts.find(fontName);

		if (it == loadedFonts.end()) {
		
			loadedFonts.insert(std::make_pair(fontName, new FontAsset(fontSize, TTF_OpenFont((resource.getLocation()).c_str(), fontSize))));

			it = loadedFonts.find(fontName);
			if (it->second->font == nullptr) {
				Logger::Instance->logMessage(LogLevel::ERROR, "Failed to load font \"" + fontName + "\"!");
				return nullptr;
			}
		}

		return it->second;
	}

	unsigned int FontAsset::getFontSize() {
		return fontSize;
	}

	TTF_Font * FontAsset::getTTF() {
		return font;
	}
}