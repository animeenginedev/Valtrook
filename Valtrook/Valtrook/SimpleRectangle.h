#pragma once

#include "TextureAsset.h"
#include "Glyph.h"
#include "Colour.h"
#include "GLBlendMode.h"

namespace Val {
	class RenderingEngine;
	class VBOBatcher;

	class SimpleRectangle {
	public:
		SimpleRectangle(const TextureResource& texture);
		SimpleRectangle(const TextureResource& texture, float x, float y, float depth, float halfWidth, float halfHeight);
		SimpleRectangle(const TextureResource& texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize);
		SimpleRectangle(const TextureResource& texture, float x, float y, float depth, float halfWidth, float halfHeight, Colour colour, const GLBlendMode& blendMode = GLBlendMode::Blend_Default);
		SimpleRectangle(const TextureResource& texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize, Colour colour, const GLBlendMode& blendMode = GLBlendMode::Blend_Default);
		~SimpleRectangle();

		void setTexture(const TextureResource& texture);
		void setX(float x);
		void setY(float y);
		void setDepth(float depth);
		void setCenter(const std::array<float, 2>& center);
		void setCenter(float x, float y);
		void setHalfWidth(float h_width);
		void setHalfHeight(float h_height);
		void setHalfSize(const std::array<float, 2>& halfSize);
		void setHalfSize(float h_width, float h_height);
		void setWidth(float width);
		void setHeight(float height);
		void setSize(float width, float height);
		void setColour(const Colour& colour);
		void setUV(const UV& uv);
		void setBlendMode(const GLBlendMode& blendMode);
		//Honestly, don't ask. It's really used to GUI rendering, setting the center/height/width and the uv dependant on the culling aabb size&position
		void setCullSurface(AABB<float> cullAABB);

		TextureResource getTexture() const;
		float getX() const;
		float getY() const;
		float getDepth() const;
		std::array<float, 2> getCenter() const;
		float getHalfWidth() const;
		float getHalfHeight() const;
		std::array<float, 2> getHalfSize() const;
		Colour getColour() const;
		UV getUV() const;
		GLBlendMode getBlendMode() const;
		AABB<float> getCullSurface() const;

		std::array<TriangleGlyph, 2> getRenderGlyphs();
		void sendRenderInformation(RenderingEngine* engine);
		void sendRenderInformation(VBOBatcher* batcher);
	protected:
		std::array<float, 2> center;
		float depth;
		std::array<float, 2> halfSize;

		AABB<float> cullAABB;
		bool bHasCullSurface;

		TextureResource texture;
		GLBlendMode blendMode;
		UV uvBounds;
		std::array<TriangleGlyph, 2> Glyph;
		Colour renderColour;

		bool needsReconstructed;
		void recalculateVertexes();
	};
}