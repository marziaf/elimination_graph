#include "fill.h"
#include "graph.h"
#include "lex.h"
#include <assert.h>
#include <unordered_map>
#include <vector>

/**
 * Return
 * - the new graph with the fill-in edges
 * - the added edges
 */
Elimination_graph fill(std::vector<Node> G, const Order &ord) {
  int n = G.size();
  // Iterate over the nodes according to the order
  Elimination_graph elim = Elimination_graph(G);
  for (int node : ord.alpha) {
    // Look for its adjacent with lowest cardinality that comes after node (i.e.
    // not eliminated yet)
    printf("Working on node %c\n", G[node].id);
    int best_card = n;
    for (int adj : G[node].adj) {
      int adj_card = ord.alphainv[adj];
      if (adj_card < best_card && adj_card > ord.alphainv[node])
        best_card = adj_card;
    }

    if (best_card < n) {
      int best_card_pos = ord.alpha[best_card];
      // Update with fill in by connecting next_in_adj to adj arcs
      for (int adj : G[node].adj) {
        if (ord.alphainv[adj] > best_card &&
            G[best_card_pos].adj.find(adj) == G[best_card_pos].adj.end()) {
          printf("Adding edge %c -> %c\n", G[best_card_pos].id, G[adj].id);
          G[best_card_pos].adj.insert(adj);
          elim.new_edges.push_back({best_card_pos, adj});
        }
      }
    }
  }
  printf("************************");
  return elim;
}
