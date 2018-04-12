#pragma once

#include "CollisionShape2D.h"
#include <array>

namespace Val {
	class CollisionShapeCircle : public CollisionShape2D {
	public:
		CollisionShapeCircle(std::array<float, 2> center, float radius);
		CollisionShapeCircle(float x, float y, float radius);
		~CollisionShapeCircle();

		void setX(float x);
		void setY(float y);
		void setCenter(float x, float y);
		void setCenter(std::array<float, 2> pos);
		void setRadius(float r);

		float getX() const;
		float getY() const;
		float getRadius() const;
		float getRadiusSquared() const;

		bool isPointInside(float x, float y) const override;
		AABB<float> getAABB() const override;

	protected:
		std::array<float, 2> center;
		float radiusSquared;
	};
}