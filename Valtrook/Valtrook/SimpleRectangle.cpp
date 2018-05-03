#include "SimpleRectangle.h"

#include "RenderingEngine.h"
#include "VBOBatcher.h"
#include "Logger.h"
#include "Conversion.h"
#include "ArrayMath.h"
#include "GlyphCuller.h"

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
		ATexturedRect(texture, center, halfSize, depth, colour, blendMode), cullAABB({ 0.0f, 0.0f, 0.0f, 0.0f }), bHasCullSurface(false) {
	}
	SimpleRectangle::~SimpleRectangle() {
	}
	void SimpleRectangle::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::user_type<SimpleRectangle>(), "SimpleRectangle");
		script->add(chaiscript::base_class<ATexturedRect, SimpleRectangle>());

		script->add(chaiscript::constructor<SimpleRectangle(TextureResource)>(), "SimpleRectangle");
		script->add(chaiscript::constructor<SimpleRectangle(TextureResource, float, float, float, float, float)>(), "SimpleRectangle");
		script->add(chaiscript::constructor<SimpleRectangle(TextureResource, std::array<float, 2>, float, std::array<float, 2>)>(), "SimpleRectangle");
		script->add(chaiscript::constructor<SimpleRectangle(TextureResource, float, float, float, float, float, Colour, GLBlendMode)>(), "SimpleRectangle");
		script->add(chaiscript::constructor<SimpleRectangle(TextureResource, std::array<float, 2>, float, std::array<float, 2>, Colour, GLBlendMode)>(), "SimpleRectangle");

		script->add(chaiscript::fun(&SimpleRectangle::getTexture), "getTexture");

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