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

void update_gstar(Elimination_graph &elim, int from, int to) {
  auto &g = elim.filled_graph;
  if (from != to && g[from].adj.find(to) == g[from].adj.end()) {
    elim.new_edges.push_back({from, to});
    g[from].adj.insert(to);
    g[to].adj.insert(from);
  }
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
 * Get node with highest cardinality
 */
int get_highest_node(std::vector<float> labels) {
  int n = labels.size();
  int highest_node = -1;
  float highest_lab = -1;
  for (int i = 0; i < n; i++) {
    if (labels[i] > highest_lab) {
      highest_node = i;
      highest_lab = labels[i];
    }
  }
  return highest_node;
}

struct Edge {
  int n1;
  int n2;
  inline bool operator==(Edge e) const {
    if ((n1 == e.n1 && n2 == e.n2) || (n1 == e.n2 && n2 == e.n1))
      return true;
    return false;
  }
};
class EgdeHash {
public:
  size_t operator()(const Edge e) const {
    size_t half_size_bit = sizeof(int) * 4;
    int min = e.n1 < e.n2 ? e.n1 : e.n2;
    int max = e.n1 < e.n2 ? e.n2 : e.n1;
    return min + 2 << half_size_bit * max;
  }
};

/**
 * returns the elimination order for the graph G and also the fill-in graph
 */
std::pair<Order, Elimination_graph> lexm(const std::vector<Node> &G) {
  int n = G.size();
  Order ord = Order(n);
  Elimination_graph elim = Elimination_graph(G);
  std::vector<float> labels(n);
  std::vector<bool> updated(n);
  int highest_node;

  // Assign cardinalities from the highest
  for (int card = n - 1; card >= 0; --card) {
    highest_node = get_highest_node(labels);
    add_in_order(ord, card, highest_node);

    // Time-optimized priority queue to store the nodes to visit
    // Ordered according the best path needed to reach the node: the nodes store
    // the information about the path with smallest labels that reaches highnode
    std::vector<std::vector<int>> search(labels[highest_node] + 1);
    std::unordered_set<Edge, EgdeHash> visited_edges;
    // Save what is the best path (minimum label nodes path) encountered
    std::vector<int> best_path(n, labels[highest_node] + 1);
    // Use level to access the search priority queue
    int level = labels[highest_node] + 1;
    // Push the adjacents to the node as first elements of the queue
    for (int adj : G[highest_node].adj) {
      if (labels[adj] >= 0) {
        search[(int)labels[adj]].push_back(adj);
        best_path[adj] = (int)labels[adj];
        labels[adj] += 0.5;
        updated[adj] = true;
        level = std::min(level, best_path[adj]);
      }
    }

    while (level < labels[highest_node] ||
           (level == labels[highest_node] && !search[level].empty())) {
      // Pop the elements on the queue until it is empty
      assert(!search[level].empty());
      int current = search[level].back();
      search[level].pop_back();
      // Visit the adjacents not in alphainv and which are connected to current
      // via a not yet visited edge
      for (int adj : G[current].adj) {
        if (ord.alphainv[adj] < 0 &&
            visited_edges.find({current, adj}) == visited_edges.end()) {
          // Mark the new edge as visited
          visited_edges.insert({current, adj});
          // Get the shortest path to reach adj
          int best_path_to_reach = std::min(best_path[current], best_path[adj]);
          // If the label is higher than the max label on the path to reach
          // highnode (stored in best path), there is a chain highnode->...->adj
          if (labels[adj] > best_path_to_reach && !updated[adj]) {
            labels[adj] += 0.5;
            update_gstar(elim, highest_node, adj);
            best_path[adj] = (int)labels[adj];
            updated[adj] = true;
          } else {
            best_path[adj] = best_path_to_reach;
          }
          search[best_path[adj]].push_back(adj);
        }
      }
      while (level <= labels[highest_node] && search[level].empty()) {
        level++;
      }
    }
    // reset updated
    // don't consider yet reached node labels
    for (Node node : G)
      if (ord.alphainv[node.pos] >= 0) {
        labels[node.pos] = -1;
        updated[node.pos] = true;
      } else {
        updated[node.pos] = false;
      }

    // sort labels and make the labels integer
    radix_sort_and_relabel(labels);
  }
  return {ord, elim};
}

/*
    // save the highest label of the adjacents
int level = 0;
// adjacents' label update:
// by definition there is a chain v->adj(v)
for (int adj : G[highest_node].adj) {
  if (!reached[adj]) {
    if (labels[adj] > level)
      level = labels[adj];
    reach[labels[adj]].push_back(adj);
    reached[adj] = true;
    labels[adj] += 0.5f;
  }
}
while (level > 0 || level == 0 && !reach[0].empty()) {
  int w = reach[level].back();
  reach[level].pop_back();
  
  for (int adj_w : G[w].adj) {
    if (!reached[adj_w]) {
      reached[adj_w] = true;

      if (labels[adj_w] > level) {
        // If lab(adjw) > level, there is a chain.
        // There can be a chain v-w-adjw-next
        // only if next > max(lab(adjw), level ) = lab(adjw)
        //
        // If lab(adjw) <= lab(w), there can be a chain v-w-adjw-next
        // only if next > max(lab(w), level) = level
        level = labels[adj_w]; // level has increased
        reach[level].push_back(adj_w);
        labels[adj_w] += 0.5f;
        update_gstar(elim, highest_node, adj_w);
      }
      reach[level].push_back(adj_w);
    }
  }
  while (level >= 0 && reach[level].empty()) {
    level--;
  }
}
*/