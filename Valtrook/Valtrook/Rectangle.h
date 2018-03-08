#pragma once

#include "Glyph.h"
#include "Colour.h"
#include "Vertex.h"
#include "TextureAsset.h"
#include "UV.h"
#include "GLBlendMode.h"

#include <type_traits>
#include <array>

namespace Val {
	class RenderingEngine;
	class VBOBatcher;

	class SimpleRectangle {
	public:
		SimpleRectangle();
		~SimpleRectangle();

		virtual void initialise(const TextureResource& texture, const float& x, const float& y, const float& depth, const float& halfWidth, const float& halfHeight, const Colour& colour = Colour(255, 255, 255, 255), const UV& uv = UV(0.0f, 0.0f, 1.0f, 1.0f), const GLBlendMode& blendMode = GLBlendMode::Blend_Default);

		void setTexture(const TextureResource& texture);
		void setX(const float& x);
		void setY(const float& y);
		void setDepth(const float& depth);
		void setCenter(const std::array<float, 2>& center);
		void setCenter(const float &x, const float &y);
		void setHalfWidth(const float& h_width);
		void setHalfHeight(const float& h_height);
		void setHalfSize(const std::array<float, 2>& halfSize);
		void setHalfSize(const float& h_width, const float& h_height);
		void setWidth(float width);
		void setHeight(float height);
		void setSize(float width, float height);
		void setColour(const Colour& colour);
		void setUV(const UV& uv);
		void setBlendMode(const GLBlendMode& blendMode);

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

		std::array<TriangleGlyph, 2> getRenderGlyphs();
		void sendRenderInformation(RenderingEngine* engine);
		void sendRenderInformation(VBOBatcher* batcher);
	protected:
		std::array<float, 2> center;
		float depth;
		std::array<float, 2> halfSize;

		TextureResource texture;
		GLBlendMode blendMode;
		UV uvBounds;
		std::array<TriangleGlyph, 2> Glyph;
		Colour renderColour;

		bool needsReconstructed;
		virtual void recalculateVertexes();
	};

	class Rectangle : public SimpleRectangle {
	public:
		void initialise(const TextureResource& texture, const float& x, const float& y, const float& depth, const float& halfWidth, const float& halfHeight, const float& rotation, const Colour& colour = Colour(255, 255, 255, 255), const UV& uv = UV(0.0f, 0.0f, 1.0f, 1.0f), const GLBlendMode& blendMode = GLBlendMode::Blend_Default);
		
		void setRotation(const float& rotation);

		float getRotation() const;
	protected:
		float rotation;
		
		void initialise(const TextureResource& texture, const float& x, const float& y, const float& depth, const float& halfWidth, const float& halfHeight, const Colour& colour = Colour(255, 255, 255, 255), const UV& uv = UV(0.0f, 0.0f, 1.0f, 1.0f), const GLBlendMode& blendMode = GLBlendMode::Blend_Default) override;

		void recalculateVertexes() override;
	};
}