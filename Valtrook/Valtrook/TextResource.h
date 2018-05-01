#pragma once

#include "FontAsset.h"
#include <sdl_ttf.h>
#include <string>


namespace Val {
	class TextResource {
	public:
		TextResource(std::string contents, std::string fontName, unsigned int textSize = 96, std::string fontExtension = ".ttf");
		TextResource(FontAsset* font, std::string contents);
		~TextResource();

		FontAsset* getFont() const;
		std::string getTextString() const;

		std::array<int, 2> getTextTextureSize();
		float getScaledTextWidth(float textWorldHeight);
		float getScaledTextHeight(float textWorldWidth);

		std::array<int, 2> getTextTextureSize(const std::string& text);
		float getScaledTextWidth(const std::string& text, float textWorldHeight);
		float getScaledTextHeight(const std::string& text,float textWorldWidth);

		bool operator==(const TextResource& rhs) const {
			return font == rhs.font && data == rhs.data;
		}
		bool operator!=(const TextResource& rhs) const {
			return font != rhs.font || data != rhs.data;
		}
	protected:
		FontAsset* font;
		std::string data;
	};


}
//Resource Locations are used in unordered_maps for organizing textures.
namespace std {
	template<>
	struct hash<Val::TextResource> {
		size_t operator()(const Val::TextResource& rl) const {
			return hash<Val::FontAsset*>()(rl.getFont()) + hash<string>()(rl.getTextString());
		}
	};
}