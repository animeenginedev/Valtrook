#pragma once

#include "TextResource.h"
#include "Glyph.h"
#include "AABB.h"
#include <array>
#include "GLBlendMode.h"
#include "Texture.h"
#include "Colour.h"
#include "RegisterToScript.h"

namespace Val {
	class RenderingEngine;
	class VBOBatcher;

	class SimpleTextRectangle : public RegisterToScript{
	public:
		SimpleTextRectangle(const TextResource& texture);
		SimpleTextRectangle(const TextResource& texture, float x, float y, float depth, float halfWidth, float halfHeight);
		SimpleTextRectangle(const TextResource& texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize);
		SimpleTextRectangle(const TextResource& texture, float x, float y, float depth, float halfWidth, float halfHeight, Colour colour, const GLBlendMode& blendMode = GLBlendMode::Blend_Default);
		SimpleTextRectangle(const TextResource& texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize, Colour colour, const GLBlendMode& blendMode = GLBlendMode::Blend_Default);
		~SimpleTextRectangle();

		void registerToScript(chaiscript::ChaiScript* script);

		void setTextResource(const TextResource& text);
		void setText(const std::string& contents);
		void setFont(FontAsset* newFont);
		void setScaleTextToHeight(bool scaleTextToHeight);
		void setX(float x);
		void setY(float y);
		void setCenter(float x, float y);
		void setCenter(std::array<float, 2> center);
		void setDepth(float depth);
		void setHalfWidth(float h_width);
		void setHalfHeight(float h_height);
		void setHalfSize(const std::array<float, 2>& halfSize);
		void setHalfSize(float h_width, float h_height);
		void setColour(const Colour& colour);
		void setUV(const UV& uv);
		void setBlendMode(const GLBlendMode& blendMode);
		//Honestly, don't ask. It's really used to GUI rendering, setting the center/height/width and the uv dependant on the culling aabb size&position
		void setCullSurface(AABB<float> cullAABB);

		TextResource getTextResource() const;
		std::string getText() const;
		FontAsset* getFont() const;
		bool doesScaleTextToHeight() const;
		float getX() const;
		float getY() const;
		std::array<float, 2> getCenter() const;
		float getDepth() const;
		float getHalfWidth() const;
		float getHalfHeight() const;
		std::array<float, 2> getHalfSize() const;
		Colour getColour() const;
		UV getUV() const;
		GLBlendMode getBlendMode() const;
		AABB<float> getCullSurface() const;

		void reconstruct();

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
		Colour colour;
		AABB<float> cullAABB;
		bool bHasCullSurface;

		TextResource textResource;
		std::array<TriangleGlyph, 2> Glyph;
		bool scaleTextToHeight;

		bool needsReconstructed;
		void recalculateVertexes();
	};
}
