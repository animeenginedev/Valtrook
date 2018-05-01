#pragma once

#include "Glyph.h"
#include "AABB.h"
#include "Max.h"
#include "Min.h"
#include "Conversion.h"

namespace Val {
	class GlyphCuller {
	public:
		static std::vector<LineGlyph> cullLines(const std::vector<LineGlyph> lines, const AABB<float>& culler) {
			std::vector<LineGlyph> linesOut = std::vector<LineGlyph>();
			linesOut.reserve(lines.size());

			for (auto line : lines) {
				line = cullLine(line, culler);
				//Ignore completly culled lines which should have a length of 0 || the exact same x & y positions
				if (line.vertexes[0].x != line.vertexes[1].x || line.vertexes[0].y != line.vertexes[1].y)
					linesOut.push_back(line);
			}

			return linesOut;
		}

		static LineGlyph cullLine(const LineGlyph& line, const AABB<float>& culler) {
			Vertex a = line.vertexes[0];
			Vertex b = line.vertexes[1];
			
			if (!culler.containsPoint(a.x, a.y)) {
				a.x = Max<float>(Min<float>(culler.maxX(), a.x), culler.minX());
				a.y = Max<float>(Min<float>(culler.maxY(), a.y), culler.minY());
			}
			if (!culler.containsPoint(b.x, b.y)) {
				b.x = Max<float>(Min<float>(culler.maxX(), b.x), culler.minX());
				b.y = Max<float>(Min<float>(culler.maxY(), b.y), culler.minY());
			}
			
			return LineGlyph(line.textureId, { a, b }, line.blendMode);
		}

		static RectangleGlyph cullRectangle(const RectangleGlyph& rect, const AABB<float>& culler) {
			Vertex TL = rect.vertexes[0];
			Vertex TR = rect.vertexes[1];
			Vertex BL = rect.vertexes[2];
			Vertex BR = rect.vertexes[4];

			float rectHalfWidth = fabs((TL.x - TR.x) / 2.0f);
			float rectHalfHeight = fabs((TL.y - BL.y) / 2.0f);

			//We have to convert it back to world since the culler is in world coords
			AABB<float> rectAABB = AABB<float>(UnalignedPixelToWorld<float, float>(TL.x + rectHalfWidth),
											   UnalignedPixelToWorld<float, float>(TL.y - rectHalfHeight),
											   UnalignedPixelToWorld<float, float>(rectHalfWidth),
											   UnalignedPixelToWorld<float, float>(rectHalfHeight));

			//Completly inside, no changes needed
			if (rectAABB.isCompletlyInside(culler))
				return rect;

			//Completly Culled
			if (!rectAABB.intersectsAABB(culler)) {
				return RectangleGlyph(0, std::array<Vertex, 4>(), &GLBlendMode::Blend_Default);
			}

			//Partial Culling aka HELL
			auto resultAABB = rectAABB.getCulledAABB(culler);

			std::array<float, 2> culledMin = { WorldToUnalignedPixel<float>(resultAABB.minX()), WorldToUnalignedPixel<float>(resultAABB.minY()) };
			std::array<float, 2> culledMax = { WorldToUnalignedPixel<float>(resultAABB.maxX()), WorldToUnalignedPixel<float>(resultAABB.maxY()) };
			
			float uWidth = fabs(TR.u - TL.u);
			float vHeight = fabs(BL.v - TL.v);

			bool uLow = TL.u < TR.u;
			bool vLow = TL.v < BL.v;

			TL.u = TL.u + ((uLow ? -1.0f : 1.0f) * (uWidth * ((TL.x - culledMin[0]) / (rectHalfWidth + rectHalfWidth))));
			TL.v = TL.v + ((vLow ? -1.0f : 1.0f) * (vHeight * ((culledMax[1] - TL.y) / (rectHalfHeight + rectHalfHeight))));
			TL.x = culledMin[0];
			TL.y = culledMax[1];

			TR.u = TR.u + ((!uLow ? -1.0f : 1.0f) * (uWidth * ((culledMax[0]- TR.x) / (rectHalfWidth + rectHalfWidth))));
			TR.v = TR.v + ((vLow ? -1.0f : 1.0f) * (vHeight * ((culledMax[1] - TR.y) / (rectHalfHeight + rectHalfHeight))));
			TR.x = culledMax[0];
			TR.y = culledMax[1];

			BL.u = BL.u + ((uLow ? -1.0f : 1.0f) * (uWidth * ((BL.x - culledMin[0]) / (rectHalfWidth + rectHalfWidth))));
			BL.v = BL.v + ((!vLow ? -1.0f : 1.0f) * (vHeight * ((BL.y - culledMin[1]) / (rectHalfHeight + rectHalfHeight))));
			BL.x = culledMin[0];
			BL.y = culledMin[1];

			BR.u = BR.u + ((!uLow ? -1.0f : 1.0f) * (uWidth * ((culledMax[0] - BR.x) / (rectHalfWidth + rectHalfWidth))));
			BR.v = BR.v + ((!vLow ? -1.0f : 1.0f) * (vHeight * ((BR.y - culledMin[1]) / (rectHalfHeight + rectHalfHeight))));
			BR.x = culledMax[0];
			BR.y = culledMin[1];

			return RectangleGlyph(rect.textureId, std::array<Vertex, 4>({
				TL,
				TR,
				BL,
				BR
			}), rect.blendMode);
		}
	};
};