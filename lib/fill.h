#include "graph.h"
#include "lex.h"
#include <vector>

#ifndef FILL_H
#define FILL_H

/**
 * Return
 * - the new graph with the fill-in edges
 * - the added edges
 */
std::pair<std::vector<Node>, std::vector<std::pair<int, int>>>
fill(std::vector<Node> G, const Order &ord);
#endif