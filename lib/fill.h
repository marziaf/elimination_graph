#include "graph.h"
#include "lex.h"
#include <vector>

#ifndef FILL_H
#define FILL_H

struct Elimination_graph {
  std::vector<Node> filled_graph;
  std::vector<std::pair<int, int>> new_edges;

  Elimination_graph() {}
  Elimination_graph(std::vector<Node> g) : filled_graph(g) {}
};

/**
 * Return
 * - the new graph with the fill-in edges
 * - the added edges
 */
Elimination_graph fill(std::vector<Node> G, const Order &ord);

#endif