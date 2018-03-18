#include "TextResource.h"

#include "RuntimeConstants.h"

namespace Val {
	TextResource::TextResource(std::string contents, std::string fontName, unsigned int textSize, std::string fontExtension) : font(FontAsset::getFont(ResourceLocation(fontName, fontExtension, RuntimeConstants::Instance->FontPath), textSize)), data(contents) {
	}
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