#include "TextRectangle.h"

#include "TextRenderer.h"
#include "Logger.h"
#include "Conversion.h"
#include "RenderingEngine.h"
#include "VBOBatcher.h"

namespace Val {
	TextRectangle::TextRectangle(const TextResource & texture) : TextRectangle(texture, { 0.0f, 0.0f }, 0.5f, { 1.0f, 1.0f }, 0.0f, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	TextRectangle::TextRectangle(const TextResource & texture, float x, float y, float depth, float halfWidth, float halfHeight, float rotation) : TextRectangle(texture, { x, y }, depth, { halfWidth, halfHeight }, rotation, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	TextRectangle::TextRectangle(const TextResource & texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize, float rotation) : TextRectangle(texture, center, depth, halfSize, rotation, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	TextRectangle::TextRectangle(const TextResource & texture, float x, float y, float depth, float halfWidth, float halfHeight, float rotation, Colour colour, const GLBlendMode & blendMode) : TextRectangle(texture, { x, y }, depth, { halfWidth, halfHeight }, rotation, colour, blendMode) {
	}
	TextRectangle::TextRectangle(const TextResource & texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize, float rotation, Colour colour, const GLBlendMode & blendMode) :
		textResource(texture), center(center), depth(depth), halfSize(halfSize), rotation(rotation), renderColour(colour), blendMode(blendMode), needsReconstructed(true) {
	}
	TextRectangle::~TextRectangle() {
	}

	void TextRectangle::setTextResource(const TextResource & text) {
		textResource = text;
		needsReconstructed = true;
	}

	void TextRectangle::setText(const std::string & contents) {
		textResource = TextResource(textResource.getFont(), contents);
		needsReconstructed = true;
	}

	void TextRectangle::setFont(FontAsset * newFont) {
		textResource = TextResource(newFont, textResource.getTextString());
		needsReconstructed = true;
	}

	void TextRectangle::setX(float x) {
		this->center[0] = x;
		needsReconstructed = true;
	}
	void TextRectangle::setY(float y) {
		this->center[1] = y;
		needsReconstructed = true;
	}
	void TextRectangle::setDepth(float depth) {
		this->depth = depth;
		needsReconstructed = true;
	}
	void TextRectangle::setCenter(const std::array<float, 2>& center) {
		this->center = center;
		needsReconstructed = true;
	}
	void TextRectangle::setCenter(float x, float y) {
		this->center[0] = x;
		this->center[1] = y;
		needsReconstructed = true;
	}
	void TextRectangle::setHalfWidth(float h_width) {
		this->halfSize[0] = h_width;
		needsReconstructed = true;
	}
	void TextRectangle::setHalfHeight(float h_height) {
		this->halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void TextRectangle::setHalfSize(const std::array<float, 2>& halfSize) {
		this->halfSize = halfSize;
		needsReconstructed = true;
	}
	void TextRectangle::setHalfSize(float h_width, float h_height) {
		this->halfSize[0] = h_width;
		this->halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void TextRectangle::setWidth(float width) {
		this->halfSize[0] = width / 2.0f;
		needsReconstructed = true;
	}
	void TextRectangle::setHeight(float height) {
		this->halfSize[1] = height / 2.0f;
		needsReconstructed = true;
	}
	void TextRectangle::setSize(float width, float height) {
		this->halfSize[0] = width / 2.0f;
		this->halfSize[1] = height / 2.0f;
		needsReconstructed = true;
	}
	void TextRectangle::setColour(const Colour & colour) {
		this->renderColour = colour;
		needsReconstructed = true;
	}
	void TextRectangle::setBlendMode(const GLBlendMode & blendMode) {
		this->blendMode = blendMode;
		needsReconstructed = true;
	}
	void TextRectangle::setScaleTextToHeight(bool scaleTextToHeight) {
		this->scaleTextToHeight = scaleTextToHeight;
		needsReconstructed = true;
	}
	void Val::TextRectangle::setRotation(float rotation) {
		this->rotation = rotation;
		needsReconstructed = true;
	}
	TextResource TextRectangle::getTextResource() const {
		return textResource;
	}
	std::string TextRectangle::getText() const {
		return textResource.getTextString();
	}
	FontAsset * TextRectangle::getFont() const {
		return textResource.getFont();
	}
	float TextRectangle::getX() const {
		return center[0];
	}
	float TextRectangle::getY() const {
		return center[1];
	}
	float TextRectangle::getDepth() const {
		return depth;
	}
	std::array<float, 2> TextRectangle::getCenter() const {
		return center;
	}
	float TextRectangle::getHalfWidth() const {
		return halfSize[0];
	}
	float TextRectangle::getHalfHeight() const {
		return halfSize[1];
	}
	std::array<float, 2> TextRectangle::getHalfSize() const {
		return halfSize;
	}
	Colour TextRectangle::getColour() const {
		return renderColour;
	}
	GLBlendMode TextRectangle::getBlendMode() const {
		return blendMode;
	}
	float Val::TextRectangle::getRotation() const {
		return rotation;
	}
	bool TextRectangle::doesScaleTextToHeight() const {
		return scaleTextToHeight;
	}
	void TextRectangle::reconstruct() {
		if (needsReconstructed) {
			recalculateVertexes();
		}
	}
	std::array<TriangleGlyph, 2> TextRectangle::getRenderGlyphs() {
		if (needsReconstructed)
			recalculateVertexes();
		return Glyph;
	}
	void TextRectangle::sendRenderInformation(RenderingEngine * engine) {
		if (needsReconstructed)
			recalculateVertexes();

		engine->getRenderer()->add(Glyph);
	}
	void TextRectangle::sendRenderInformation(VBOBatcher * batcher) {
		if (needsReconstructed)
			recalculateVertexes();

		batcher->add(Glyph);
	}
	void TextRectangle::recalculateVertexes() {
		needsReconstructed = false;

		auto texture = TextRenderer::getTexture(textResource);;

		auto renderColour = this->renderColour;

		if (texture.getTexture() == nullptr) {
			Logger::Instance->logMessage(WARNING, "Invalid Texture on Text Rectangle");
			texture = Texture::errorTexture;
			renderColour = Colour(255, 255, 255, this->renderColour.a);
		}

		auto uvBounds = texture.getBounds();

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