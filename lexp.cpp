#include <iostream>
#include <unordered_set>
#include <stack>
#include <unordered_map>
#include <vector>
#include <assert.h>

#include "lexp.h"
#ifndef GRAPH_H
#include "graph.h"
#endif

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

	// use a deque to create the list of sets of nodes
	std::stack<std::unordered_set<Node *>> sets;

	// step 1: sets initialization
	// - insert all the nodes in the first set
	std::unordered_set<Node *> init_set;
	for (Node &node : G)
		init_set.insert(&node);
	sets.push(init_set);

	// step 2: iteratively assign the order
	int cardinality_to_assign = n - 1;
	while (!sets.empty())
	{
		// - get a node from the set on top of the queue
		// - delete it from the set
		std::unordered_set<Node *> *set = &sets.top();

		assert(!set->empty());
		auto node_it = set->begin();
		Node *node = *node_it;
		set->erase(node_it);

		printf("Working on node %c\n", node->id);

		// proceed with cardinality assignment only if the
		// node has not been considered yet
		std::unordered_set<Node *> adj_set;
		if (ord.alphainv[node] < 0)
		{
			// assign cardinality
			add_in_order(ord, cardinality_to_assign, node);
			--cardinality_to_assign;
			// visit the adjacent nodes and if not ordered yet,
			// move them to the new adj_set to add in queue
			for (Node *neighbor : node->adj)
			{
				if (set->find(neighbor) != set->end())
					set->erase(neighbor);
				if (ord.alphainv[neighbor] < 0)
					adj_set.insert(neighbor);
			}
		}

		// - delete empty sets from queue
		if (set->empty())
			sets.pop();
		// insert the set created by adjacents only if not empty
		if (!adj_set.empty())
			sets.push(adj_set);
	}
	return ord;
}
