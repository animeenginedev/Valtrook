#include "Rectangle.h"

#include "Conversion.h"
#include "RenderingEngine.h"
#include "VBOBatcher.h"
#include "Logger.h"

namespace Val {
	SimpleRectangle::SimpleRectangle() {
	}
	SimpleRectangle::~SimpleRectangle() {
	}
	void SimpleRectangle::initialise(const TextureResource & texture, const float & x, const float & y, const float & depth, const float & halfWidth, const float & halfHeight, const Colour & colour, const GLBlendMode & blendMode) {
		this->texture = texture;
		this->center = { x, y };
		this->depth = depth;
		this->halfSize = { halfWidth, halfHeight };
		this->renderColour = colour;
		this->uvBounds = texture.getBounds();
		this->blendMode = blendMode;
		
		needsReconstructed = true;
	}
	void SimpleRectangle::setTexture(const TextureResource & texture) {
		this->texture = texture;
		this->uvBounds = texture.getBounds();
		needsReconstructed = true;
	}
	void SimpleRectangle::setX(const float & x) {
		this->center[0] = x;
		needsReconstructed = true;
	}
	void SimpleRectangle::setY(const float & y) {
		this->center[1] = y;
		needsReconstructed = true;
	}
	void SimpleRectangle::setDepth(const float & depth) {
		this->depth = depth;
		needsReconstructed = true;
	}
	void SimpleRectangle::setCenter(const std::array<float, 2>& center) {
		this->center = center;
		needsReconstructed = true;
	}
	void SimpleRectangle::setCenter(const float & x, const float & y) {
		this->center[0] = x;
		this->center[1] = y;
		needsReconstructed = true;
	}
	void SimpleRectangle::setHalfWidth(const float & h_width) {
		this->halfSize[0] = h_width;
		needsReconstructed = true;
	}
	void SimpleRectangle::setHalfHeight(const float & h_height) {
		this->halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void SimpleRectangle::setHalfSize(const std::array<float, 2>& halfSize) {
		this->halfSize = halfSize;
		needsReconstructed = true;
	}
	void SimpleRectangle::setHalfSize(const float & h_width, const float & h_height) {
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
	void Val::Rectangle::initialise(const TextureResource & texture, const float & x, const float & y, const float & depth, const float & halfWidth, const float & halfHeight, const float & rotation, const Colour & colour, const GLBlendMode & blendMode) {
		this->texture = texture;
		this->center = { x, y };
		this->depth = depth;
		this->halfSize = { halfWidth, halfHeight };
		this->rotation = rotation;
		this->renderColour = colour;
		this->uvBounds = texture.getBounds();
		this->blendMode = blendMode;

		needsReconstructed = true;
	}
	void Rectangle::setRotation(const float & rotation) {
		this->rotation = rotation;
		needsReconstructed = true;
	}
	float Rectangle::getRotation() const {
		return rotation;
	}
	void Rectangle::initialise(const TextureResource & texture, const float & x, const float & y, const float & depth, const float & halfWidth, const float & halfHeight, const Colour & colour, const GLBlendMode & blendMode) {
	}
	void Rectangle::recalculateVertexes() {
		needsReconstructed = false;

		if (texture.getTexture() == nullptr) {
			Logger::Instance->logMessage(WARNING, "Invalid Texture on Rectangle");
			return;
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