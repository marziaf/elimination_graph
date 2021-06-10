#include "lex.h"
#include "graph.h"
#include <assert.h>
#include <iostream>
#include <map>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/*
 * Initialize the structure order
 **/
order get_empty_order(std::vector<Node> &G) {
  int n = G.size();
  std::vector<Node *> alpha;
  std::unordered_map<Node *, int> alphainv;
  for (int i = 0; i < n; ++i) {
    alpha.push_back(nullptr);
    alphainv[&G[i]] = -1;
  }
  return {alpha, alphainv};
}
/**
 * Put node in order position pos
 */
void add_in_order(order &ord, int pos, Node *node) {
  assert(pos < ord.alpha.size());
  ord.alpha[pos] = node;
  ord.alphainv[node] = pos;
}

/**
 * returns the elimination order for the graph G
 */
order lexp_iter(std::vector<Node> &G) {
  int n = G.size();
  order ord = get_empty_order(G);
  std::queue<Node *> visit_order;
  std::unordered_set<Node *> visited;

  // add any node as root of the tree
  visit_order.push(&G[0]);
  visited.insert(&G[0]);

  // iteratively assign the cardinality
  int cardinality_to_assign = n - 1;
  while (!visit_order.empty()) {
    assert(visited.size() <= n);
    assert(cardinality_to_assign >= 0);
    // get a node with deque
    Node *node = visit_order.front();
    visit_order.pop();

    // assign cardinality
    add_in_order(ord, cardinality_to_assign, node);
    --cardinality_to_assign;
    // if not yet in queue, enque the adjacent nodes
    for (Node *neighbor : node->adj) {
      assert(neighbor->id - 'a' < n);
      if (visited.find(neighbor) == visited.end()) {
        visit_order.push(neighbor);
        visited.insert(neighbor);
      }
    }
  }
  return ord;
}

order lexm(std::vector<Node> &G) {
  int n = G.size();
  order ord = get_empty_order(G);
  std::unordered_map<Node *, float> node_labels;
  // reach[i] are the nodes m reachable from the considered node n
  // such that label(m) < label(n)
  std::unordered_map<float, std::vector<Node *>> reach;
  // keep track of the yet visited nodes
  std::unordered_set<Node *> reached;
  // Store one of the nodes with highest label
  Node *highest_node = &G[0];
  // Initialize labels to 0
  for (Node &node : G) {
    node_labels[&node] = 0;
  }
  float delta = 1 / n;
  // Assign cardinalities from the highest
  for (int card = n - 1; card >= 0; --card) {
    // Set the cardinality of the node with highest label
    add_in_order(ord, card, highest_node);
    int high_lab = node_labels[highest_node];
    reach.clear();
    reached.clear();
    // adjacents' label update
    for (Node *neighbor : highest_node->adj) {
      if (ord.alphainv[neighbor] < 0) {
        reach[node_labels[neighbor]].push_back(neighbor);
        reached.insert(neighbor);
        node_labels[neighbor] += delta;
        if (node_labels[neighbor] > node_labels[highest_node])
          highest_node = neighbor;
      }
    }
    // search the chains from highest_node to each unnumbered vertex w with a
    // chain with labels < label(w)
    for (int l = high_lab; l >= 0; --l) {
      // given k<l, n in reachable[k] => n in reachable[l]
      for (Node *r : reach[l]) {
        for (Node *adj : r->adj) {
          if (reached.find(adj) == reached.end()) {
            reached.insert(adj);
            if (node_labels[adj] < l) {
              reach[node_labels[adj]].push_back(adj);
              node_labels[adj] += delta;
              if (node_labels[adj] > node_labels[highest_node])
                highest_node = adj;
            } else
              reach[l].push_back(adj);
          }
        }
      }
    }
  }
}