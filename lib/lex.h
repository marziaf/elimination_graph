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

/**
 * return the elimination order for the graph G
 */
Order lexp(const std::vector<Node> &G);
Order lexm(const std::vector<Node> &G);

#endif