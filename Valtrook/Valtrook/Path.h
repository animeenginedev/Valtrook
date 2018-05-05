#pragma once

#include <array>
#include <vector>

namespace Val {
	template<typename Node>
	class Path {
	public:
		Path(Node Start, Node End, std::vector<Node> path);
		~Path();

		Node Start, End;
		//Path Contains Start and End.
		std::vector<Node> path;
		float EuclideanDistance;

		unsigned int GetLength();
		float GetEuclideanDistance();
		//void Append(Path& Appendee);
		//Path Reverse();
	};

	template<typename Node>
	inline Path<Node>::Path(Node Start, Node End, std::vector<Node> path) : Start(Start), End(End), path(path) {
		this->EuclideanDistance = GetEuclideanDistance();
	}

	template<typename Node>
	inline Path<Node>::~Path() {
	}

	template<typename Node>
	inline unsigned int Path<Node>::GetLength() {
		return path.size();
	}

	template<typename Node>
	inline float Path<Node>::GetEuclideanDistance() {
		EuclideanDistance = 0.0f;
		if (path.size() != 0) {
			for (unsigned int i = 0; i < path.size() - 1; ++i) {
				EuclideanDistance += DistanceBetween(path[i], path[i + 1]);
			}
		}
		return EuclideanDistance;
	}
}