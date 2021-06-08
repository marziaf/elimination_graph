#include <vector>
#include <unordered_map>
#include "graph.h"
#include "lex.h"
#include "fill.h"

/**
 * Return 
 * - the new graph with the fill-in edges and the nodes respecting the order
 * - the number of added edges
 */
std::pair<std::vector<Node>, int> fill(std::vector<Node> &graph, order ord)
{
    int n = graph.size();
    // Copy the graph reordering the nodes
    std::vector<Node> filled_graph;
    for (int i = 0; i < n; ++i)
        filled_graph.push_back(*ord.alpha[i]);

    // Iterate over the nodes according to the order
    int num_filled_edges = 0;
    for (Node *node : ord.alpha)
    {
        // Look for its adjacent with lowest cardinality that comes after node
        int best_card = n;
        for (Node *neighbor : node->adj)
        {
            int neigh_card = ord.alphainv[neighbor];
            // smallest cardinality without considering yet eliminated nodes
            if (neigh_card < best_card && neigh_card > ord.alphainv[node])
                best_card = neigh_card;
        }
        // Update with fill in by connecting next_in_adj to adj arcs
        if (best_card < n)
        {
            std::unordered_set<Node *> adj = filled_graph[best_card].adj;
            for (Node *neighbor : node->adj)
            {
                if (ord.alphainv[neighbor] > best_card)
                {
                    if (adj.find(neighbor) == adj.end())
                    {
                        adj.insert(neighbor);
                        ++num_filled_edges;
                    }
                }
            }
        }
    }
    return {filled_graph, num_filled_edges};
}