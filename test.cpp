#include "lib/fill.h"
#include "lib/graph.h"
#include "lib/lex.h"
#include "test_cases.cpp"
#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <set>
#include <stdio.h>
#include <unordered_set>
#include <vector>

struct Results {
  std::string test_name;
  std::vector<Node> original_graph;
  Order ord;
  std::vector<Node> fillin_graph;
  std::vector<std::pair<int, int>> added_edges;
  int expected_num_added_edges;

  Results(std::string name, std::vector<Node> G, int e)
      : test_name(name), original_graph(G), expected_num_added_edges(e){};
};

void print_graph(const std::vector<Node> &G) {
  for (Node node : G) {
    printf("%c :  {", node.id);
    for (int adj : node.adj) {
      printf(" %c ", G[adj].id);
    }
    printf("}\n");
  }
}

void print_order(const Order &ord, const std::vector<Node> &G) {
  for (int n : ord.alpha) {
    printf("%c ", G[n].id);
  }
  printf("\n");
}

void print(const Results &r) {
  std::cout << "TESTCASE: " << r.test_name << std::endl;
  printf("ORIGINAL GRAPH:\n");
  print_graph(r.original_graph);
  printf("ORDER:\n");
  print_order(r.ord, r.original_graph);
  printf("FILL_IN GRAPH:\n");
  if (r.added_edges.size() > 0) {
    printf("Added edges:\n");
    for (auto edge : r.added_edges)
      printf("%c -> %c\n", edge.first, edge.second);
    printf("The new graph is:\n");
    print_graph(r.fillin_graph);
  } else
    printf("No edges added\n");
}
std::vector<Results> get_test_results(Order (*lexfun)(std::vector<Node> &)) {
  std::vector<Results> tests;
  tests.push_back(Results("list", get_list_graph(), 0));
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

void print_results(const std::vector<Results> &res) {
  for (auto &test : res) {
    print(test);
  }
  printf("-----------------------\n");
}

int main() {
  auto results_p = get_test_results(lexp_iter);
  print_results(results_p);
}