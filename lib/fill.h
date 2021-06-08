#include <vector>
#include "graph.h"
#include "lex.h"
#ifndef FILL_H
#define FILL_H

/**
 * Return 
 * - the new graph with the fill-in edges and the nodes respecting the order
 * - the number of added edges
 */
std::pair<std::vector<Node>, int> fill(std::vector<Node> &graph, order ord);
#endif