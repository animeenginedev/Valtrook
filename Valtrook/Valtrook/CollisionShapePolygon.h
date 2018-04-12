#pragma once

#include "CollisionShape2D.h"
#include <vector>
#include <array>

namespace Val {
	class CollisionShapePolygon : public CollisionShape2D {
	public:
		CollisionShapePolygon(std::array<float, 2> center, std::vector<std::array<float, 2>> verts);
		CollisionShapePolygon(AABB<float> aabb);
		~CollisionShapePolygon();

		void setX(float x);
		void setY(float y);
		void setCenter(float x, float y);
		void setCenter(std::array<float, 2> center);

		float getX() const;
		float getY() const;
		std::array<float, 2> getCenter() const;


		void setVertex(std::array<float, 2> vertexPos, unsigned int vertNum);
		template<typename... Args>
		void addVertex(const std::array<float, 2> vertexPos, Args&&... args) {
			addVertex(vertexPos);
			addVertex(args...);
		}
		void addVertex(std::array<float, 2> vertexPos);
		std::array<float, 2> getVertex(unsigned int vert) const;
		void removeVertex(unsigned int vertNum);
		void clearVertexes();

		bool isPointInside(float x, float y) const override;
		AABB<float> getAABB() const override;

	protected:
		mutable AABB<float> calculatedAABB;
		mutable bool recalculateAABB;

		std::array<float, 2> center;
		//local offset from center
		std::vector<std::array<float, 2>> verts;
	};
}