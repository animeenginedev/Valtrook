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

	FontAsset* CHAIgetFont(ResourceLocation resource, unsigned int fontSize) {
		return FontAsset::getFont(resource, fontSize);
	}

	void FontAsset::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::user_type<FontAsset>(), "FontAsset");

		script->add(chaiscript::fun(&FontAsset::getFontSize), "getFontSize");

		script->add(chaiscript::fun(&CHAIgetFont), "getFont");
	}

	FontAsset * FontAsset::getFont(ResourceLocation resource, unsigned int fontSize) {
		std::string fontName = resource.getLocation() + ":" + std::to_string(fontSize);

		auto it = loadedFonts.find(fontName);

		if (it == loadedFonts.end()) {
		
			loadedFonts.insert(std::make_pair(fontName, new FontAsset(fontSize, TTF_OpenFont((resource.getLocation()).c_str(), fontSize))));

			it = loadedFonts.find(fontName);
			if (it->second->font == nullptr) {
				Logger::Instance->logMessage(LogLevel::L_ERROR, "Failed to load font \"" + fontName + "\"!");
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