#include "lib/fill.h"
#include "lib/graph.h"
#include "lib/lex.h"
#include "test_cases.h"
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <vector>

// run with -O0 otherwise functions may even not run at all
// valgrind --tool=massif ./space lexp 10 20
int main(int argc, char **argv) {
  if (argc < 4)
    return 1;
  char *pend;
  std::string algorithm = argv[1];
  int n = std::strtol(argv[2], &pend, 10);
  int e = std::strtol(argv[3], &pend, 10);

  std::cout << "Running " << algorithm << " with n=" << n << ", e=" << e
            << std::endl;

  std::vector<Node> graph = get_random_graph(n, e);

  if (algorithm.compare("lexp") == 0) {
    lexp(graph);
  } else if (algorithm.compare("lexm") == 0) {
    lexm(graph);
  } else if (algorithm.compare("fill") == 0) {
    Order order = Order(n);
    for (int i = 0; i < n; i++) {
      order.alpha[i] = i;
      order.alphainv[i] = i;
    }
    fill(graph, order);
  } else {
    return 2;
  }

  return 0;
}