#include "TextResource.h"

namespace Val {
	TextResource::TextResource(FontAsset * font, std::string contents) : font(font), data(contents) {
	}

	TextResource::~TextResource() {
	}

	FontAsset * TextResource::getFont() const {
		return font;
	}

	std::string TextResource::getTextString() const {
		return data;
	}
}