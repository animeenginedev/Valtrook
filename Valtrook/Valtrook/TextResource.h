#pragma once

#include "FontAsset.h"
#include <sdl_ttf.h>
#include <string>


namespace Val {
	class TextResource {
	public:
		TextResource(FontAsset* font, std::string contents);
		~TextResource();

		FontAsset* getFont() const;
		std::string getTextString() const;

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