#include "graph.h"
#include <unordered_map>
#include <vector>

#ifndef LEXP_H
#define LEXP_H

/*
 * Use a struct to gather alpha and its inverse
 **/
struct Order {
  // alpha[cardinality] = node.pos
  std::vector<int> alpha;
  // alphainv[node.pos] = cardinality
  std::vector<int> alphainv;

  Order(int n) : alpha(n, -1), alphainv(n, -1) {}
  Order() {}
};

struct Elimination_graph {
  std::vector<Node> filled_graph;
  std::vector<std::pair<int, int>> new_edges;

  Elimination_graph() {}
  Elimination_graph(std::vector<Node> g) : filled_graph(g) {}
  Elimination_graph(int n) : filled_graph(n) {}
};

/**
 * return the elimination order for the graph G
 */
Order lexp(const std::vector<Node> &G);
std::pair<Order, Elimination_graph> lexm(const std::vector<Node> &G);

#endif