#include "Rectangle.h"

#include "Conversion.h"
#include "RenderingEngine.h"
#include "VBOBatcher.h"
#include "Logger.h"
#include "Rectangle.h"

namespace Val {
	Rectangle::Rectangle(const TextureResource & texture) : Rectangle(texture, { 0.0f, 0.0f }, 0.5f, { 1.0f, 1.0f }, 0.0f, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	Rectangle::Rectangle(const TextureResource & texture, float x, float y, float depth, float halfWidth, float halfHeight, float rotation) : Rectangle(texture, { x, y }, depth, { halfWidth, halfHeight }, rotation, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	Rectangle::Rectangle(const TextureResource & texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize, float rotation) : Rectangle(texture, center, depth, halfSize, rotation, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	Rectangle::Rectangle(const TextureResource & texture, float x, float y, float depth, float halfWidth, float halfHeight, float rotation, Colour colour, const GLBlendMode & blendMode) : Rectangle(texture, { x, y }, depth, { halfWidth, halfHeight }, rotation, colour, blendMode) {
	}
	Rectangle::Rectangle(const TextureResource & texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize, float rotation, Colour colour, const GLBlendMode & blendMode) :
		texture(texture), center(center), depth(depth), halfSize(halfSize), rotation(rotation), renderColour(colour), blendMode(blendMode), uvBounds(texture.getBounds()), needsReconstructed(true) {
	}
	Rectangle::~Rectangle() {
	}

	void Rectangle::setTexture(const TextureResource & texture) {
		this->texture = texture;
		this->uvBounds = texture.getBounds();
		needsReconstructed = true;
	}
	void Rectangle::setX(float x) {
		this->center[0] = x;
		needsReconstructed = true;
	}
	void Rectangle::setY(float y) {
		this->center[1] = y;
		needsReconstructed = true;
	}
	void Rectangle::setDepth(float depth) {
		this->depth = depth;
		needsReconstructed = true;
	}
	void Rectangle::setCenter(const std::array<float, 2>& center) {
		this->center = center;
		needsReconstructed = true;
	}
	void Rectangle::setCenter(float x, float y) {
		this->center[0] = x;
		this->center[1] = y;
		needsReconstructed = true;
	}
	void Rectangle::setHalfWidth(float h_width) {
		this->halfSize[0] = h_width;
		needsReconstructed = true;
	}
	void Rectangle::setHalfHeight(float h_height) {
		this->halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void Rectangle::setHalfSize(const std::array<float, 2>& halfSize) {
		this->halfSize = halfSize;
		needsReconstructed = true;
	}
	void Rectangle::setHalfSize(float h_width, float h_height) {
		this->halfSize[0] = h_width;
		this->halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void Rectangle::setWidth(float width) {
		this->halfSize[0] = width / 2.0f;
		needsReconstructed = true;
	}
	void Rectangle::setHeight(float height) {
		this->halfSize[1] = height / 2.0f;
		needsReconstructed = true;
	}
	void Rectangle::setSize(float width, float height) {
		this->halfSize[0] = width / 2.0f;
		this->halfSize[1] = height / 2.0f;
		needsReconstructed = true;
	}
	void Rectangle::setColour(const Colour & colour) {
		this->renderColour = colour;
		needsReconstructed = true;
	}
	void Rectangle::setUV(const UV & uv) {
		this->uvBounds = uv;
		needsReconstructed = true;
	}
	void Rectangle::setBlendMode(const GLBlendMode & blendMode) {
		this->blendMode = blendMode;
		needsReconstructed = true;
	}
	void Rectangle::setRotation(float rotation) {
		this->rotation = rotation;
		needsReconstructed = true;
	}
	TextureResource Rectangle::getTexture() const {
		return texture;
	}
	float Rectangle::getX() const {
		return center[0];
	}
	float Rectangle::getY() const {
		return center[1];
	}
	float Rectangle::getDepth() const {
		return depth;
	}
	std::array<float, 2> Rectangle::getCenter() const {
		return center;
	}
	float Rectangle::getHalfWidth() const {
		return halfSize[0];
	}
	float Rectangle::getHalfHeight() const {
		return halfSize[1];
	}
	std::array<float, 2> Rectangle::getHalfSize() const {
		return halfSize;
	}
	Colour Rectangle::getColour() const {
		return renderColour;
	}
	UV Rectangle::getUV() const {
		return uvBounds;
	}
	GLBlendMode Rectangle::getBlendMode() const {
		return blendMode;
	}
	float Rectangle::getRotation() const {
		return rotation;
	}

	std::array<TriangleGlyph, 2> Rectangle::getRenderGlyphs() {
		if (needsReconstructed)
			recalculateVertexes();
		return Glyph;
	}
	void Rectangle::sendRenderInformation(RenderingEngine * engine) {
		if (needsReconstructed)
			recalculateVertexes();

		engine->getRenderer()->add(Glyph);
	}
	void Rectangle::sendRenderInformation(VBOBatcher * batcher) {
		if (needsReconstructed)
			recalculateVertexes();

		batcher->add(Glyph);
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