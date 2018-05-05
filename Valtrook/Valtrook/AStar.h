#pragma once

#include "Path.h"
#include "GraphInterface.h"


#include <queue>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <stack>

namespace Val {
	template<class Graph, typename Node>
	class AStar {
	public:
		AStar();
		~AStar();

		void Initialise(Graph graph);

		virtual float GetExtraDataCost(unsigned int data);
		virtual float HeuristicValue(Node a, Node b);
		Path<Node> GetSolvePath(Node start, Node end);
		unsigned int NodeCloseCount;
	protected:
		Graph graph;
	};

	template<class Graph, typename Node>
	inline AStar<Graph, Node>::AStar() {
		NodeCloseCount = 0;
	}

	template<class Graph, typename Node>
	inline AStar<Graph, Node>::~AStar() {
	}

	template<class Graph, typename Node>
	inline void AStar<Graph, Node>::Initialise(Graph graph) {
		this->graph = graph;

		for (unsigned int i = 0; i < graph.Nodes.size(); i++) {
			graph.Nodes[i].Value = 0;
			graph.Nodes[i].Value2 = 0;
		}
	}

	template<class Graph, typename Node>
	inline float AStar<Graph, Node>::GetExtraDataCost(unsigned int data) {
		return 0.0f;
	}

	template<class Graph, typename Node>
	inline float AStar<Graph, Node>::HeuristicValue(Node a, Node b) {
		return DistanceBetween(a, b) + a.Value + GetExtraDataCost(a.ExtraData);
	}

	template<typename Node>
	struct AStarNodeComparison {
		bool operator() (const Node &rhs, const Node &lhs) {
			static float StepMod = 1.0f;
			static float HeuristicMod = 1.0f;


			return (rhs.Value * StepMod) + (rhs.Value2 * HeuristicMod) > (lhs.Value * StepMod) + (lhs.Value2 * HeuristicMod);
		}
	};


	template<class Graph, typename Node>
	inline Path<Node> AStar<Graph, Node>::GetSolvePath(Node start, Node end) {
		std::vector<Node> Nodes = graph.GetNodeCopy();
		NodeCloseCount = 0;
		Node current = start;


		//Node: Value = StepCost, Value2 = HeuristicCost
		Nodes[current.Id].Value2 = HeuristicValue(current, end);


		// ThisNode, (From, Cost)
		//This should be a sorted queue based on StepCount & Heuristic
		//std::priority_queue<Node, std::vector<Node>, cmp<Node>> Open = std::priority_queue<Node, std::vector<Node>, cmp<Node>>();
		std::priority_queue<Node, std::vector<Node>, AStarNodeComparison<Node>> Open = std::priority_queue<Node, std::vector<Node>, AStarNodeComparison<Node>>();

		//std::deque<unsigned int> Open = std::deque<unsigned int>();
		//Open.push(start);
		Open.push(start);
		std::unordered_set<unsigned int> Opened = std::unordered_set<unsigned int>();
		std::unordered_set<unsigned int> Closed = std::unordered_set<unsigned int>();
		std::unordered_map<unsigned int, unsigned int> ComeFrom = std::unordered_map<unsigned int, unsigned int>();
		std::vector<Node> Neighbours;

		ComeFrom.insert(std::make_pair(end.Id, -1));

		while (Open.size() > 0) {
			current = Nodes[Open.top().Id];
			Open.pop();
			Opened.erase(current.Id);
			Closed.insert(current.Id);
			NodeCloseCount++;


			Neighbours = graph.GetNeighbours(current);
			for (auto Node : Neighbours) {
				if (Closed.find(Node.Id) != Closed.end() || Opened.find(Node.Id) != Opened.end()) {
					//Update if the step cost is less.
					if (Node.Value > current.Value + DistanceBetween(Node, current)) {
						Nodes[Node.Id].Value = current.Value + DistanceBetween(Node, current);
						ComeFrom.erase(Node.Id);
						ComeFrom.insert(std::make_pair(Node.Id, current.Id));
					}
				} else {
					Nodes[Node.Id].Value2 = HeuristicValue(Node, end);
					Nodes[Node.Id].Value = current.Value + DistanceBetween(Node, current);
					Open.push(Nodes[Node.Id]);
					Opened.insert(Node.Id);

					ComeFrom.erase(Node.Id);
					ComeFrom.insert(std::make_pair(Node.Id, current.Id));
					if (Node.Id == end.Id) {
						goto Done;
					}
				}
			}

			//std::sort(Open.begin(), Open.end(), AStarNodeComparison<Node>());

			if (current.Id == end.Id)
				break;
		}
		Done:

		if (ComeFrom.at(end.Id) == -1) {
			return  Path<Node>(start, end, std::vector<Node>());
		}

		std::vector<Node> PathNodes = std::vector<Node>();
		Node n = end;
		while (!(n == start)) {
			PathNodes.push_back(n);
			n = Nodes[ComeFrom.at(n.Id)];
		}

		std::reverse(PathNodes.begin(), PathNodes.end());

		Path<Node> path = Path<Node>(start, end, PathNodes);

		return path;
	}
}