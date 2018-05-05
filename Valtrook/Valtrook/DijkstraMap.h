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
	class DijkstraMap {
	public:
		DijkstraMap();
		~DijkstraMap();

		void Initialise(Graph graph);

		virtual float GetExtraDataCost(unsigned int data);
		void Solve(Node end, bool force);
		Path<Node> GetSolvePath(Node start, Node end);
		Graph graph;
	protected:

		Node currentEnd;
	};

	template<class Graph, typename Node>
	inline DijkstraMap<Graph, Node>::DijkstraMap() {
		currentEnd.Id = -200;
	}

	template<class Graph, typename Node>
	inline DijkstraMap<Graph, Node>::~DijkstraMap() {
	}

	template<class Graph, typename Node>
	inline void DijkstraMap<Graph, Node>::Initialise(Graph graph) {
		this->graph = graph;
	}

	template<class Graph, typename Node>
	inline float DijkstraMap<Graph, Node>::GetExtraDataCost(unsigned int data) {
		return 0.0f;
	}

	template<class Graph, typename Node>
	inline void DijkstraMap<Graph, Node>::Solve(Node end, bool force) {
		if (this->currentEnd == end && !force) {
			return;
		}
		for (unsigned int i = 0; i < graph.Nodes.size(); i++) {
			graph.Nodes[i].Value = 0;
			graph.Nodes[i].Value = 0;
		}

		currentEnd = end;

		Node current = end;
		//Value = Cost, Value2 = GoTo
		graph.Nodes[current.Id].Value = 0;
		graph.Nodes[current.Id].Value2 = -1;


		// ThisNode, (From, Cost)
		//This should be a sorted queue based on StepCount & Heuristic
		//std::priority_queue<Node, std::vector<Node>, cmp<Node>> Open = std::priority_queue<Node, std::vector<Node>, cmp<Node>>();
		std::deque<Node> Open = std::deque<Node>();

		//std::deque<unsigned int> Open = std::deque<unsigned int>();
		//Open.push(start);
		Open.push_back(end);
		std::unordered_set<unsigned int> Opened = std::unordered_set<unsigned int>();
		std::unordered_set<unsigned int> Closed = std::unordered_set<unsigned int>();
		std::vector<Node> Neighbours = std::vector<Node>();

		while (Open.size() > 0) {
			current = graph.Nodes[Open.front().Id];
			Open.pop_front();
			Opened.erase(current.Id);
			Closed.insert(current.Id);

			Neighbours.clear();
			Neighbours = graph.GetNeighbours(current);
			for (auto Node : Neighbours) {
				if (Closed.find(Node.Id) != Closed.end() || Opened.find(Node.Id) != Opened.end()) {
					/*if (Node.Id = end.Id)
					continue;*/
					//Update if the step cost is less.
					if (Node.Value > current.Value + DistanceBetween(Node, current)) {
						graph.Nodes[Node.Id].Value = current.Value + DistanceBetween(Node, current);
						graph.Nodes[Node.Id].Value2 = current.Id;
					}
				} else {
					graph.Nodes[Node.Id].Value2 = current.Id;
					graph.Nodes[Node.Id].Value = current.Value + DistanceBetween(Node, current);
					Open.push_back(graph.Nodes[Node.Id]);
					Opened.insert(Node.Id);
				}
			}
		}
	}

	template<class Graph, typename Node>
	inline Path<Node> DijkstraMap<Graph, Node>::GetSolvePath(Node start, Node end) {
		Solve(end, false);

		std::vector<Node> PathNodes = std::vector<Node>();
		Node n = graph.Nodes[start.Id];
		while (!(n.Id == end.Id)) {
			PathNodes.push_back(n);
			n = graph.Nodes[graph.Nodes[n.Id].Value2];
		}

		std::reverse(PathNodes.begin(), PathNodes.end());

		Path<Node> path = Path<Node>(start, end, PathNodes);

		return path;
	}
}