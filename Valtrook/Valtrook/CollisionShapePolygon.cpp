#include "CollisionShapePolygon.h"

#include "Max.h"
#include "Min.h"

namespace Val {
	CollisionShapePolygon::CollisionShapePolygon(std::array<float, 2> center, std::vector<std::array<float, 2>> verts) : center(center), verts(verts), calculatedAABB(0.0f, 0.0f, 0.0f), recalculateAABB(true) {
	}
	CollisionShapePolygon::CollisionShapePolygon(AABB<float> aabb) : center({ aabb.centerX, aabb.centerY }), 
		calculatedAABB(0.0f, 0.0f, 0.0f), recalculateAABB(true) {
		verts.resize(4);
		verts[0] = { aabb.halfWidth, aabb.halfHeight };
		verts[1] = { -aabb.halfWidth, aabb.halfHeight };
		verts[2] = { -aabb.halfWidth, -aabb.halfHeight };
		verts[3] = { aabb.halfWidth, -aabb.halfHeight };
	}
	CollisionShapePolygon::~CollisionShapePolygon() {
	}
	void CollisionShapePolygon::setX(float x) {
		center[0] = x;
	}
	void CollisionShapePolygon::setY(float y) {
		center[1] = y;
	}
	void CollisionShapePolygon::setCenter(float x, float y) {
		center[0] = x;
		center[1] = y;
	}
	void CollisionShapePolygon::setCenter(std::array<float, 2> center) {
		this->center = center;
	}
	float CollisionShapePolygon::getX() const {
		return center[0];
	}
	float CollisionShapePolygon::getY() const {
		return center[1];
	}
	std::array<float, 2> CollisionShapePolygon::getCenter() const {
		return center;
	}
	void CollisionShapePolygon::setVertex(std::array<float, 2> vertexPos, unsigned int vertNum) {
		if (verts.size() < vertNum) {
			verts[vertNum] = vertexPos;
			recalculateAABB = true;
		} else if (verts.size() == vertNum) {
			addVertex(vertexPos);
			recalculateAABB = true;
		}
	}
	void CollisionShapePolygon::addVertex(std::array<float, 2> vertexPos) {
		verts.push_back(vertexPos);
		recalculateAABB = true;
	}
	std::array<float, 2> CollisionShapePolygon::getVertex(unsigned int vert) const {
		return verts[verts.size() < vert ? verts.size() - 1 : vert];
	}
	void CollisionShapePolygon::removeVertex(unsigned int vertNum) {
		if (verts.size() - 1 < vertNum) {
			verts.erase(verts.begin() + vertNum);
			recalculateAABB = true;
		} else if(verts.size() - 1 == vertNum) {
			recalculateAABB = true;
			verts.pop_back();
		}
	}
	void CollisionShapePolygon::clearVertexes() {
		verts.clear();
		recalculateAABB = true;
	}
	bool CollisionShapePolygon::isPointInside(float x, float y) const {
		int i, j, nvert = verts.size();
		bool c = false;

		for (i = 0, j = nvert - 1; i < nvert; j = i++) {
			if (((verts[i][1] >= y) != (verts[j][1] >= y)) &&
				(x <= (verts[j][0] - verts[i][0]) * (y - verts[i][1]) / (verts[j][1] - verts[i][1]) + verts[i][0])
				)
				c = !c;
		}

		return c;
	}
	AABB<float> CollisionShapePolygon::getAABB() const {
		if (verts.size() == 0)
			return AABB<float>(0, 0, 0.0f);
		if (recalculateAABB) {
			std::array<float, 4> minMaxes = { verts[0][0], verts[0][1], verts[0][0], verts[0][1] };

			for (const auto& vert : verts) {
				minMaxes[0] = Max<float>(vert[0], minMaxes[0]);
				minMaxes[1] = Max<float>(vert[1], minMaxes[1]);
				minMaxes[2] = Min<float>(vert[0], minMaxes[2]);
				minMaxes[3] = Min<float>(vert[1], minMaxes[3]);
			}
			calculatedAABB.halfWidth = (minMaxes[0] - minMaxes[2]) / 2.0f;
			calculatedAABB.halfHeight = (minMaxes[1] - minMaxes[3]) / 2.0f;
			recalculateAABB = false;
		}

		calculatedAABB.centerX = center[0];
		calculatedAABB.centerY = center[1];
		return calculatedAABB;
	}
}