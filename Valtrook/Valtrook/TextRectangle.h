#pragma once

#include "Rectangle.h"
#include "TextResource.h"

namespace Val {
	class RenderingEngine;
	class VBOBatcher;

	class TextRectangle {
	public:
		TextRectangle(const TextResource& texture);
		TextRectangle(const TextResource& texture, float x, float y, float depth, float halfWidth, float halfHeight, float rotation);
		TextRectangle(const TextResource& texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize, float rotation);
		TextRectangle(const TextResource& texture, float x, float y, float depth, float halfWidth, float halfHeight, float rotation, Colour colour, GLBlendMode* blendMode = &GLBlendMode::Blend_Default);
		TextRectangle(const TextResource& texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize, float rotation, Colour colour, GLBlendMode* blendMode = &GLBlendMode::Blend_Default);
		~TextRectangle();

		void setTextResource(const TextResource& text);
		void setText(const std::string& contents);
		void setFont(FontAsset* newFont);
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
		void setBlendMode(GLBlendMode* blendMode);
		void setScaleTextToHeight(bool scaleTextToHeight);
		void setRotation(float rotation);

		TextResource getTextResource() const;
		std::string getText() const;
		FontAsset* getFont() const;
		float getX() const;
		float getY() const;
		float getDepth() const;
		std::array<float, 2> getCenter() const;
		float getHalfWidth() const;
		float getHalfHeight() const;
		std::array<float, 2> getHalfSize() const;
		Colour getColour() const;
		GLBlendMode* getBlendMode() const;
		bool doesScaleTextToHeight() const;
		float getRotation() const;

		void reconstruct();

		std::array<TriangleGlyph, 2> getRenderGlyphs();
		void sendRenderInformation(RenderingEngine* engine);
		void sendRenderInformation(VBOBatcher* batcher);
	protected:
		std::array<float, 2> center;
		float depth;
		std::array<float, 2> halfSize;
		float rotation;

		TextResource textResource;
		GLBlendMode* blendMode;
		std::array<TriangleGlyph, 2> Glyph;
		Colour renderColour;
		bool scaleTextToHeight;

		bool needsReconstructed;
		void recalculateVertexes();
	};
}