#include <unordered_set>

#ifndef GRAPH_H
#define GRAPH_H

struct Node {
  char id = 0;
  // adj : the adjacent nodes
  std::unordered_set<Node *> adj;

  Node(char ID) : id(ID) {}
  // Node(const Node &n) : id(id), adj(adj) {}
};
#endif