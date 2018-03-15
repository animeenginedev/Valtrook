#include "TextRectangle.h"

#include "TextRenderer.h"
#include "Logger.h"
#include "Conversion.h"

namespace Val {
	SimpleTextRectangle::SimpleTextRectangle() : textResource(nullptr, ""), scaleTextToHeight(true) {
	}
	SimpleTextRectangle::~SimpleTextRectangle() {
	}
	void SimpleTextRectangle::initialise(TextResource text, const float & x, const float & y, const float & depth, const float & halfWidth, const float & halfHeight, const Colour & colour, const GLBlendMode & blendMode) {
		SimpleRectangle::initialise(TextRenderer::getTexture(text), x, y, depth, halfWidth, halfHeight, colour, blendMode);

		textResource = text;
	}
	void SimpleTextRectangle::setText(const std::string & contents) {
		textResource = TextResource(textResource.getFont(), contents);
		needsReconstructed = true;
	}
	std::string SimpleTextRectangle::getText() const {
		return textResource.getTextString();
	}
	void SimpleTextRectangle::changeFont(FontAsset * newFont) {
		textResource = TextResource(newFont, textResource.getTextString());
		needsReconstructed = true;
	}
	FontAsset * SimpleTextRectangle::getFont() const {
		return textResource.getFont();
	}
	void SimpleTextRectangle::setScaleTextToHeight(const bool & scaleTextToHeight) {
		this->scaleTextToHeight = scaleTextToHeight;
		needsReconstructed = true;
	}
	bool SimpleTextRectangle::doesScaleTextToHeight() const {
		return scaleTextToHeight;
	}
	void SimpleTextRectangle::initialise(const TextureResource & texture, const float & x, const float & y, const float & depth, const float & halfWidth, const float & halfHeight, const Colour & colour, const GLBlendMode & blendMode) {
	}
	void SimpleTextRectangle::recalculateVertexes() {
		needsReconstructed = false;

		texture = TextRenderer::getTexture(textResource);

		if (texture.getTexture() == nullptr) {
			Logger::Instance->logMessage(WARNING, "Invalid Texture on Simple Text Rectangle");
			return;
		}

		if (scaleTextToHeight) {
			auto texSize = texture.getTextureSizeInPixel();
			halfSize[0] = (halfSize[1] / texSize[1]) * texSize[0];
		}

		std::array<float, 2> centerUPixel = { WorldToUnalignedPixel<float>(center[0]), WorldToUnalignedPixel<float>(center[1]) };
		std::array<float, 2> halfSizeUPixel = { WorldToUnalignedPixel<float>(halfSize[0]), WorldToUnalignedPixel<float>(halfSize[1]) };

		Glyph = RectangleGlyph(texture.getGLTexture()->getTextureID(), std::array<Vertex, 4>({

			Vertex(centerUPixel[0] - halfSizeUPixel[0], centerUPixel[1] + halfSizeUPixel[1], depth, uvBounds.u, uvBounds.v, renderColour),
			Vertex(centerUPixel[0] + halfSizeUPixel[0], centerUPixel[1] + halfSizeUPixel[1], depth, uvBounds.u + uvBounds.uWidth, uvBounds.v, renderColour),
            Vertex(centerUPixel[0] - halfSizeUPixel[0], centerUPixel[1] - halfSizeUPixel[1], depth, uvBounds.u, uvBounds.v + uvBounds.vHeight, renderColour),
			Vertex(centerUPixel[0] + halfSizeUPixel[0], centerUPixel[1] - halfSizeUPixel[1], depth, uvBounds.u + uvBounds.uWidth, uvBounds.v + uvBounds.vHeight, renderColour)

		}), &blendMode).dispose();
	}
	TextRectangle::TextRectangle() {
	}
	TextRectangle::~TextRectangle() {
	}
	void Val::TextRectangle::initialise(TextResource text, const float & x, const float & y, const float & depth, const float & halfWidth, const float & halfHeight, const float & rotation, const Colour & colour, const GLBlendMode & blendMode) {
		SimpleTextRectangle::initialise(text, x, y, depth, halfWidth, halfHeight, colour, blendMode);

		this->rotation = rotation;
	}
	void Val::TextRectangle::setRotation(const float & rotation) {
		this->rotation = rotation;
		needsReconstructed = true;
	}
	float Val::TextRectangle::getRotation() const {
		return rotation;
	}
	void Val::TextRectangle::initialise(TextResource text, const float & x, const float & y, const float & depth, const float & halfWidth, const float & halfHeight, const Colour & colour, const GLBlendMode & blendMode) {
	}
	void Val::TextRectangle::recalculateVertexes() {
		needsReconstructed = false;

		texture = TextRenderer::getTexture(textResource);

		if (texture.getTexture() == nullptr) {
			Logger::Instance->logMessage(WARNING, "Invalid Texture on Text Rectangle");
			return;
		}

		if (scaleTextToHeight) {
			auto texSize = texture.getTextureSizeInPixel();
			halfSize[0] = (halfSize[1] / texSize[1]) * texSize[0];
		}

		std::array<float, 2> centerUPixel = { WorldToUnalignedPixel<float>(center[0]), WorldToUnalignedPixel<float>(center[1]) };
		std::array<float, 2> halfSizeUPixel = { WorldToUnalignedPixel<float>(halfSize[0]), WorldToUnalignedPixel<float>(halfSize[1]) };

		float cosAngle = static_cast<float>(cos(rotation));
		float sinAngle = static_cast<float>(sin(rotation));

		float hCos = halfSizeUPixel[1] * cosAngle;
		float wCos = halfSizeUPixel[0] * cosAngle;
		float hSin = halfSizeUPixel[1] * sinAngle;
		float wSin = halfSizeUPixel[0] * sinAngle;

		Glyph = RectangleGlyph(texture.getGLTexture()->getTextureID(), std::array<Vertex, 4>({

			Vertex(centerUPixel[0] - wCos + hSin, centerUPixel[1] - hCos - wSin, depth, uvBounds.u + uvBounds.uWidth, uvBounds.v, renderColour),
			Vertex(centerUPixel[0] + wCos + hSin, centerUPixel[1] - hCos + wSin, depth, uvBounds.u, uvBounds.v, renderColour),
			Vertex(centerUPixel[0] - wCos - hSin, centerUPixel[1] + hCos - wSin , depth, uvBounds.u + uvBounds.uWidth, uvBounds.v + uvBounds.vHeight, renderColour),
			Vertex(centerUPixel[0] + wCos - hSin, centerUPixel[1] + hCos + wSin, depth, uvBounds.u, uvBounds.v + uvBounds.vHeight, renderColour)

		}), &blendMode).dispose();
	}
}