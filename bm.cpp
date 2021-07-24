#include "lib/fill.h"
#include "lib/graph.h"
#include "lib/lex.h"
#include "test_cases.h"
#include <assert.h>
#include <benchmark/benchmark.h>
#include <cstdlib>
#include <stdio.h>
#include <vector>

void BM_lexp(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int n = state.range(0);
    int e = (n - 1) + std::rand() % (n * (n - 1) / 2 - (n - 1));
    std::vector<Node> graph = get_random_graph(n, e);
    state.SetComplexityN(n + e);
    state.ResumeTiming();
    benchmark::DoNotOptimize(lexp(graph));
  }
}

void BM_lexm(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int n = state.range(0);
    int e = (n - 1) + std::rand() % (n * (n - 1) / 2 - (n - 1));
    std::vector<Node> graph = get_random_graph(n, e);
    state.ResumeTiming();
    benchmark::DoNotOptimize(lexm(graph));
    state.SetComplexityN(n * e);
  }
}

void BM_fill(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int n = state.range(0);
    int e = (n - 1) + std::rand() % (n * (n - 1) / 2 - (n - 1));
    std::vector<Node> graph = get_random_graph(n, e);
    Order order = Order(n);
    for (int i = 0; i < n; i++) {
      order.alpha[i] = i;
      order.alphainv[i] = i;
    }
    state.ResumeTiming();
    Elimination_graph res = fill(graph, order);
    state.PauseTiming();
    state.SetComplexityN(n + res.new_edges.size() + e);
  }
}
// BENCHMARK(BM_lexm)->DenseRange(10, 30, 2)->Complexity(benchmark::oAuto);
// BENCHMARK(BM_lexp)->DenseRange(10, 70, 2)->Complexity(benchmark::oAuto);
BENCHMARK(BM_fill)->DenseRange(10, 30, 2)->Complexity(benchmark::oAuto);

BENCHMARK_MAIN();