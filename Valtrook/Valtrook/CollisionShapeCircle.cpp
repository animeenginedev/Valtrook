#include "CollisionShapeCircle.h"

#include <math.h>

namespace Val {
	CollisionShapeCircle::CollisionShapeCircle(std::array<float, 2> center, float radius) : center(center), radiusSquared(radius * radius) {
	}
	CollisionShapeCircle::CollisionShapeCircle(float x, float y, float radius) : center({ x, y }), radiusSquared(radius * radius) {
	}
	CollisionShapeCircle::~CollisionShapeCircle() {
	}
	void CollisionShapeCircle::setX(float x) {
		center[0] = x;
	}
	void CollisionShapeCircle::setY(float y) {
		center[1] = y;
	}
	void CollisionShapeCircle::setCenter(float x, float y) {
		center[0] = x;
		center[1] = y;
	}
	void CollisionShapeCircle::setCenter(std::array<float, 2> pos) {
		center = pos;
	}
	void CollisionShapeCircle::setRadius(float r) {
		radiusSquared = r * r;
	}
	float CollisionShapeCircle::getX() const {
		return center[0];
	}
	float CollisionShapeCircle::getY() const {
		return center[1];
	}
	float CollisionShapeCircle::getRadius() const {
		return sqrt(radiusSquared);
	}
	float CollisionShapeCircle::getRadiusSquared() const {
		return radiusSquared;
	}
	bool CollisionShapeCircle::isPointInside(float x, float y) const {
		return fabs((center[0] - x) * (center[0] - x)) + ((center[1] - y) * (center[1] - y)) < radiusSquared;
	}
	AABB<float> CollisionShapeCircle::getAABB() const {
		return AABB<float>(center[0], center[1], getRadius());
	}
}