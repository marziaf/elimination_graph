#include "lex.h"
#include "graph.h"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

/**
 * Put node in order position pos
 */
void add_in_order(Order &ord, int card, int node_pos) {
  assert(card < ord.alpha.size() && card >= 0);
  ord.alpha[card] = node_pos;
  ord.alphainv[node_pos] = card;
}

/**
 * returns the elimination order for the graph G
 */
Order lexp(const std::vector<Node> &G) {
  const int n = G.size();
  Order order = Order(n);
  std::stack<std::unordered_set<int>> set_stack;

  // Initialize the stack with the first set containing all the nodes
  std::unordered_set<int> init_set;
  for (Node node : G)
    init_set.insert(node.pos);
  set_stack.push(init_set);

  int cardinality = n - 1;
  while (!set_stack.empty()) {
    // get a vertex from the topmost set
    int node = *set_stack.top().begin();
    set_stack.top().erase(node);

    // if it has no cardinality yet, assign it
    if (order.alphainv[node] < 0) {
      add_in_order(order, cardinality, node);
      cardinality--;

      // create a new set with the unvisited adjacents and push it
      std::unordered_set<int> new_set;
      for (int adj : G[node].adj) {
        set_stack.top().erase(adj);
        if (order.alphainv[adj] < 0)
          new_set.insert(adj);
      }
      set_stack.push(new_set);
    }
    // remove empty sets
    while (!set_stack.empty() && set_stack.top().empty())
      set_stack.pop();
  }
  return order;
}

Order lexm(const std::vector<Node> &G) {
  int n = G.size();
  Order ord = Order(n);
  std::vector<float> labels(n);
  // reach[l] = {nodes reachable from a node with label l}
  std::vector<std::vector<int>> reach;
  std::vector<int> reached(n, false);

  // Assign cardinalities from the highest
  int highest_node = 0;
  float highest_card = 0;
  for (int card = n - 1; card >= 0; --card) {
    reach.resize(highest_card + 2);

    // Set the cardinality of the node with highest label
    add_in_order(ord, card, highest_node);
    reached[highest_node] = true;
    // adjacents' label update
    for (int adj : G[highest_node].adj) {
      if (!reached[adj]) {
        reach[labels[adj]].push_back(adj);
        reached[adj] = true;
        labels[adj] += 0.5f;
      }
    }
    // search the chains from highest_node (begin chain) to each unnumbered
    // vertex w (end_chain) with a chain with labels < label(w) start the search
    // from the highest labels for efficiency
    for (int level = highest_card; level >= 0; level--) {
      while (!reach[level].empty()) {
        int w = reach[level].back();
        reach[level].pop_back();

        for (int adj_w : G[w].adj) {
          if (!reached[adj_w]) {

            reached[adj_w] = true;
            if (labels[adj_w] < level) { // connected to v by a chain
              reach[labels[adj_w]].push_back(adj_w);
              labels[adj_w] += 0.5f;
            } else {
              reach[level].push_back(adj_w);
            }
          }
        }
      }
    }
    // reach.clear();
    std::vector<std::pair<int, float>> sorted_labels;
    // reset reached
    // don't consider yet reached node labels
    for (Node node : G) {
      if (ord.alphainv[node.pos] >= 0) {
        reached[node.pos] = true;
        labels[node.pos] = -1;
      } else {
        reached[node.pos] = false;
      }
    }
    // save {node, label} on sorted labels
    for (int i = 0; i < n; i++)
      sorted_labels.push_back({i, labels[i]});

    // sort labels
    std::sort(sorted_labels.begin(), sorted_labels.end(),
              [](std::pair<int, float> a, std::pair<int, float> b) {
                return a.second > b.second;
              }); // TODO make custom sort
    // rename labels to integers
    float old_lab = -1;
    int updated_lab = -1;
    for (int i = n - 1; i >= 0; --i) {
      auto lab = sorted_labels[i];
      if (lab.second > old_lab) {
        old_lab = lab.second;
        updated_lab++;
      }
      lab.second = updated_lab;
      labels[lab.first] = updated_lab;
    }

    // set new high values
    highest_node = sorted_labels[0].first;
    highest_card = sorted_labels[0].second;
  }
  return ord;
}