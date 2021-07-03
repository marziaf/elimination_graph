#include "test_cases.h"
#include "lib/graph.h"
#include <algorithm>
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

std::vector<Node> get_I_love_edges_minimum_graph() {
  std::vector<Node> G;
  const int n = 9;
  for (int i = 0; i < n; ++i)
    G.push_back(Node((char)('1' + i), i));

  G[0].adj = {1, 3, 4};
  G[1].adj = {0, 3, 5, 2};
  G[2].adj = {6, 5, 1};
  G[3].adj = {0, 1, 5, 7, 4};
  G[4].adj = {0, 3, 7};
  G[5].adj = {1, 2, 7, 6, 8, 3};
  G[6].adj = {2, 8, 5};
  G[7].adj = {4, 3, 5, 8};
  G[8].adj = {5, 6, 7};

  return G;
}

void check_graph_connected(const std::vector<Node> &g) {
  for (Node n : g) {
    assert(!n.adj.empty());
  }
}

// Returns a random graph of n nodes and e edges
std::vector<Node> get_random_graph(int n, int e) {
  // Create an adjacency matrix to create the vertices
  bool m[n][n];
  std::fill(*m, *m + n * n, false);
  // create the first n-1 egdes to ensure connectivity property
  // creating a tree involving all edges
  assert(n > 0);
  assert(e >= n - 1);
  assert(e <= n * (n - 1) / 2);
  // each time take an already connected root to connect to
  // a random number of not yet connected nodes
  int last_connected_node = 0;
  while (last_connected_node < n - 1) {
    // get new root from one of the previously connected nodes
    int root = std::rand() % (last_connected_node + 1);
    // create new edges from root to missing nodes
    int first_node = last_connected_node + 1;
    last_connected_node =
        std::min(first_node + (std::rand() % (n - first_node)), n - 1);

    for (int child = first_node; child <= last_connected_node; ++child) {
      m[root][child] = true;
      m[child][root] = true;
    }
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
  check_graph_connected(G);

  return G;
}