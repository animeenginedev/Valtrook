#pragma once

#include "TextResource.h"
#include "Glyph.h"
#include "Colour.h"
#include "GLBlendMode.h"
#include "UV.h"
#include "Justification.h"
#include "AABB.h"
#include "WordString.h"

namespace Val {
	class RenderingEngine;
	class VBOBatcher;

	class SimpleMultilineTextRectangle {
	public:
		SimpleMultilineTextRectangle(const TextResource& texture, float x, float y, float depth, float maxWidthPerLine, float heightPerLine);
		SimpleMultilineTextRectangle(const TextResource& texture, std::array<float, 2> center, float depth, float maxWidthPerLine, float heightPerLine);
		SimpleMultilineTextRectangle(const TextResource& texture, float x, float y, float depth, float maxWidthPerLine, float heightPerLine, Colour colour, const GLBlendMode& blendMode = GLBlendMode::Blend_Default);
		SimpleMultilineTextRectangle(const TextResource& texture, std::array<float, 2> center, float depth, float maxWidthPerLine, float heightPerLine, Colour colour, const GLBlendMode& blendMode = GLBlendMode::Blend_Default);
		~SimpleMultilineTextRectangle();

		void setTextResource(const TextResource& text);
		void setText(const std::string& contents);
		void setFont(FontAsset* newFont);
		void setX(float x);
		void setY(float y);
		void setDepth(float depth);
		void setCenter(const std::array<float, 2>& center);
		void setCenter(float x, float y);
		void setHalfWidthPerLine(float hwpl);
		void setHalfHeightPerLine(float hhpl);
		void setHalfSizePerLine(const std::array<float, 2>& hspl);
		void setSize(float width, float height);
		void setColour(const Colour& colour);
		void setBlendMode(const GLBlendMode& blendMode);
		//Honestly, don't ask. It's really used to GUI rendering, setting the center/height/width and the uv dependant on the culling aabb size&position
		void setCullSurface(AABB<float> cullAABB);
		void setJustification(HorizontalJustification justification);

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
		float getHalfWidthPerLine() const;
		float getHalfHeightPerLine() const;
		std::array<float, 2> getHalfSizePerLine() const;
		Colour getColour() const;
		GLBlendMode getBlendMode() const;
		AABB<float> getCullSurface() const;
		HorizontalJustification getJustification() const;

		void reconstruct();

		void setSingleLineMode(bool singlieLineMode);
		bool isSingleLineMode() const;

		//number of glyphs dependant on number of lines
		std::vector<std::array<TriangleGlyph, 2>> getRenderGlyphs();
		void sendRenderInformation(RenderingEngine* engine);
		void sendRenderInformation(VBOBatcher* batcher);
	protected:
		std::array<float, 2> center;
		float depth;
		std::array<float, 2> halfSize;

		float halfHeightPerLine;
		float halfWidthPerLine;
		bool singleLineMode;
		HorizontalJustification justification;
		bool needsTextReconstructed;

		AABB<float> cullAABB;
		bool bHasCullSurface;

		TextResource textResourceMaster;
		std::vector<TextResource> textResourcePerLine;
		GLBlendMode blendMode;
		std::vector<std::array<TriangleGlyph, 2>> Glyph;
		Colour renderColour;

		bool needsReconstructed;
		void recalculateVertexes();
	};
}