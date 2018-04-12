#pragma once

#include "Glyph.h"
#include "AABB.h"
#include "Max.h"
#include "Min.h"
#include "Conversion.h"

namespace Val {
	class GlyphCuller {
	public:
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