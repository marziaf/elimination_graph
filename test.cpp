#include "lib/fill.h"
#include "lib/graph.h"
#include "lib/lex.h"
#include "test_cases.h"
#include <algorithm>
#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <random>
#include <stack>
#include <stdio.h>
#include <vector>

#define LEXM 0
#define LEXP 1

// check if the graph after lexp+fill / lexm is a perfect elimination graph
// a graph is perfectly triangulated for order alpha if removing the edges in
// order it remains triangulated
bool check_triangulation(const Results &res, int type) {
  Order order;
  std::vector<Node> graph;
  if (type == LEXM) {
    order = res.ord_lexm;
    graph = res.elimination_graph_lexm.filled_graph;
  } else {
    order = res.ord_lexp;
    graph = res.elimination_graph_lexp.filled_graph;
  }

  for (int node : order.alpha) {
    // Virtually delete the node and the edges
    // Check that all its adjacents (not yet eliminated) are directly
    // interconnected
    for (int adj1 : graph[node].adj) {
      for (int adj2 : graph[node].adj) {
        if (adj1 != adj2 && order.alphainv[adj1] > order.alphainv[node] &&
            order.alphainv[adj2] > order.alphainv[node] &&
            graph[adj1].adj.find(adj2) == graph[adj1].adj.end() &&
            graph[adj2].adj.find(adj1) == graph[adj2].adj.end())
          return false;
      }
    }
  }
  return true;
}

std::vector<std::vector<int>> find_loop_4(std::vector<Node> G, int start) {
  std::vector<int> backtrack_stack;
  std::vector<std::vector<int>> cycles;
  std::vector<bool> visited(G.size(), false);
  backtrack_stack.push_back(start);
  visited[start] = true;
  while (!backtrack_stack.empty()) {
    int node = backtrack_stack.back();
    auto iter = G[node].adj.begin();
    while (iter != G[node].adj.end() && visited[*iter]) {
      iter++;
    }
    if (iter != G[node].adj.end()) {
      int adj = *iter;
      backtrack_stack.push_back(adj);
      visited[adj] = true;
      if (backtrack_stack.size() == 4) {
        if (G[adj].adj.find(start) != G[adj].adj.end()) {
          cycles.push_back(backtrack_stack);
        }
        backtrack_stack.pop_back();
      }
    } else {
      backtrack_stack.pop_back();
    }
  }

  return cycles;
}

bool check_triangulation_without_order(std::vector<Node> G) {
  for (Node n : G) {
    std::vector<std::vector<int>> cycles = find_loop_4(G, n.pos);
    for (auto loop : cycles) {
      if (G[loop[0]].adj.find(loop[2]) == G[loop[0]].adj.end() &&
          G[loop[1]].adj.find(loop[3]) == G[loop[1]].adj.end()) {
        return false;
      }
    }
  }
  return true;
}

bool check_minimality_through_triangulation(Results res) {
  Elimination_graph &g = res.elimination_graph_lexm;

  for (auto f : g.new_edges) {
    int n1 = f.first;
    int n2 = f.second;
    g.filled_graph[n1].adj.erase(n2);
    g.filled_graph[n2].adj.erase(n1);
    if (check_triangulation(res, LEXM))
      return false;
    g.filled_graph[n1].adj.insert(n2);
    g.filled_graph[n2].adj.insert(n1);
  }
  return true;
}

// Check minimal triangulation
// a graph has a minimal triangulation F iff each f in F is a unique chord of
// a 4-cycle
bool check_minimality(const Results &res) {
  std::vector<Node> orig = res.original_graph;
  Elimination_graph g = res.elimination_graph_lexm;
  for (auto f : g.new_edges) {
    // Find the triangles that share f as common edge by intersecting the
    // adjacents of the two nodes of the edge
    // Store the nodes not in f and check that no edge connects them
    // Check the cycles on the original graph
    Node n1 = orig[f.first];
    Node n2 = orig[f.second];
    std::vector<int> vertices;
    std::set_intersection(n1.adj.begin(), n1.adj.end(), n2.adj.begin(),
                          n2.adj.end(), std::back_inserter(vertices));
    // check the chords in the new graph
    for (int v1 : vertices) {
      for (int v2 : vertices) {
        if (g.filled_graph[v1].adj.find(v2) != g.filled_graph[v1].adj.end() ||
            g.filled_graph[v2].adj.find(v1) != g.filled_graph[v2].adj.end())
          return false;
      }
    }
  }
  return true;
}

// check if the new graph is minimum
void run_test_minimum_ordering(const Results &test, int type) {
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
      printf("unknown minimum\n");
  } else {
    if (test.min_num_added_edges != g.new_edges.size())
      std::cout << "not minimum " << type_string << std::endl;
    else
      std::cout << "minimum with " << type_string << std::endl;
  }
}

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
    type_string = "LEXP + FILL";
    ord = r.ord_lexp;
    elim = r.elimination_graph_lexp;
  }
  if (print_original) {
    printf("\nOriginal Graph:\n");
    print_graph(r.original_graph);
  }
  std::cout << std::endl << type_string << std::endl;
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
    print_graphical_results(test, LEXP, false);
    printf("\n");
    run_test_minimum_ordering(test, LEXP);
    printf("\n");
  }
}

std::vector<Results> get_test_results() {
  std::vector<Results> tests;

  tests.push_back(Results("list", get_list_graph(), 0));
  tests.push_back(Results("triangulated", get_perfect_elimination_graph(), 0));
  tests.push_back(Results("tree", get_tree(), 0));
  tests.push_back(Results("non-triangulated", get_nontriang_graph(), 2));
  tests.push_back(Results("ring", get_ring_graph(), 2));
  tests.push_back(Results("kite", get_kite_graph(), 0));
  tests.push_back(Results("many fill edges", get_I_love_edges_graph(), 3));
  tests.push_back(Results("fake minimum many fill edges",
                          get_I_love_edges_minimum_graph(), 5));
  tests.push_back(Results("random n=4, e=6", get_random_graph(4, 6), 0));
  tests.push_back(Results("random n=5, e=7", get_random_graph(5, 7), -1));
  tests.push_back(Results("random n=10, e=25", get_random_graph(10, 25), -1));
  tests.push_back(Results("random n=20, e=19", get_random_graph(20, 19), 0));
  for (int i = 0; i < 10; i++) {
    int n = 2 + std::rand() % 30;
    int e = (n - 1) + std::rand() % ((n - 1) * n / 2 - (n - 1) + 1);
    tests.push_back(
        Results("random n=" + std::to_string(n) + ", e=" + std::to_string(e),
                get_random_graph(n, e), -1));
  }

  for (auto &test : tests) {
    std::tie(test.ord_lexm, test.elimination_graph_lexm) =
        lexm(test.original_graph);
    test.ord_lexp = lexp(test.original_graph);
    test.elimination_graph_lexp = fill(test.original_graph, test.ord_lexp);
  }
  return tests;
}

void run_tests(const std::vector<Results> &res) {
  printf("LEXM\n");
  for (auto test : res) {
    if (check_triangulation(test, LEXM) &&
        check_minimality_through_triangulation(test))
      std::cout << test.test_name << ": passed" << std::endl;
    else
      std::cout << test.test_name << ": *****FAILED*****" << std::endl;
  }
  printf("LEXP+FILL\n");
  for (auto test : res) {
    bool original_triangulated =
        check_triangulation_without_order(test.original_graph);
    if ((original_triangulated &&
         test.elimination_graph_lexp.new_edges.size() == 0) ||
        (check_triangulation(test, LEXP)))
      std::cout << test.test_name << ": passed" << std::endl;
    else
      std::cout << test.test_name << ": *****FAILED*****" << std::endl;
  }
}

int main() {
  srand(time(NULL));
  // srand(0);
  auto results = get_test_results();
  run_tests(results);
  print_results(results);
}
