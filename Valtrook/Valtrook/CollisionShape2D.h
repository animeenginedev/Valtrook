#pragma once

#include "AABB.h"

namespace Val {
	class CollisionShape2D {
	public:
		virtual bool isPointInside(float x, float y) const = 0;
		virtual AABB<float> getAABB() const = 0;
	};
}