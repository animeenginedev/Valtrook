#include "SimpleRectangle.h"

#include "RenderingEngine.h"
#include "VBOBatcher.h"
#include "Logger.h"
#include "Conversion.h"
#include "ArrayMath.h"
#include "GlyphCuller.h"
#include "TextureAsset.h"

namespace Val {
	SimpleRectangle::SimpleRectangle(const TextureResource & texture) : SimpleRectangle(texture, { 0.0f, 0.0f }, 0.5f, { 1.0f, 1.0f }, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	SimpleRectangle::SimpleRectangle(const TextureResource & texture, float x, float y, float depth, float halfWidth, float halfHeight) : SimpleRectangle(texture, {x, y}, depth, { halfWidth, halfHeight }, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	SimpleRectangle::SimpleRectangle(const TextureResource & texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize) : SimpleRectangle(texture, center, depth, halfSize, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	SimpleRectangle::SimpleRectangle(const TextureResource & texture, float x, float y, float depth, float halfWidth, float halfHeight, Colour colour, const GLBlendMode & blendMode) : SimpleRectangle(texture, { x, y }, depth, { halfWidth, halfHeight }, colour, blendMode) {
	}
	SimpleRectangle::SimpleRectangle(const TextureResource & texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize, Colour colour, const GLBlendMode & blendMode) :
		center(center), depth(depth), halfSize(halfSize), needsReconstructed(true), texture(texture), uvBounds(texture.getBounds()), colour(colour), blendMode(blendMode), cullAABB({ 0.0f, 0.0f, 0.0f, 0.0f }), bHasCullSurface(false) {
	}
	SimpleRectangle::~SimpleRectangle() {
	}
	SimpleRectangle createSimpleRectangle(std::string texturePath, float x, float y, float depth, float hW, float hH) {
		return SimpleRectangle(TextureAsset::getTexture(texturePath), x, y, depth, hW, hH);
	}
	void SimpleRectangle::setTexture(const TextureResource & texture) {
		this->texture = texture;
		this->uvBounds = texture.getBounds();
		needsReconstructed = true;
	}
	void SimpleRectangle::setHalfWidth(float h_width) {
		halfSize[0] = h_width;
		needsReconstructed = true;
	}
	void SimpleRectangle::setHalfHeight(float h_height) {
		halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void SimpleRectangle::setHalfSize(const std::array<float, 2>& halfSize) {
		this->halfSize = halfSize;
		needsReconstructed = true;
	}
	void SimpleRectangle::setHalfSize(float h_width, float h_height) {
		halfSize[0] = h_width;
		halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void SimpleRectangle::setColour(const Colour & colour) {
		this->colour = colour;
		needsReconstructed = true;
	}
	void SimpleRectangle::setUV(const UV & uv) {
		this->uvBounds = uv;
		needsReconstructed = true;
	}
	void SimpleRectangle::setBlendMode(const GLBlendMode & blendMode) {
		this->blendMode = blendMode;
		needsReconstructed = true;
	}
	Colour SimpleRectangle::getColour() const {
		return colour;
	}
	UV SimpleRectangle::getUV() const {
		return uvBounds;
	}
	GLBlendMode SimpleRectangle::getBlendMode() const {
		return blendMode;
	}
	void SimpleRectangle::setCullSurface(AABB<float> cullAABB) {
		static AABB<float> noCullSurface = { 0.0f, 0.0f, 0.0f, 0.0f };
		this->cullAABB = cullAABB;
		bHasCullSurface = !(cullAABB == noCullSurface);
		needsReconstructed = true;
	}
	TextureResource SimpleRectangle::getTexture() const {
		return texture;
	}
	AABB<float> SimpleRectangle::getCullSurface() const {
		return cullAABB;
	}
	std::array<TriangleGlyph, 2> SimpleRectangle::getRenderGlyphs() {
		if (needsReconstructed)
			recalculateVertexes();
		return Glyph;
	}
	void SimpleRectangle::setX(float x) {
		center[0] = x;
		needsReconstructed = true;
	}
	void SimpleRectangle::setY(float y) {
		center[1] = y;
		needsReconstructed = true;
	}
	void SimpleRectangle::setCenter(float x, float y) {
		center[0] = x;
		center[1] = y;
		needsReconstructed = true;
	}
	void SimpleRectangle::setCenter(std::array<float, 2> center) {
		this->center = center;
		needsReconstructed = true;
	}
	void SimpleRectangle::setDepth(float depth) {
		this->depth = depth;
		needsReconstructed = true;
	}
	float SimpleRectangle::getX() const {
		return center[0];
	}
	float SimpleRectangle::getY() const {
		return center[1];
	}
	std::array<float, 2> SimpleRectangle::getCenter() const {
		return center;
	}
	float SimpleRectangle::getDepth() const {
		return depth;
	}
	float SimpleRectangle::getHalfWidth() const {
		return halfSize[0];
	}
	float SimpleRectangle::getHalfHeight() const {
		return halfSize[1];
	}
	std::array<float, 2> SimpleRectangle::getHalfSize() const {
		return halfSize;
	}
	void SimpleRectangle::sendRenderInformation(RenderingEngine * engine) {
		if (needsReconstructed)
			recalculateVertexes();

		engine->getRenderer()->add(Glyph);
	}
	void SimpleRectangle::sendRenderInformation(VBOBatcher * batcher) {
		if (needsReconstructed)
			recalculateVertexes();

		batcher->add(Glyph);
	}

	void SimpleRectangle::recalculateVertexes() {
		needsReconstructed = false;

		if (texture.getTexture() == nullptr) {
			Logger::Instance->logMessage(WARNING, "Invalid Texture on Simple Rectangle");

			texture = Texture::errorTexture;
		}

		std::array<float, 2> centerUPixel = { WorldToUnalignedPixel<float>(center[0]), WorldToUnalignedPixel<float>(center[1]) };
		std::array<float, 2> halfSizeUPixel = { WorldToUnalignedPixel<float>(halfSize[0]), WorldToUnalignedPixel<float>(halfSize[1]) };


		auto Glyph = RectangleGlyph(texture.getGLTexture()->getTextureID(), std::array<Vertex, 4>({

			Vertex(centerUPixel[0] - halfSizeUPixel[0], centerUPixel[1] + halfSizeUPixel[1], depth, uvBounds.u, uvBounds.v, colour),
			Vertex(centerUPixel[0] + halfSizeUPixel[0], centerUPixel[1] + halfSizeUPixel[1], depth, uvBounds.u + uvBounds.uWidth, uvBounds.v, colour),
			Vertex(centerUPixel[0] - halfSizeUPixel[0], centerUPixel[1] - halfSizeUPixel[1], depth, uvBounds.u, uvBounds.v + uvBounds.vHeight, colour),
			Vertex(centerUPixel[0] + halfSizeUPixel[0], centerUPixel[1] - halfSizeUPixel[1], depth, uvBounds.u + uvBounds.uWidth, uvBounds.v + uvBounds.vHeight, colour)

		}), &blendMode);

		if (bHasCullSurface)
			this->Glyph = (GlyphCuller::cullRectangle(Glyph, cullAABB)).dispose();
		else
			this->Glyph = Glyph.dispose();

	}
}