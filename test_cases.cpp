#include "lib/graph.h"
#include <assert.h>
#include <random>
#include <vector>

// Perfect elimination graph
std::vector<Node> get_perfect_elimination_graph() {
  const int n = 10;
  std::vector<Node> G;
  for (int i = 0; i < n; ++i)
    G.push_back(Node((char)('1' + i), i));
  G[0].adj = {2, 4};
  G[1].adj = {5, 6};
  G[2].adj = {0, 3, 4, 8};
  G[3].adj = {2, 4, 8};
  G[4].adj = {0, 2, 3, 7, 8};
  G[5].adj = {1, 6, 9};
  G[6].adj = {1, 5, 9};
  G[7].adj = {4, 8, 9};
  G[8].adj = {2, 3, 4, 7, 9};
  G[9].adj = {5, 6, 7, 8};

  return G;
}
std::vector<Node> get_tree() {
  const int n = 10;
  std::vector<Node> G;
  for (int i = 0; i < n; ++i)
    G.push_back(Node((char)('1' + i), i));
  G[0].adj = {4};
  G[1].adj = {6};
  G[2].adj = {8};
  G[3].adj = {8};
  G[4].adj = {0, 7};
  G[5].adj = {9};
  G[6].adj = {1, 9};
  G[7].adj = {4, 9};
  G[8].adj = {2, 3, 9};
  G[9].adj = {5, 6, 7, 8};

  return G;
}
// get simple non triangulated
std::vector<Node> get_nontriang_graph() {
  const int n = 6;
  std::vector<Node> G;
  for (int i = 0; i < n; ++i)
    G.push_back(Node((char)('1' + i), i));
  G[0].adj = {2, 3};
  G[1].adj = {2, 4};
  G[2].adj = {0, 1, 5};
  G[3].adj = {0, 5};
  G[4].adj = {1, 5};
  G[5].adj = {2, 3, 4};
  return G;
}

std::vector<Node> get_list_graph() {
  std::vector<Node> G;
  const int n = 5;
  for (int i = 0; i < n; ++i)
    G.push_back(Node((char)('a' + i), i));
  G[0].adj = {1};
  G[1].adj = {0, 2};
  G[2].adj = {1, 3};
  G[3].adj = {2, 4};
  G[4].adj = {3};
  return G;
}

std::vector<Node> get_ring_graph() {
  std::vector<Node> G;
  const int n = 5;
  for (int i = 0; i < n; ++i)
    G.push_back(Node((char)('a' + i), i));
  G[0].adj = {1, 4};
  G[1].adj = {0, 2};
  G[2].adj = {1, 3};
  G[3].adj = {2, 4};
  G[4].adj = {3, 0};
  return G;
}

std::vector<Node> get_I_love_edges_graph() {
  std::vector<Node> G;
  const int n = 9;
  for (int i = 0; i < n; ++i)
    G.push_back(Node((char)('1' + i), i));

  G[0].adj = {1, 4};
  G[1].adj = {0, 3, 5, 2};
  G[2].adj = {6, 1};
  G[3].adj = {1, 5, 7, 4};
  G[4].adj = {0, 3, 7};
  G[5].adj = {1, 6, 8, 3};
  G[6].adj = {2, 8, 5};
  G[7].adj = {4, 3, 8};
  G[8].adj = {5, 6, 7};

  return G;
}

// factorial of a number
int fact(int n) { return (n == 0) || (n == 1) ? 1 : n * fact(n - 1); }

// Returns a random graph of n nodes and e edges maximum
std::vector<Node> get_random_graph(int n, int e) {
  // Create an adjacency matrix to create the vertices
  bool m[n][n] = {false};
  // create the first n-1 egdes to ensure connectivity property
  // creating a tree involving all edges
  assert(e > n - 1);
  assert(e < fact(n));
  // keep track of the nodes not yet connected
  std::set<int> to_be_connected;
  for (int i = 0; i < n - 1; ++i) {
    to_be_connected.insert(i);
  }
  // each time take a random (already connected) root to connect to
  // a random number of not yet connected nodes
  int root = *to_be_connected.begin();
  int new_root;
  while (to_be_connected.size() > 0) {
    to_be_connected.erase(root);
    int root_connections = (std::rand() % (to_be_connected.size() - 1)) + 1;
    for (int i = 0; i < root_connections; ++i) {
      int child = *to_be_connected.begin();
      if (i == 0)
        new_root = child;
      m[root][child] = true;
      m[child][root] = true;
      to_be_connected.erase(child);
    }
    root = new_root;
  }

  // Create the remaining connections
  e -= n - 1;
  while (e > 0) {
    int r1 = std::rand() % n;
    int r2 = std::rand() % n;
    if (r1 != r2 && !m[r1][r2]) {
      m[r1][r2] = true;
      m[r2][r1] = true;
      e--;
    }
  }

  // create graph
  std::vector<Node> G(n);
  for (int r = 0; r < n; r++) {
    G[r] = Node((char)('a' + r), r);
    for (int c = 0; c < n; c++) {
      if (m[r][c])
        G[r].adj.insert(c);
    }
  }
  return G;
}

struct Results {
  std::string test_name;
  std::vector<Node> original_graph;
  Order ord;
  Elimination_graph elimination_graph;
  int expected_num_added_edges;

  Results(std::string name, std::vector<Node> G, int e)
      : test_name(name), original_graph(G), expected_num_added_edges(e){};
};

std::vector<Results>
get_test_results(Order (*lexfun)(const std::vector<Node> &)) {
  std::vector<Results> tests;
  tests.push_back(Results("list", get_list_graph(), 0));
  tests.push_back(Results("triangulated", get_perfect_elimination_graph(), 0));
  tests.push_back(Results("tree", get_tree(), 0));
  tests.push_back(Results("non-triangulated", get_nontriang_graph(), 2));
  tests.push_back(Results("ring", get_ring_graph(), 2));
  tests.push_back(Results("many fill edges", get_I_love_edges_graph(), 5));

  for (auto &test : tests) {
    test.ord = lexfun(test.original_graph);
    test.elimination_graph = fill(test.original_graph, test.ord);
  }
  return tests;
}
