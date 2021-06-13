#include "lib/fill.h"
#include "lib/graph.h"
#include "lib/lex.h"
#include "test_cases.cpp"
#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <vector>

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
  if (r.elimination_graph.new_edges.size() > 0) {
    printf("Added edges:\n");
    for (auto edge : r.elimination_graph.new_edges)
      printf("%c -> %c\n", r.original_graph[edge.first].id,
             r.original_graph[edge.second].id);
    printf("The new graph is:\n");
    print_graph(r.elimination_graph.filled_graph);
  } else
    printf("No edges added\n");
}

void print_results(const std::vector<Results> &res) {
  for (auto &test : res) {
    print(test);
    printf("-----------------------\n");
  }
}

void run_test(const std::vector<Results> &res) {
  for (auto test : res) {
    std::cout << "testcase: " << test.test_name << " -- ";
    if (test.expected_num_added_edges !=
        test.elimination_graph.new_edges.size())
      printf("***FAILED****\n");
    else
      printf("passed\n");
  }
}

int main() {
  auto results_p = get_test_results(lexp);
  auto results_m = get_test_results(lexm);
  printf("Results LEXP+FILL\n");
  print_results(results_p);
  printf("Results LEXM+FILL\n");
  print_results(results_m);
  printf("\nTests for LEXP+FILL\n");
  run_test(results_p);
  printf("\nTests for LEXM+FILL\n");
  run_test(results_m);
}
