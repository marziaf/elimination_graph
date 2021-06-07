#include <unordered_set>

#define GRAPH_H

struct Node
{
    char id;
    // adj : the adjacent nodes
    std::unordered_set<Node *> adj;
};