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

	// step 1: sets initialization
	// - insert all the nodes in the stack
	visit_order.push(&G[0]);

	// step 2: iteratively assign the cardinality
	int cardinality_to_assign = n - 1;
	while (!visit_order.empty())
	{
		// - get a node from the set on top of the queue
		// - delete it from the stack
		Node *node = visit_order.front();
		visit_order.pop();

		// proceed with cardinality assignment only if the
		// node has not been given a cardinality yet
		if (ord.alphainv[node] < 0)
		{
			// assign cardinality
			add_in_order(ord, cardinality_to_assign, node);
			--cardinality_to_assign;
			// visit the adjacent nodes and if not visited yet,
			// move them to the new adj_set to add in queue
			for (Node *neighbor : node->adj)
			{
				if (ord.alphainv[neighbor] < 0)
					visit_order.push(neighbor);
			}
		}
	}
	return ord;
}
