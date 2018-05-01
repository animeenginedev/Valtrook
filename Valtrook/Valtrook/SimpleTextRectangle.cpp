#include "SimpleTextRectangle.h"

#include "TextRenderer.h"
#include "Logger.h"
#include "Conversion.h"
#include "RenderingEngine.h"
#include "VBOBatcher.h"
#include "GlyphCuller.h"


namespace Val {
	SimpleTextRectangle::SimpleTextRectangle(const TextResource & texture) : SimpleTextRectangle(texture, { 0.0f, 0.0f }, 0.5f, { 1.0f, 1.0f }, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	SimpleTextRectangle::SimpleTextRectangle(const TextResource & texture, float x, float y, float depth, float halfWidth, float halfHeight) : SimpleTextRectangle(texture, { x, y }, depth, { halfWidth, halfHeight }, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	SimpleTextRectangle::SimpleTextRectangle(const TextResource & texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize) : SimpleTextRectangle(texture, center, depth, halfSize, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	SimpleTextRectangle::SimpleTextRectangle(const TextResource & texture, float x, float y, float depth, float halfWidth, float halfHeight, Colour colour, const GLBlendMode & blendMode) : SimpleTextRectangle(texture, { x, y }, depth, { halfWidth, halfHeight }, colour, blendMode) {
	}
	SimpleTextRectangle::SimpleTextRectangle(const TextResource & texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize, Colour colour, const GLBlendMode & blendMode) :
	    ATexturedRect(TextureResource(nullptr, ResourceLocation("", "", "")), center, halfSize, depth, colour, blendMode), textResource(texture), cullAABB(0.0f, 0.0f, 0.0f, 0.0f), bHasCullSurface(false) {
	}
	SimpleTextRectangle::~SimpleTextRectangle() {
	}

	void SimpleTextRectangle::setTextResource(const TextResource & text) {
		textResource = text;
		needsReconstructed = true;
	}

	void SimpleTextRectangle::setText(const std::string & contents) {
		textResource = TextResource(textResource.getFont(), contents);
		needsReconstructed = true;
	}

	void SimpleTextRectangle::setFont(FontAsset * newFont) {
		textResource = TextResource(newFont, textResource.getTextString());
		needsReconstructed = true;
	}
	void SimpleTextRectangle::setScaleTextToHeight(bool scaleTextToHeight) {
		this->scaleTextToHeight = scaleTextToHeight;
		needsReconstructed = true;
	}
	void SimpleTextRectangle::setCullSurface(AABB<float> cullAABB) {
		static AABB<float> noCullSurface = { 0.0f, 0.0f, 0.0f, 0.0f };
		this->cullAABB = cullAABB;
		bHasCullSurface = !(cullAABB == noCullSurface);
		needsReconstructed = true;
	}
	TextResource SimpleTextRectangle::getTextResource() const {
		return textResource;
	}
	std::string SimpleTextRectangle::getText() const {
		return textResource.getTextString();
	}
	FontAsset * SimpleTextRectangle::getFont() const {
		return textResource.getFont();
	}
	bool SimpleTextRectangle::doesScaleTextToHeight() const {
		return scaleTextToHeight;
	}
	AABB<float> SimpleTextRectangle::getCullSurface() const {
		return cullAABB;
	}
	void SimpleTextRectangle::reconstruct() {
		if (needsReconstructed) {
			recalculateVertexes();
		}
	}
	std::array<TriangleGlyph, 2> SimpleTextRectangle::getRenderGlyphs() {
		if (needsReconstructed)
			recalculateVertexes();
		return Glyph;
	}
	void SimpleTextRectangle::sendRenderInformation(RenderingEngine * engine) {
		if (needsReconstructed)
			recalculateVertexes();

		engine->getRenderer()->add(Glyph);
	}
	void SimpleTextRectangle::sendRenderInformation(VBOBatcher * batcher) {
		if (needsReconstructed)
			recalculateVertexes();

		batcher->add(Glyph);
	}
	void SimpleTextRectangle::recalculateVertexes() {
		needsReconstructed = false;

		texture = TextRenderer::getTexture(textResource);
		Colour renderColour = this->colour;

		if (texture.getTexture() == nullptr) {
			Logger::Instance->logMessage(WARNING, "Invalid Texture on Simple Text Rectangle");
			texture = Texture::errorTexture;
			renderColour = Colour(255, 255, 255, this->colour.a);
		}

		uvBounds = texture.getBounds();

		if (scaleTextToHeight) {
			auto texSize = texture.getTextureSizeInPixel();
			halfSize[0] = (halfSize[1] / texSize[1]) * texSize[0];
		}

		std::array<float, 2> centerUPixel = { WorldToUnalignedPixel<float>(center[0]), WorldToUnalignedPixel<float>(center[1]) };
		std::array<float, 2> halfSizeUPixel = { WorldToUnalignedPixel<float>(halfSize[0]), WorldToUnalignedPixel<float>(halfSize[1]) };

		auto Glyph = RectangleGlyph(texture.getGLTexture()->getTextureID(), std::array<Vertex, 4>({

			Vertex(centerUPixel[0] - halfSizeUPixel[0], centerUPixel[1] + halfSizeUPixel[1], depth, uvBounds.u, uvBounds.v, renderColour),
			Vertex(centerUPixel[0] + halfSizeUPixel[0], centerUPixel[1] + halfSizeUPixel[1], depth, uvBounds.u + uvBounds.uWidth, uvBounds.v, renderColour),
			Vertex(centerUPixel[0] - halfSizeUPixel[0], centerUPixel[1] - halfSizeUPixel[1], depth, uvBounds.u, uvBounds.v + uvBounds.vHeight, renderColour),
			Vertex(centerUPixel[0] + halfSizeUPixel[0], centerUPixel[1] - halfSizeUPixel[1], depth, uvBounds.u + uvBounds.uWidth, uvBounds.v + uvBounds.vHeight, renderColour)
																								  
		}), &blendMode);

		this->Glyph = bHasCullSurface ? GlyphCuller::cullRectangle(Glyph, cullAABB).dispose() : Glyph.dispose();
	}
}