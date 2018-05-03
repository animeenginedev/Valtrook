#include "TextResource.h"

#include "RuntimeConstants.h"
#include <sdl_ttf.h>

namespace Val {
	TextResource::TextResource(std::string contents, std::string fontName, unsigned int textSize, std::string fontExtension) : font(FontAsset::getFont(ResourceLocation(fontName, fontExtension, RuntimeConstants::Instance->FontPath), textSize)), data(contents) {
	}
	TextResource::TextResource(FontAsset * font, std::string contents) : font(font), data(contents) {
	}

	TextResource::~TextResource() {
	}

	void TextResource::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::user_type<TextResource>(), "TextResource");

		script->add(chaiscript::constructor<TextResource(FontAsset*, std::string)>(), "TextResource");

		script->add(chaiscript::fun(&TextResource::getFont), "getFont");
		script->add(chaiscript::fun(&TextResource::getTextString), "getTextString");

		script->add(chaiscript::fun([](const TextResource lhs, const TextResource rhs) {return (lhs == rhs); }), "==");
		script->add(chaiscript::fun([](const TextResource lhs, const TextResource rhs) {return (lhs != rhs); }), "!=");
		script->add(chaiscript::fun([](TextResource& lhs, const TextResource rhs) {return (lhs = rhs); }), "=");
	}

	FontAsset * TextResource::getFont() const {
		return font;
	}

	std::string TextResource::getTextString() const {
		return data;
	}
	std::array<int, 2> TextResource::getTextTextureSize() {
		int w = 0, h = 0;
	
		TTF_SizeText(font->getTTF(), data.c_str(), &w, &h);	

		return { w, h };
	}
	float TextResource::getScaledTextWidth(float textWorldHeight) {
		auto textureSize = getTextTextureSize();

		return (textWorldHeight / textureSize[1]) * textureSize[0];
	}
	float TextResource::getScaledTextHeight(float textWorldWidth) {
		auto textureSize = getTextTextureSize();

		return (textWorldWidth / textureSize[0]) * textureSize[1];
	}
	std::array<int, 2> TextResource::getTextTextureSize(const std::string & text) {
		int w = 0, h = 0;

		TTF_SizeText(font->getTTF(), text.c_str(), &w, &h);

		return{ w, h };
	}
	float TextResource::getScaledTextWidth(const std::string & text, float textWorldHeight) {
		auto textureSize = getTextTextureSize(text);

		return (textWorldHeight / textureSize[1]) * textureSize[0];
		return 0.0f;
	}
	float TextResource::getScaledTextHeight(const std::string & text, float textWorldWidth) {
		auto textureSize = getTextTextureSize(text);

		return (textWorldWidth / textureSize[0]) * textureSize[1];
	}
}