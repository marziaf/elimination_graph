#include <iostream>
#include <unordered_set>
#include <queue>
#include <unordered_map>
#include <vector>
#include <assert.h>
#include "graph.h"
#include "lex.h"

/*
* Initialize the structure order
**/
order get_empty_order(std::vector<Node> &G)
{
	int n = G.size();
	std::vector<Node *> alpha(n);
	std::unordered_map<Node *, int> alphainv;
	for (int i = 0; i < n; ++i)
	{
		alpha[i] = nullptr;
		alphainv[&G[i]] = -1;
	}
	return {alpha, alphainv};
}
/**
 * Put node in order position pos
 */
void add_in_order(order &ord, int pos, Node *node)
{
	ord.alpha[pos] = node;
	ord.alphainv[node] = pos;
}

/**
 * returns the elimination order for the graph G
 */
order lexp_iter(std::vector<Node> &G)
{
	int n = G.size();
	order ord = get_empty_order(G);
	std::queue<Node *> visit_order;
	std::unordered_set<Node *> visited;

	// add any node as root of the tree
	visit_order.push(&G[0]);

	// iteratively assign the cardinality
	int cardinality_to_assign = n - 1;
	while (!visit_order.empty())
	{
		// get a node with deque
		Node *node = visit_order.front();
		visit_order.pop();
		visited.insert(node);

		// assign cardinality
		add_in_order(ord, cardinality_to_assign, node);
		--cardinality_to_assign;
		// if not yet in queue, enque the adjacent nodes
		for (Node *neighbor : node->adj)
		{
			if (visited.find(neighbor) == visited.end())
			{
				visit_order.push(neighbor);
				visited.insert(neighbor);
			}
		}
	}
	return ord;
}
