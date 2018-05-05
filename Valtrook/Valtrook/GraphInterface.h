#pragma once

#include <vector>

namespace Val {
	template<typename Node>
	class GraphInterface {
	public:
		virtual std::vector<Node> GetNeighbours(Node &n) = 0;
		virtual Node GetNode(unsigned int Id) = 0;

		virtual std::vector<Node> GetNodeCopy() = 0;
		virtual void ResetNodeValueData() = 0;

		virtual Node& operator[] (int x) = 0;
	};
}