#pragma once

namespace Val {
	template<typename Position>
	class GraphNode {
	public:
		GraphNode();
		GraphNode(int id, Position x, Position y, unsigned int ExtraData = 0);
		~GraphNode();

		unsigned int Id;
		Position x, y;
		unsigned int ExtraData;

		float Value, Value2;
	};

	template<typename Position>
	bool operator == (GraphNode<Position> &rhs, GraphNode<Position> &lhs) {
		return rhs.Id == lhs.Id;
	}

	template<typename Position>
	float DistanceBetween(GraphNode<Position> &rhs, GraphNode<Position> &lhs) {
		if (rhs.Id == lhs.Id)
			return 0.0f;
		return std::sqrt(((static_cast<float>(rhs.x) - static_cast<float>(lhs.x)) * (static_cast<float>(rhs.x) - static_cast<float>(lhs.x))) +
			((static_cast<float>(rhs.y) - static_cast<float>(lhs.y)) * (static_cast<float>(rhs.y) - static_cast<float>(lhs.y))));
	}

	template<unsigned int>
	unsigned int DistanceBetween(GraphNode<unsigned int> &rhs, GraphNode<unsigned int> &lhs) {
		if (rhs.Id == lhs.Id)
			return 0.0f;
		if (rhs.x == lhs.x) {
			return std::abs(static_cast<float>(rhs.y - lhs.y));
		}
		if (rhs.y == lhs.y) {
			return std::abs(static_cast<float>(rhs.x - lhs.x));
		}
		return std::sqrtf(((static_cast<float>(rhs.x) - lhs.x) * (rhs.x - lhs.x)) + ((static_cast<float>(rhs.y) - lhs.y) * (rhs.y - lhs.y)));
	}

	template<int>
	int DistanceBetween(GraphNode<int> &rhs, GraphNode<int> &lhs) {
		if (rhs.Id == lhs.Id)
			return 0.0f;
		if (rhs.x == lhs.x) {
			return std::abs(static_cast<float>(rhs.y - lhs.y));
		}
		if (rhs.y == lhs.y) {
			return std::abs(static_cast<float>(rhs.x - lhs.x));
		}
		return std::sqrtf(((rhs.x - lhs.x) * (rhs.x - lhs.x)) + ((rhs.y - lhs.y) * (rhs.y - lhs.y)));
	}

	template<typename Position>
	inline GraphNode<Position>::GraphNode() : Id(0), x(Position()), y(Position()), ExtraData(0), Value(0), Value2(0) {
	}

	template<typename Position>
	inline GraphNode<Position>::GraphNode(int id, Position x, Position y, unsigned int ExtraData) : Id(id), x(x), y(y), ExtraData(ExtraData), Value(0), Value2(0) {
	}

	template<typename Position>
	inline GraphNode<Position>::~GraphNode() {
	}
}