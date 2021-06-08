#include <vector>
#include <unordered_map>
#include "graph.h"

#ifndef LEXP_H
#define LEXP_H

/* 
* Use a struct to gather alpha and its inverse
**/
struct order
{
    std::vector<Node *> alpha;
    std::unordered_map<Node *, int> alphainv;
};

/**
 * return the elimination order for the graph G
 */
order lexp_iter(std::vector<Node> &G);
order lexm(std::vector<Node> &G);

#endif