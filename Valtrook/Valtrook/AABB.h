#pragma once

#include "Min.h"
#include "Max.h"
#include <type_traits>

namespace Val {
	template<typename Real,
		typename = std::enable_if_t<std::is_arithmetic<Real>::value>>
	class AABB {
	public:
		AABB() : centerX(0x0), centerY(0x0), halfWidth(0x0), halfHeight(0x0) {};
		AABB(Real x, Real y, Real halfDimension) : centerX(x), centerY(y), halfWidth(halfDimension), halfHeight(halfDimension) {}
		AABB(Real x, Real y, Real halfWidth, Real halfHeight) : centerX(x), centerY(y), halfWidth(halfWidth), halfHeight(halfHeight) {}
		~AABB() {}

		Real getX() const {
			return centerX;
		}
		Real getY() const {
			return centerY;
		}
		Real getHalfWidth() const {
			return halfWidth;
		}
		Real getHalfHeight() const {
			return halfHeight;
		}

		Real minX() const {
			return centerX - halfWidth;
		}

		Real minY() const {
			return centerY - halfHeight;
		}

		Real maxX() const {
			return centerX + halfWidth;
		}

		Real maxY() const {
			return centerY + halfHeight;
		}

		void setCenter(Real x, Real y) {
			centerX = x;
			centerY = y;
		}

		bool containsPoint(Real x, Real y) const {
			return !(x > centerX + halfWidth ||
					 x < centerX - halfWidth ||
					 y < centerY - halfHeight ||
					 y > centerY + halfHeight);
		}
		bool intersectsAABB(const AABB<Real>& rhs) const {
			return !(rhs.centerX - rhs.halfWidth > centerX + halfWidth ||
					 rhs.centerX + rhs.halfWidth < centerX - halfWidth ||
					 rhs.centerY - rhs.halfHeight > centerY + halfHeight ||
					 rhs.centerY + rhs.halfHeight < centerY - halfHeight);
		}
		// InsideTestingAABB.isCompletlyInside(OuterAABB)
		bool isCompletlyInside(const AABB<Real>& rhs) const {
			return rhs.centerX - rhs.halfWidth  < centerX - halfWidth &&
				rhs.centerX + rhs.halfWidth > centerX + halfWidth &&
				rhs.centerY - rhs.halfHeight < centerY - halfHeight &&
				rhs.centerY + rhs.halfHeight > centerY + halfHeight;
		}

		AABB<Real> getCulledAABB(const AABB<Real>& cullingAABB) const {
			Real MinX = Max<Real>(cullingAABB.centerX - cullingAABB.halfWidth, centerX - halfWidth);
			Real MaxX = Min<Real>(cullingAABB.centerX + cullingAABB.halfWidth, centerX + halfWidth);
			Real MinY = Max<Real>(cullingAABB.centerY - cullingAABB.halfHeight, centerY - halfHeight);
			Real MaxY = Min<Real>(cullingAABB.centerY + cullingAABB.halfHeight, centerY + halfHeight);

			
		
			return AABB<Real>(MinX + ((MaxX - MinX) / static_cast<Real>(2.0)),
							  MinY + ((MaxY - MinY) / static_cast<Real>(2.0)),
							  ((MaxX - MinX) / static_cast<Real>(2.0)),
							  ((MaxY - MinY) / static_cast<Real>(2.0)));
		}

		bool operator==(const AABB<Real>& rhs) const {
			return centerX == rhs.centerX &&
				centerY == rhs.centerY &&
				halfWidth == rhs.halfWidth &&
				halfHeight == rhs.halfHeight;
		}

		bool equals(const AABB<Real>& rhs, Real epsilon = static_cast<Real>(0.01f)) const {
			return fabs(centerX - rhs.centerX) < epsilon &&
				fabs(centerY - rhs.centerY) < epsilon &&
				fabs(halfWidth - rhs.halfWidth) < epsilon &&
				fabs(halfHeight - rhs.halfHeight) < epsilon;
		}
		
		Real centerX, centerY, halfWidth, halfHeight;
	};
}
