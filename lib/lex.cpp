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
 *
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

void update_gstar(Elimination_graph &elim, int from, int to,
                  const std::vector<Node> &G) {
  if (G[from].adj.find(to) == G[from].adj.end()) {
    elim.new_edges.push_back({from, to});
  }
  elim.filled_graph[from].adj.insert(to);
  elim.filled_graph[to].adj.insert(from);
}

void countSort(std::vector<std::pair<int, int>> &vec, float div) {
  int count[10] = {0};
  // count the occurences of each digit
  for (auto v : vec) {
    int d = (int)(v.second / div) % 10;
    count[d]++;
  }
  // get the index where to put the first element with a certain digit
  int indices[10];
  indices[0] = 0;
  for (int i = 1; i < 10; i++)
    indices[i] = indices[i - 1] + count[i - 1];

  std::vector<std::pair<int, int>> tmp(vec.size());
  for (auto v : vec) {
    int d = (int)(v.second / div) % 10;
    tmp[indices[d]] = v;
    indices[d]++;
  }
  vec = tmp;
}

void relabel(std::vector<std::pair<int, int>> &vec) {
  int old_lab = -1;
  int updated_lab = -1;
  for (int i = 0; i < vec.size(); ++i) {
    if (vec[i].second > old_lab) {
      updated_lab++;
      old_lab = vec[i].second;
    }
    vec[i].second = updated_lab;
  }
}

void radix_sort_and_relabel(std::vector<float> &labels) {
  // find the max to know how many digits are there at most
  float max = -1;
  for (auto l : labels)
    if (l > max)
      max = l;
  // ignore negative labels
  // multiply by 10 to avoid floating point precision issues
  std::vector<std::pair<int, int>> pos;
  for (int i = 0; i < labels.size(); ++i) {
    if (labels[i] >= 0)
      pos.push_back({i, 10 * labels[i]});
  }
  max = max * 10;

  for (int e = 1; e <= max; e *= 10) {
    countSort(pos, e);
  }
  relabel(pos);
  for (auto p : pos)
    labels[p.first] = p.second;
}

/**
 * returns the elimination order for the graph G and also the fill-in graph
 */
std::pair<Order, Elimination_graph> lexm(const std::vector<Node> &G) {
  int n = G.size();
  Order ord = Order(n);
  Elimination_graph elim = Elimination_graph();
  for (Node node : G) {
    node.adj = {};
    elim.filled_graph.push_back(node);
  }
  std::vector<float> labels(n);
  // reach[l] = {nodes reachable from a node with label l}
  std::vector<int> reached(n, false);

  // Assign cardinalities from the highest
  for (int card = n - 1; card >= 0; --card) {
    // set new high values
    int highest_node = -1;
    float highest_lab = -1;
    for (int i = 0; i < n; i++) {
      if (labels[i] > highest_lab) {
        highest_node = i;
        highest_lab = labels[i];
      }
    }
    // Set the cardinality of the node with highest label
    add_in_order(ord, card, highest_node);
    reached[highest_node] = true;
    std::vector<std::vector<int>> reach(highest_lab + 1);

    // adjacents' label update
    for (int adj : G[highest_node].adj) {
      if (!reached[adj]) {
        reach[labels[adj]].push_back(adj);
        reached[adj] = true;
        labels[adj] += 0.5f;
        update_gstar(elim, highest_node, adj, G);
      }
    }
    // search the chains from highest_node (begin chain) to each unnumbered
    // vertex w (end_chain) with a chain with labels < label(w) start the
    // search from the highest labels for efficiency
    for (int level = highest_lab; level >= 0; level--) {
      while (!reach[level].empty()) {
        int w = reach[level].back();
        reach[level].pop_back();

        for (int adj_w : G[w].adj) {
          if (!reached[adj_w]) {

            reached[adj_w] = true;
            if (labels[adj_w] < level) { // connected to v by a chain
              reach[labels[adj_w]].push_back(adj_w);
              labels[adj_w] += 0.5f;
              update_gstar(elim, highest_node, adj_w, G);
            } else {
              reach[level].push_back(adj_w);
            }
          }
        }
      }
    }

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
    // sort labels and make the labels integer
    radix_sort_and_relabel(labels);
  }
  return {ord, elim};
}