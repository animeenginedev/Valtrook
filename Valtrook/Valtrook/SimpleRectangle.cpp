#include "SimpleRectangle.h"

#include "RenderingEngine.h"
#include "VBOBatcher.h"
#include "Logger.h"
#include "Conversion.h"

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
		texture(texture), center(center), depth(depth), halfSize(halfSize), renderColour(colour), blendMode(blendMode), uvBounds(texture.getBounds()), needsReconstructed(true) {
	}
	SimpleRectangle::~SimpleRectangle() {
	}

	void SimpleRectangle::setTexture(const TextureResource & texture) {
		this->texture = texture;
		this->uvBounds = texture.getBounds();
		needsReconstructed = true;
	}
	void SimpleRectangle::setX(float x) {
		this->center[0] = x;
		needsReconstructed = true;
	}
	void SimpleRectangle::setY(float y) {
		this->center[1] = y;
		needsReconstructed = true;
	}
	void SimpleRectangle::setDepth(float depth) {
		this->depth = depth;
		needsReconstructed = true;
	}
	void SimpleRectangle::setCenter(const std::array<float, 2>& center) {
		this->center = center;
		needsReconstructed = true;
	}
	void SimpleRectangle::setCenter(float x, float y) {
		this->center[0] = x;
		this->center[1] = y;
		needsReconstructed = true;
	}
	void SimpleRectangle::setHalfWidth(float h_width) {
		this->halfSize[0] = h_width;
		needsReconstructed = true;
	}
	void SimpleRectangle::setHalfHeight(float h_height) {
		this->halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void SimpleRectangle::setHalfSize(const std::array<float, 2>& halfSize) {
		this->halfSize = halfSize;
		needsReconstructed = true;
	}
	void SimpleRectangle::setHalfSize(float h_width, float h_height) {
		this->halfSize[0] = h_width;
		this->halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void SimpleRectangle::setWidth(float width) {
		this->halfSize[0] = width / 2.0f;
		needsReconstructed = true;
	}
	void SimpleRectangle::setHeight(float height) {
		this->halfSize[1] = height / 2.0f;
		needsReconstructed = true;
	}
	void SimpleRectangle::setSize(float width, float height) {
		this->halfSize[0] = width / 2.0f;
		this->halfSize[1] = height / 2.0f;
		needsReconstructed = true;
	}
	void SimpleRectangle::setColour(const Colour & colour) {
		this->renderColour = colour;
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
	TextureResource SimpleRectangle::getTexture() const {
		return texture;
	}
	float SimpleRectangle::getX() const {
		return center[0];
	}
	float SimpleRectangle::getY() const {
		return center[1];
	}
	float SimpleRectangle::getDepth() const {
		return depth;
	}
	std::array<float, 2> SimpleRectangle::getCenter() const {
		return center;
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
	Colour SimpleRectangle::getColour() const {
		return renderColour;
	}
	UV SimpleRectangle::getUV() const {
		return uvBounds;
	}
	GLBlendMode SimpleRectangle::getBlendMode() const {
		return blendMode;
	}
	std::array<TriangleGlyph, 2> SimpleRectangle::getRenderGlyphs() {
		if (needsReconstructed)
			recalculateVertexes();
		return Glyph;
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
			return;
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
}