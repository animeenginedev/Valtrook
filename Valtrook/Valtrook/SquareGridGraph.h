#pragma once

#include "GraphInterface.h"
#include "GraphNode.h"

#include <array>

namespace Val {
	template<typename Node>
	class SquareGridGraph : public GraphInterface<Node> {
	public:
		SquareGridGraph();
		~SquareGridGraph();

		void Initialise(unsigned int width, unsigned int height);
		void SetExtraData(unsigned int x, unsigned int y, unsigned int Data);
		void SetExtraData(unsigned int id, unsigned int Data);
		unsigned int GetExtraData(unsigned int x, unsigned int y);
		unsigned int GetExtraData(unsigned int id);

		Node GetNode(unsigned int x, unsigned int y);
		Node GetNode(unsigned int id) override;

		std::vector<Node> GetNodeCopy() override;
		void ResetNodeValueData() override;
		std::vector<Node> GetNeighbours(Node &n) override;

		Node& operator[] (int x) override {
			return Nodes[x];
		}
		unsigned int width, height;
		std::vector<Node> Nodes;

		inline unsigned int GetID(unsigned int x, unsigned int y) {
			return ((x)+(y * width));
		};
	};

	template<typename Node>
	inline SquareGridGraph<Node>::SquareGridGraph() {
	}

	template<typename Node>
	inline SquareGridGraph<Node>::~SquareGridGraph() {
	}

	template<typename Node>
	inline void SquareGridGraph<Node>::Initialise(unsigned int width, unsigned int height) {
		unsigned int CachedWH = width * height;
		Nodes = std::vector<Node>(CachedWH);

		this->width = width;
		this->height = height;

		unsigned int y = -1;
		for (unsigned int xy = 0; xy < CachedWH; ++xy) {
			if (xy % width == 0)
				++y;
			Nodes[xy].Id = xy;
			Nodes[xy].x = xy % width;
			Nodes[xy].y = y;
		}
	}

	template<typename Node>
	inline void SquareGridGraph<Node>::SetExtraData(unsigned int x, unsigned int y, unsigned int Data) {
		Nodes[GetID(x, y)].ExtraData = Data;
	}

	template<typename Node>
	inline void SquareGridGraph<Node>::SetExtraData(unsigned int id, unsigned int Data) {
		Nodes[id].ExtraData = Data;
	}

	template<typename Node>
	inline unsigned int SquareGridGraph<Node>::GetExtraData(unsigned int x, unsigned int y) {
		return Nodes[GetID(x, y)].ExtraData;
	}

	template<typename Node>
	inline unsigned int SquareGridGraph<Node>::GetExtraData(unsigned int id) {
		return Nodes[id].ExtraData;
	}

	template<typename Node>
	inline Node SquareGridGraph<Node>::GetNode(unsigned int x, unsigned int y) {
		return Nodes[GetID(x, y)];
	}

	template<typename Node>
	inline Node SquareGridGraph<Node>::GetNode(unsigned int id) {
		return Nodes[id];
	}

	template<typename Node>
	inline std::vector<Node> SquareGridGraph<Node>::GetNodeCopy() {
		return Nodes;
	}

	template<typename Node>
	inline void SquareGridGraph<Node>::ResetNodeValueData() {
		for (int i = 0; i < Nodes.size(); i++) {
			Nodes[i].Value = 0;
			Nodes[i].Value2 = 0;
		}
	}

	template<typename Node>
	inline std::vector<Node> SquareGridGraph<Node>::GetNeighbours(Node& n) {
		std::vector<Node> Neighbours = std::vector<Node>();

		if (n.x != 0) {
			if (n.y != 0) {
				auto Node = GetNode(n.x - 1, n.y - 1);
				if (Node.ExtraData != 1) {
					Neighbours.push_back(Node);
				}
			}
			if (n.y != height - 1) {
				auto Node = GetNode(n.x - 1, n.y + 1);
				if (Node.ExtraData != 1) {
					Neighbours.push_back(Node);
				}
			}
			auto Node = GetNode(n.x - 1, n.y);
			if (Node.ExtraData != 1) {
				Neighbours.push_back(Node);
			}
		}

		if (n.x != width - 1) {
			if (n.y != 0) {
				auto Node = GetNode(n.x + 1, n.y - 1);
				if (Node.ExtraData != 1) {
					Neighbours.push_back(Node);
				}
			}
			if (n.y != height - 1) {
				auto Node = GetNode(n.x + 1, n.y + 1);
				if (Node.ExtraData != 1) {
					Neighbours.push_back(Node);
				}
			}
			auto Node = GetNode(n.x + 1, n.y);
			if (Node.ExtraData != 1) {
				Neighbours.push_back(Node);
			}
		}
		if (n.y != 0) {
			auto Node = GetNode(n.x, n.y - 1);
			if (Node.ExtraData != 1) {
				Neighbours.push_back(Node);
			}
		}
		if (n.y != height - 1) {
			auto Node = GetNode(n.x, n.y + 1);
			if (Node.ExtraData != 1) {
				Neighbours.push_back(Node);
			}
		}
		return Neighbours;
	}
}