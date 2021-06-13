#include "lib/fill.h"
#include "lib/graph.h"
#include "lib/lex.h"
#include "test_cases.h"
#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <vector>

#define LEXM 0
#define LEXP 1

// check if the new graph is minimum
void run_test_minimum_ordering(Results test, int type) {
  std::string type_string;
  Elimination_graph g;
  if (type == LEXM) {
    type_string = "lexm";
    g = test.elimination_graph_lexm;
  } else {
    type_string = "lexp";
    g = test.elimination_graph_lexp;
  }

  if (test.min_num_added_edges < 0) {
    if (g.new_edges.size() == 0)
      std::cout << "minimum with " << type_string << std::endl;
    else
      printf("unknown minimality\n");
  } else {
    if (test.min_num_added_edges != g.new_edges.size())
      std::cout << "***NOT MINIMUM with " << type_string << "****" << std::endl;
    else
      std::cout << "minimum with " << type_string << std::endl;
  }
}
/*
// check if the graph after lex+fill is a perfect elimination graph
void check_triangulation(const std::vector<Results> &res) {
  for (auto test : res) {
  }
}*/

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

void print_graphical_results(const Results &r, int type, bool print_original) {
  std::string type_string;
  Order ord;
  Elimination_graph elim;
  if (type == LEXM) {
    type_string = "LEXM";
    ord = r.ord_lexm;
    elim = r.elimination_graph_lexm;
  } else {
    type_string = "LEXP";
    ord = r.ord_lexp;
    elim = r.elimination_graph_lexp;
  }
  if (print_original) {
    printf("\nOriginal Graph:\n");
    print_graph(r.original_graph);
  }
  std::cout << std::endl << type_string << " + FILL" << std::endl;
  printf("Order:\n");
  print_order(ord, r.original_graph);
  if (elim.new_edges.size() > 0) {
    printf("Added edges:\n");
    for (auto edge : elim.new_edges)
      printf("%c -> %c\n", r.original_graph[edge.first].id,
             r.original_graph[edge.second].id);
    printf("The new graph is:\n");
    print_graph(elim.filled_graph);
  } else
    printf("No edges added\n");
}

void print_results(const std::vector<Results> &res) {
  for (auto &test : res) {
    std::cout << "\n\n\n ++++++++ RUNNING " << test.test_name << " ++++++++ "
              << std::endl;
    print_graphical_results(test, LEXM, true);
    printf("\n");
    run_test_minimum_ordering(test, LEXM);
    printf("\n");
    // TODO triangulation test
    print_graphical_results(test, LEXP, false);
    printf("\n");
    run_test_minimum_ordering(test, LEXP);
    printf("\n");
    // TODO triangulation test
  }
}

std::vector<Results> get_test_results() {
  std::vector<Results> tests;
  tests.push_back(Results("list", get_list_graph(), 0));
  tests.push_back(Results("triangulated", get_perfect_elimination_graph(), 0));
  tests.push_back(Results("tree", get_tree(), 0));
  tests.push_back(Results("non-triangulated", get_nontriang_graph(), 2));
  tests.push_back(Results("ring", get_ring_graph(), 2));
  tests.push_back(Results("many fill edges", get_I_love_edges_graph(), 5));
  tests.push_back(Results("random n=4, e=6", get_random_graph(4, 6), 0));
  tests.push_back(Results("random n=5, e=7", get_random_graph(5, 7), -1));
  tests.push_back(Results("random n=10, e=25", get_random_graph(10, 25), -1));

  for (auto &test : tests) {
    test.ord_lexm = lexm(test.original_graph);
    test.elimination_graph_lexm = fill(test.original_graph, test.ord_lexm);
    test.ord_lexp = lexp(test.original_graph);
    test.elimination_graph_lexp = fill(test.original_graph, test.ord_lexp);
  }
  return tests;
}

int main() {
  auto results = get_test_results();
  print_results(results);
}
