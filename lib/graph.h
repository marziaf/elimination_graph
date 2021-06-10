#include <set>

#ifndef GRAPH_H
#define GRAPH_H

struct Node {
  char id;
  // pos: the position in the graph array
  int pos;
  // adj : the adjacent nodes pos
  std::set<int> adj;

  Node(char ID, int position) : id(ID), pos(position) {}
  Node() : id(0), pos(-1) {}
  // Node(const Node &n) : id(id), adj(adj) {}
};
#endif