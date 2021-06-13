#include "lib/fill.h"
#include "lib/graph.h"
#include "lib/lex.h"
#include "test_cases.h"
#include <assert.h>
#include <benchmark/benchmark.h>
#include <cstdlib>
#include <stdio.h>
#include <vector>

static void get_test_lexp(int n, int e) {
  Results test = Results("list", get_random_graph(n, e), -1);
}