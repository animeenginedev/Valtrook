#include "SimpleRectangle.h"

#include "RenderingEngine.h"
#include "VBOBatcher.h"
#include "Logger.h"
#include "Conversion.h"
#include "ArrayMath.h"

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
		texture(texture), center(center), depth(depth), halfSize(halfSize), cullAABB({ 0.0f, 0.0f, 0.0f, 0.0f }), bHasCullSurface(false), renderColour(colour), blendMode(blendMode), uvBounds(texture.getBounds()), needsReconstructed(true) {
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
	void SimpleRectangle::setCullSurface(AABB<float> cullAABB) {
		static AABB<float> noCullSurface = { 0.0f, 0.0f, 0.0f, 0.0f };
		this->cullAABB = cullAABB;
		bHasCullSurface = !(cullAABB == noCullSurface);
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
	AABB<float> SimpleRectangle::getCullSurface() const {
		return cullAABB;
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

			texture = Texture::errorTexture;
		}

		if (bHasCullSurface) {
			AABB<float> thisAABB = AABB<float>(center[0], center[1], halfSize[0], halfSize[1]);
			if (thisAABB.isCompletlyInside(cullAABB)) {
				//goto looooooool
				goto normalRender;
			} else if (thisAABB.intersectsAABB(cullAABB)) {
				auto resultAABB = thisAABB.getCulledAABB(cullAABB);

				std::array<float, 2> minUPixel = { WorldToUnalignedPixel<float>(resultAABB.minX()), WorldToUnalignedPixel<float>(resultAABB.minY()) };
				std::array<float, 2> maxUPixel = { WorldToUnalignedPixel<float>(resultAABB.maxX()), WorldToUnalignedPixel<float>(resultAABB.maxY()) };

				float uStart = uvBounds.u + (uvBounds.uWidth * ((resultAABB.minX() - thisAABB.minX()) / (thisAABB.maxX() - thisAABB.minX())));
				float vStart = uvBounds.v + (uvBounds.vHeight * ((resultAABB.minY() - thisAABB.minY()) / (thisAABB.maxY() - thisAABB.minY())));
				float uWidth = uvBounds.uWidth * ((resultAABB.maxX() - resultAABB.minX()) / (thisAABB.maxX() - thisAABB.minX()));
				float vHeight = uvBounds.vHeight * ((resultAABB.maxY() - resultAABB.minY()) / (thisAABB.maxY() - thisAABB.minY()));

				vStart = (uvBounds.vHeight - vHeight) - vStart;
				
				Glyph = RectangleGlyph(texture.getGLTexture()->getTextureID(), std::array<Vertex, 4>({

					Vertex(minUPixel[0], maxUPixel[1], depth, uStart, vStart, renderColour),
					Vertex(maxUPixel[0], maxUPixel[1], depth, uStart + uWidth, vStart, renderColour),
					Vertex(minUPixel[0], minUPixel[1], depth, uStart, vStart + vHeight, renderColour),
					Vertex(maxUPixel[0], minUPixel[1], depth, uStart + uWidth, vStart + vHeight, renderColour)

				}), &blendMode).dispose();

			} else {
				Glyph = RectangleGlyph(texture.getGLTexture()->getTextureID(), std::array<Vertex, 4>({
				}), &blendMode).dispose();
			}
			//We're completely culled so who gives a fuck
		} else {
			normalRender:
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
}