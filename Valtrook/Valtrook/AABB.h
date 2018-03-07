#pragma once

#include <type_traits>

namespace Val {
	template<typename Real,
		typename = std::enable_if_t<std::is_arithmetic<Real>::value>>
	class AABB {
	public:
		AABB(Real x, Real y, Real halfDimension) : centerX(x), centerY(y), halfWidth(halfDimension), halfHeight(halfDimension) {}
		AABB(Real x, Real y, Real halfWidth, Real halfHeight) : centerX(x), centerY(y), halfWidth(halfWidth), halfHeight(halfHeight) {}
		~AABB() {}

		Real getX() {
			return centerX;
		}
		Real getY() {
			return centerY;
		}
		Real getHalfWidth() {
			return halfWidth;
		}
		Real getHalfHeight() {
			return halfHeight;
		}

		void setCenter(Real x, Real y) {
			centerX = x;
			centerY = y;
		}

		bool containsPoint(Real x, Real y) {
			return !(x > centerX + halfWidth ||
					 x < centerX - halfWidth ||
					 y < centerY - halfHeight ||
					 y > centerY + halfHeight);
		}
		bool intersectsAABB(AABB rhs) {
			return !(rhs.centerX - rhs.halfWidth > centerX + halfWidth ||
					 rhs.centerX + rhs.halfWidth < centerX - halfWidth ||
					 rhs.centerY - rhs.halfHeight > centerY + halfHeight ||
					 rhs.centerY + rhs.halfHeight < centerY - halfHeight);
		}

		
		Real centerX, centerY, halfWidth, halfHeight;
	};
}
