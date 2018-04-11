#include "FrameRender.h"

#include "RenderingEngine.h"
#include "VBOBatcher.h"
#include "Engine.h"
#include "Logger.h"
#include "ShapeCuller.h"
namespace Val {
	FrameRender::FrameRender(const FrameStyle& frameStyle, const std::array<float, 2>& center, const std::array<float, 2>& halfSize, float edgeWidth, Colour edgeColour, Colour centerColour)
		: cullAABB(0.0f, 0.0f, 0.0f), bHasCullSurface(false), frameStyle(frameStyle), center(center), halfSize(halfSize), edgeWidth(edgeWidth), edgeColour(edgeColour), centerColour(centerColour), needsReconstructed(true)
	{
	}
	FrameRender::~FrameRender() {
	}
	void FrameRender::setFrameStyle(const FrameStyle & frameStyle) {
		this->frameStyle = frameStyle;
		needsReconstructed = true;
	}
	void FrameRender::setX(float x) {
		center[0] = x;
		needsReconstructed = true;
	}
	void FrameRender::setY(float y) {
		center[1] = y;
		needsReconstructed = true;
	}
	void FrameRender::setEdgeDepth(float edgeDepth) {
		this->edgeDepth = edgeDepth;
		needsReconstructed = true;
	}
	void FrameRender::setCenterDepth(float centerDepth) {
		this->centerDepth = centerDepth;
		needsReconstructed = true;
	}
	void FrameRender::setCenter(const std::array<float, 2>& center) {
		this->center = center;
		needsReconstructed = true;
	}
	void FrameRender::setCenter(float x, float y) {
		center[0] = x;
		center[1] = y;
		needsReconstructed = true;
	}
	void FrameRender::setHalfWidth(float h_width) {
		halfSize[0] = h_width;
		needsReconstructed = true;
	}
	void FrameRender::setHalfHeight(float h_height) {
		halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void FrameRender::setHalfSize(const std::array<float, 2>& halfSize) {
		this->halfSize = halfSize;
		needsReconstructed = true;
	}
	void FrameRender::setHalfSize(float h_width, float h_height) {
		halfSize[0] = h_width;
		halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void FrameRender::setWidth(float width) {
		halfSize[0] = width / 1.0f;
		needsReconstructed = true;
	}
	void FrameRender::setHeight(float height) {
		halfSize[1] = height / 2.0f;
		needsReconstructed = true;
	}
	void FrameRender::setEdgeBlendMode(const GLBlendMode & blendMode) {
		edgeBlendMode = blendMode;
		needsReconstructed = true;
	}
	void FrameRender::setCenterBlendMode(const GLBlendMode & blendMode) {
		centerBlendMode = blendMode;
		needsReconstructed = true;
	}
	void FrameRender::setCullSurface(AABB<float> cullAABB) {
		static AABB<float> noCullSurface = { 0.0f, 0.0f, 0.0f, 0.0f };
		this->cullAABB = cullAABB;
		bHasCullSurface = !(cullAABB == noCullSurface);
		needsReconstructed = true;
	}
	void FrameRender::setEdgeWidth(float edgeWidth) {
		this->edgeWidth = edgeWidth;
		needsReconstructed = true;
	}
	void FrameRender::setEdgeColour(Colour edgeColour) {
		this->edgeColour = edgeColour;
		needsReconstructed = true;
	}
	void FrameRender::setCenterColour(Colour centerColour) {
		this->centerColour = centerColour;
		needsReconstructed = true;
	}
	FrameStyle FrameRender::getFrameStyle() const {
		return frameStyle;
	}
	float FrameRender::getX() const {
		return center[0];
	}
	float FrameRender::getY() const {
		return center[1];
	}
	float FrameRender::getEdgeDepth() const {
		return edgeDepth;
	}
	float FrameRender::getCenterDepth() const {
		return centerDepth;
	}
	std::array<float, 2> FrameRender::getCenter() const {
		return center;
	}
	float FrameRender::getHalfWidth() const {
		return halfSize[0];
	}
	float FrameRender::getHalfHeight() const {
		return halfSize[1];
	}
	std::array<float, 2> FrameRender::getHalfSize() const {
		return halfSize;
	}
	Colour FrameRender::getEdgeColour() const {
		return edgeColour;
	}
	Colour FrameRender::getCenterColour() const {
		return centerColour;
	}
	GLBlendMode FrameRender::getEdgeBlendMode() const {
		return edgeBlendMode;
	}
	GLBlendMode FrameRender::getCenterBlendMode() const {
		return centerBlendMode;
	}
	AABB<float> FrameRender::getCullSurface() const {
		return cullAABB;
	}
	float FrameRender::getEdgeWidth() const {
		return edgeWidth;
	}
	std::array<TriangleGlyph, 18> FrameRender::getRenderGlyphs() {
		if (needsReconstructed)
			recalculateVertexes();
		return Glyph;
	}
	void FrameRender::sendRenderInformation(RenderingEngine * engine) {
		if (needsReconstructed)
			recalculateVertexes();
		engine->getRenderer()->add(Glyph);
	}
	void FrameRender::sendRenderInformation(VBOBatcher * batcher) {
		if (needsReconstructed)
			recalculateVertexes();
		batcher->add(Glyph);
	}
	void FrameRender::recalculateVertexes() {
		needsReconstructed = false;
		
		bool invalidTexture = false;
		if (frameStyle.center.getTexture() == nullptr) {
			frameStyle.center = Texture::errorTexture;
			invalidTexture = true;
		}
		if (frameStyle.edgeSide.getTexture() == nullptr) {
			frameStyle.edgeSide = Texture::errorTexture;
			invalidTexture = true;
		}
		if (frameStyle.edgeTop.getTexture() == nullptr) {
			frameStyle.edgeTop = Texture::errorTexture;
			invalidTexture = true;
		}
		if (frameStyle.corner.getTexture() == nullptr) {
			frameStyle.corner = Texture::errorTexture;
			invalidTexture = true;
		}
		if (invalidTexture) {
			Logger::Instance->logMessage(WARNING, "Invalid Texture on Frame Render");
		}
		
		std::array<float, 2> centerUPixel = { WorldToUnalignedPixel<float>(center[0]), WorldToUnalignedPixel<float>(center[1]) };
		std::array<float, 2> halfSizeUPixel = { WorldToUnalignedPixel<float>(halfSize[0]), WorldToUnalignedPixel<float>(halfSize[1]) };
		float edgeWidthUPixel = WorldToUnalignedPixel<float>(edgeWidth);

		std::array<float, 2> topLeft = { centerUPixel[0] - halfSizeUPixel[0], centerUPixel[1] + halfSizeUPixel[1] };
		std::array<float, 2> topRight = { centerUPixel[0] + halfSizeUPixel[0], centerUPixel[1] + halfSizeUPixel[1] };
		std::array<float, 2> bottomLeft = { centerUPixel[0] - halfSizeUPixel[0], centerUPixel[1] - halfSizeUPixel[1] };
		std::array<float, 2> bottomRight = { centerUPixel[0] + halfSizeUPixel[0], centerUPixel[1] - halfSizeUPixel[1] };
		//top left 4x then top right 4x bot left, bot right
		std::array<std::array<float, 2>, 16> vertexPositions = {
			topLeft[0], topLeft[1],
			topLeft[0] + edgeWidthUPixel, topLeft[1],
			topLeft[0], topLeft[1] - edgeWidthUPixel,
			topLeft[0] + edgeWidthUPixel, topLeft[1] - edgeWidthUPixel,

			topRight[0] - edgeWidthUPixel, topRight[1],
			topRight[0], topRight[1],
			topRight[0] - edgeWidthUPixel, topRight[1] - edgeWidthUPixel,
			topRight[0], topRight[1] - edgeWidthUPixel,

			bottomLeft[0], bottomLeft[1] + edgeWidthUPixel,
			bottomLeft[0] + edgeWidthUPixel, bottomLeft[1] + edgeWidthUPixel,
			bottomLeft[0], bottomLeft[1],
			bottomLeft[0] + edgeWidthUPixel, bottomLeft[1],

			bottomRight[0] - edgeWidthUPixel, bottomRight[1] + edgeWidthUPixel,
			bottomRight[0], bottomRight[1] + edgeWidthUPixel,
			bottomRight[0] - edgeWidthUPixel, bottomRight[1],
			bottomRight[0], bottomRight[1],
		};

		auto cornerUV = frameStyle.corner.getBounds();
		auto TopLeftRect = RectangleGlyph(frameStyle.corner.getGLTexture()->getTextureID(), std::array<Vertex, 4>({
			Vertex(vertexPositions[0][0], vertexPositions[0][1], edgeDepth, cornerUV.u, cornerUV.v, edgeColour),
			Vertex(vertexPositions[1][0], vertexPositions[1][1], edgeDepth, cornerUV.u + cornerUV.uWidth, cornerUV.v, edgeColour),
			Vertex(vertexPositions[2][0], vertexPositions[2][1], edgeDepth, cornerUV.u, cornerUV.v + cornerUV.vHeight, edgeColour),
			Vertex(vertexPositions[3][0], vertexPositions[3][1], edgeDepth, cornerUV.u + cornerUV.uWidth, cornerUV.v + cornerUV.vHeight, edgeColour)
		}), &edgeBlendMode);
		auto TopLeftGlyphs = bHasCullSurface ? ShapeCuller::cullRectangle(TopLeftRect, cullAABB).dispose() : TopLeftRect.dispose();

		auto TopRightRect = RectangleGlyph(frameStyle.corner.getGLTexture()->getTextureID(), std::array<Vertex, 4>({
			Vertex(vertexPositions[4][0], vertexPositions[4][1], edgeDepth,cornerUV.u + cornerUV.uWidth, cornerUV.v, edgeColour),
			Vertex(vertexPositions[5][0], vertexPositions[5][1], edgeDepth, cornerUV.u, cornerUV.v, edgeColour),
			Vertex(vertexPositions[6][0], vertexPositions[6][1], edgeDepth,cornerUV.u + cornerUV.uWidth, cornerUV.v + cornerUV.vHeight, edgeColour),
			Vertex(vertexPositions[7][0], vertexPositions[7][1], edgeDepth, cornerUV.u, cornerUV.v + cornerUV.vHeight, edgeColour)
		}), &edgeBlendMode);
		auto TopRightGlyphs = bHasCullSurface ? ShapeCuller::cullRectangle(TopRightRect, cullAABB).dispose() : TopRightRect.dispose();

		auto BottomLeftRect = RectangleGlyph(frameStyle.corner.getGLTexture()->getTextureID(), std::array<Vertex, 4>({
			Vertex(vertexPositions[8][0], vertexPositions[8][1], edgeDepth, cornerUV.u, cornerUV.v + cornerUV.vHeight, edgeColour),
			Vertex(vertexPositions[9][0], vertexPositions[9][1], edgeDepth, cornerUV.u + cornerUV.uWidth, cornerUV.v + cornerUV.vHeight, edgeColour),
			Vertex(vertexPositions[10][0], vertexPositions[10][1], edgeDepth, cornerUV.u, cornerUV.v, edgeColour),
			Vertex(vertexPositions[11][0], vertexPositions[11][1], edgeDepth, cornerUV.u + cornerUV.uWidth, cornerUV.v, edgeColour)
		}), &edgeBlendMode);
		auto BottomLeftGlyphs = bHasCullSurface ? ShapeCuller::cullRectangle(BottomLeftRect, cullAABB).dispose() : BottomLeftRect.dispose();

		auto BottomRightRect = RectangleGlyph(frameStyle.corner.getGLTexture()->getTextureID(), std::array<Vertex, 4>({
			Vertex(vertexPositions[12][0], vertexPositions[12][1], edgeDepth, cornerUV.u + cornerUV.uWidth, cornerUV.v + cornerUV.vHeight, edgeColour),
			Vertex(vertexPositions[13][0], vertexPositions[13][1], edgeDepth, cornerUV.u, cornerUV.v + cornerUV.vHeight, edgeColour),
			Vertex(vertexPositions[14][0], vertexPositions[14][1], edgeDepth, cornerUV.u + cornerUV.uWidth, cornerUV.v, edgeColour),
			Vertex(vertexPositions[15][0], vertexPositions[15][1], edgeDepth, cornerUV.u, cornerUV.v, edgeColour)
		}), &edgeBlendMode);
		auto BottomRightGlyphs = bHasCullSurface ? ShapeCuller::cullRectangle(BottomRightRect, cullAABB).dispose() : BottomRightRect.dispose();


		float lengthDiff = fabs((topLeft[0] - topRight[0]) / edgeWidthUPixel);

		auto TopEdgeRect = RectangleGlyph(frameStyle.edgeTop.getGLTexture()->getTextureID(), std::array<Vertex, 4>({
			Vertex(vertexPositions[1][0], vertexPositions[1][1], edgeDepth, 0.0f, 0.0, edgeColour),
			Vertex(vertexPositions[4][0], vertexPositions[4][1], edgeDepth, lengthDiff, 0.0f, edgeColour),
			Vertex(vertexPositions[3][0], vertexPositions[3][1], edgeDepth, 0.0f, 1.0f, edgeColour),
			Vertex(vertexPositions[6][0], vertexPositions[6][1], edgeDepth, lengthDiff , 1.0f, edgeColour)
		}), &edgeBlendMode);
		auto TopEdgeGlyphs = bHasCullSurface ? ShapeCuller::cullRectangle(TopEdgeRect, cullAABB).dispose() : TopEdgeRect.dispose();

		auto BottomEdgeRect = RectangleGlyph(frameStyle.edgeTop.getGLTexture()->getTextureID(), std::array<Vertex, 4>({
			Vertex(vertexPositions[9][0], vertexPositions[9][1], edgeDepth,  0.0f, 1.0f, edgeColour),
			Vertex(vertexPositions[12][0], vertexPositions[12][1], edgeDepth,lengthDiff, 1.0f, edgeColour),
			Vertex(vertexPositions[11][0], vertexPositions[11][1], edgeDepth,0.0f, 0.0f, edgeColour),
			Vertex(vertexPositions[14][0], vertexPositions[14][1], edgeDepth,lengthDiff, 0.0f, edgeColour)
		}), &edgeBlendMode);
		auto BottomEdgeGlyphs = bHasCullSurface ? ShapeCuller::cullRectangle(BottomEdgeRect, cullAABB).dispose() : BottomEdgeRect.dispose();
		//Height LengthDiff may be different.
		lengthDiff = fabs((topLeft[1] - bottomLeft[1]) / edgeWidthUPixel);
		auto LeftEdgeRect = RectangleGlyph(frameStyle.edgeSide.getGLTexture()->getTextureID(), std::array<Vertex, 4>({
			Vertex(vertexPositions[2][0], vertexPositions[2][1], edgeDepth, 0.0f, 0.0f, edgeColour),
			Vertex(vertexPositions[3][0], vertexPositions[3][1], edgeDepth, 1.0f, 0.0f, edgeColour),
			Vertex(vertexPositions[8][0], vertexPositions[8][1], edgeDepth, 0.0f, lengthDiff, edgeColour),
			Vertex(vertexPositions[9][0], vertexPositions[9][1], edgeDepth, 1.0f, lengthDiff, edgeColour)
		}), &edgeBlendMode);
		auto LeftEdgeGlyphs = bHasCullSurface ? ShapeCuller::cullRectangle(LeftEdgeRect, cullAABB).dispose() : LeftEdgeRect.dispose();

		auto RightEdgeRect = RectangleGlyph(frameStyle.edgeSide.getGLTexture()->getTextureID(), std::array<Vertex, 4>({
			Vertex(vertexPositions[6][0], vertexPositions[6][1], edgeDepth, 1.0f, 0.0f, edgeColour),
			Vertex(vertexPositions[7][0], vertexPositions[7][1], edgeDepth, 0.0f, 0.0f, edgeColour),
			Vertex(vertexPositions[12][0], vertexPositions[12][1], edgeDepth, 1.0f, lengthDiff, edgeColour),
			Vertex(vertexPositions[13][0], vertexPositions[13][1], edgeDepth, 0.0f,lengthDiff, edgeColour)
		}), &edgeBlendMode);
		auto RightEdgeGlyphs = bHasCullSurface ? ShapeCuller::cullRectangle(RightEdgeRect, cullAABB).dispose() : RightEdgeRect.dispose();

		auto centerUV = frameStyle.center.getBounds();
		auto correctionAmount = edgeWidthUPixel / 8.0f;
		auto CenterRect = RectangleGlyph(frameStyle.center.getGLTexture()->getTextureID(), std::array<Vertex, 4>({
			Vertex(topLeft[0] + correctionAmount, topLeft[1] - correctionAmount, centerDepth, centerUV.u, centerUV.v, centerColour),
			Vertex(topRight[0] - correctionAmount, topRight[1] - correctionAmount, centerDepth, centerUV.u + centerUV.uWidth, centerUV.v, centerColour),
			Vertex(bottomLeft[0] + correctionAmount, bottomLeft[1] + correctionAmount, centerDepth, centerUV.u, centerUV.v + centerUV.vHeight, centerColour),
				Vertex(bottomRight[0] - correctionAmount, bottomRight[1] + correctionAmount, centerDepth, centerUV.u + centerUV.uWidth, centerUV.v + centerUV.vHeight, centerColour)
		}), &centerBlendMode);
		auto CenterGlyphs = bHasCullSurface ? ShapeCuller::cullRectangle(CenterRect, cullAABB).dispose() : CenterRect.dispose();

		Glyph[0] = TopLeftGlyphs[0];
		Glyph[1] = TopLeftGlyphs[1];
		Glyph[2] = TopRightGlyphs[0];
		Glyph[3] = TopRightGlyphs[1];
		Glyph[4] = BottomLeftGlyphs[0];
		Glyph[5] = BottomLeftGlyphs[1];
		Glyph[6] = BottomRightGlyphs[0];
		Glyph[7] = BottomRightGlyphs[1];
		Glyph[8] = TopEdgeGlyphs[0];
		Glyph[9] = TopEdgeGlyphs[1];
		Glyph[10] = BottomEdgeGlyphs[0];
		Glyph[11] = BottomEdgeGlyphs[1];
		Glyph[12] = LeftEdgeGlyphs[0];
		Glyph[13] = LeftEdgeGlyphs[1];
		Glyph[14] = RightEdgeGlyphs[0];
		Glyph[15] = RightEdgeGlyphs[1];
		Glyph[16] = CenterGlyphs[0];
		Glyph[17] = CenterGlyphs[1];
	}
}