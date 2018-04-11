#pragma once
//   _   Render the edges and internal of a frame, for scaling ui without textures dyin
//  |_|

#include "TextureAsset.h"
#include "Glyph.h"
#include "Colour.h"
#include "GLBlendMode.h"

namespace Val {
	class RenderingEngine;
	class VBOBatcher;

	struct FrameStyle {
	public:
		FrameStyle(const TextureResource& edgeSide, const TextureResource& edgeTop, const TextureResource& corner, const TextureResource& center) : edgeSide(edgeSide), edgeTop(edgeTop), corner(corner), center(center) {}

		TextureResource edgeSide;
		TextureResource edgeTop;
		TextureResource corner;
		TextureResource center;
		
		static FrameStyle getDefault() {
			return FrameStyle(TextureAsset::getTexture("gui/frameEdgeSide", ".png", false, GL_REPEAT), 
							  TextureAsset::getTexture("gui/frameEdgeTop", ".png", false, GL_REPEAT), 
							  TextureAsset::getTexture("gui/frameCorner", ".png", false, GL_REPEAT),
							  TextureAsset::getTexture("gui/frameCenter", ".png", false, GL_REPEAT));
		}
		static FrameStyle getDefaultCurved() {
			return FrameStyle(TextureAsset::getTexture("gui/frameEdgeSide", ".png", false, GL_REPEAT),
							  TextureAsset::getTexture("gui/frameEdgeTop", ".png", false, GL_REPEAT),
							  TextureAsset::getTexture("gui/frameCornerCurve", ".png", false, GL_REPEAT),
							  TextureAsset::getTexture("gui/frameCenter", ".png", false, GL_REPEAT));
		}
	};

	class FrameRender {
	public:
		FrameRender(const FrameStyle& frameStyle, const std::array<float, 2>& center, const std::array<float, 2>& halfSize, float edgeWidth, Colour edgeColour = Colour(100, 100, 100, 255), Colour centerColour = Colour(50, 50, 50, 255));
		~FrameRender();

		void setFrameStyle(const FrameStyle& frameStyle);
		void setX(float x);
		void setY(float y);
		void setEdgeDepth(float edgeDepth);
		void setCenterDepth(float centerDepth);
		void setCenter(const std::array<float, 2>& center);
		void setCenter(float x, float y);
		void setHalfWidth(float h_width);
		void setHalfHeight(float h_height);
		void setHalfSize(const std::array<float, 2>& halfSize);
		void setHalfSize(float h_width, float h_height);
		void setWidth(float width);
		void setHeight(float height);
		void setEdgeBlendMode(const GLBlendMode& blendMode);
		void setCenterBlendMode(const GLBlendMode& blendMode);
		void setCullSurface(AABB<float> cullAABB);
		void setEdgeWidth(float edgeWidth);
		void setEdgeColour(Colour edgeColour);
		void setCenterColour(Colour centerColour);

		FrameStyle getFrameStyle() const;
		float getX() const;
		float getY() const;
		float getEdgeDepth() const;
		float getCenterDepth() const;
		std::array<float, 2> getCenter() const;
		float getHalfWidth() const;
		float getHalfHeight() const;
		std::array<float, 2> getHalfSize() const;
		Colour getEdgeColour() const;
		Colour getCenterColour() const;
		GLBlendMode getEdgeBlendMode() const;
		GLBlendMode getCenterBlendMode() const;
		AABB<float> getCullSurface() const;
		float getEdgeWidth() const;

		std::array<TriangleGlyph, 18> getRenderGlyphs();
		void sendRenderInformation(RenderingEngine* engine);
		void sendRenderInformation(VBOBatcher* batcher);
	protected:
		std::array<float, 2> center;
		float edgeDepth, centerDepth;
		std::array<float, 2> halfSize;
		float edgeWidth;

		AABB<float> cullAABB;
		bool bHasCullSurface;

		FrameStyle frameStyle;
		GLBlendMode edgeBlendMode, centerBlendMode;

		//Corners, Sides, Center
		std::array<TriangleGlyph, 18> Glyph;
		Colour edgeColour, centerColour;

		bool needsReconstructed;
		void recalculateVertexes();
	};
}