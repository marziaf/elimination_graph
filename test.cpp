#include "lib/fill.h"
#include "lib/graph.h"
#include "lib/lex.h"
#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <unordered_set>
#include <vector>

void print_graph(std::vector<Node> &G);

void define_edges(std::vector<Node> &graph, int node, std::vector<int> adj) {
  for (int a : adj)
    graph[node].adj.insert(&graph[a]);
}

// Perfect elimination graph
std::vector<Node> get_perfect_elimination_graph() {
  const int n = 10;
  std::vector<Node> graph;
  for (int i = 0; i < n; ++i)
    graph.push_back(Node((char)('a' + i)));
  define_edges(graph, 0, {2, 4});
  define_edges(graph, 1, {5, 6});
  define_edges(graph, 2, {0, 3, 4, 8});
  define_edges(graph, 3, {2, 4, 8});
  define_edges(graph, 4, {0, 2, 3, 7, 8});
  define_edges(graph, 5, {1, 6, 9});
  define_edges(graph, 6, {1, 5, 9});
  define_edges(graph, 7, {4, 8, 9});
  define_edges(graph, 8, {2, 3, 4, 7, 9});
  define_edges(graph, 9, {5, 6, 7, 8});

  return graph;
}

// get simple non triangulated
std::vector<Node> get_nontriang_graph() {
  const int n = 6;
  std::vector<Node> graph;
  for (int i = 0; i < n; ++i)
    graph.push_back(Node((char)('a' + i)));
  define_edges(graph, 0, {2, 3});
  define_edges(graph, 1, {2, 4});
  define_edges(graph, 2, {0, 1, 5});
  define_edges(graph, 3, {0, 5});
  define_edges(graph, 4, {1, 5});
  define_edges(graph, 5, {2, 3, 4});
  return graph;
}

std::vector<Node> get_list_graph() {
  std::vector<Node> graph;
  const int n = 5;
  for (int i = 0; i < n; ++i)
    graph.push_back(Node((char)('a' + i)));
  define_edges(graph, 0, {1});
  define_edges(graph, 1, {0, 2});
  define_edges(graph, 2, {1, 3});
  define_edges(graph, 3, {2, 4});
  define_edges(graph, 4, {3});
  return graph;
}

// Returns a random graph of n nodes and e edges maximum
std::vector<Node> get_random_graph(int n, int e) {
  std::vector<Node> graph;
  // create the nodes
  for (int i = 0; i < n; ++i)
    graph.push_back(Node((char)('a' + i)));

  // create edges (max e)
  for (int i = 0; i < e; ++i) {
    int r1 = std::rand() % n;
    int r2 = std::rand() % n;
    while (r1 == r2)
      r2 = std::rand() % n;
    graph[r1].adj.insert(&graph[r2]);
    graph[r2].adj.insert(&graph[r1]);
  }
  return graph;
}

void print_graph(std::vector<Node> &G) {
  for (auto node : G) {
    printf("%c :  {\n", node.id);
    for (auto n : node.adj) {
      printf(" %c ", n->id);
    }
    printf("}");
  }
}

void print_order(order &ord) {
  for (Node *node : ord.alpha) {
    assert(node != 0);
    printf("%c ", node->id);
  }
  printf("\n");
}

struct Results {
  std::string test_name;
  std::vector<Node> original_graph;
  order ord;
  std::vector<Node> fillin_graph;
  int added_edges;
  int expected_added_edges;

  Results(std::string name, std::vector<Node> &&G, int e)
      : test_name(name), original_graph(std::move(G)),
        expected_added_edges(e){};

  // friend std::ostream &operator<<(std::ostream &os, Results &r) {
};

void print(Results &r) {
  std::cout << "TESTCASE: " << r.test_name << std::endl;
  printf("ORIGINAL GRAPH:\n");
  print_graph(r.original_graph);
  printf("ORDER:\n");
  print_order(r.ord);
  printf("FILL_IN GRAPH:\n");
  printf("%d edges added\n", r.added_edges);
  if (r.added_edges > 0) {
    print_graph(r.fillin_graph);
  }
}
std::vector<Results> get_test_results(order (*lexfun)(std::vector<Node> &)) {
  std::vector<Results> tests;
  auto g = get_list_graph();
  tests.emplace_back("list", std::move(g), 0);
  /*tests.emplace_back(
      new Results("triangulated", get_perfect_elimination_graph(), 0));
  tests.emplace_back(new Results("non-triangulated", get_nontriang_graph(),
  2));*/
  for (auto &test : tests) {
    test.ord = lexfun(test.original_graph);
    std::tie(test.fillin_graph, test.added_edges) =
        fill(test.original_graph, test.ord);
  }
  return tests;
}

void print_results(std::vector<Results> &res) {
  for (auto &test : res) {
    print(test);
  }
  printf("-----------------------\n");
}

int main() {
  auto results_p = get_test_results(lexp_iter);
  print_results(results_p);
}