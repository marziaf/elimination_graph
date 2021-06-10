#include "lex.h"
#include "graph.h"
#include <assert.h>
#include <iostream>
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
    assert(!set_stack.top().empty());

    for (auto s : set_stack.top()) {
      printf(" %c ", G[s].id);
    }

    // get a vertex from the topmost set
    int node = *set_stack.top().begin();
    set_stack.top().erase(node);

    printf("\nnode: %c\n", G[node].id);

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

/*
Order BFS(std::vector<Node> &G) {
  const int n = G.size();
  Order ord = Order(n);
  // store the order in which to visit the nodes
  std::queue<int> visit_order;
  std::vector<bool> visited(n, false);

  // add any node as root of the tree
  visit_order.push(0);
  visited[0] = true;

  // iteratively assign the cardinality
  int cardinality = n - 1;
  while (!visit_order.empty()) {
    assert(visited.size() <= n);
    assert(cardinality >= 0);

    // get a node with deque
    int node = visit_order.front();
    visit_order.pop();
    visited[node] = true;

    // assign cardinality
    add_in_order(ord, cardinality, node);
    cardinality--;

    // if not yet in queue, enque the adjacent nodes
    for (int neighbor : G[node].adj) {
      assert(neighbor < n && neighbor >= 0);
      if (!visited[neighbor]) {
        visit_order.push(neighbor);
        visited[neighbor] = true;
      }
    }
  }
  return ord;
}
*/
/*
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
*/