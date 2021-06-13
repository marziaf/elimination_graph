#include "lib/fill.h"
#include "lib/graph.h"
#include "lib/lex.h"
#include "test_cases.h"
#include <assert.h>
#include <benchmark/benchmark.h>
#include <cstdlib>
#include <stdio.h>
#include <vector>

// n=n, e=n => O(e+n)=O(n)
void BM_lexp_en(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int n = state.range(0);
    std::vector<Node> graph = get_random_graph(n, n);
    state.SetComplexityN(n);
    state.ResumeTiming();
    benchmark::DoNotOptimize(lexp(graph));
  }
}

// n=n, e=n*(n-2)/2 => O(e+n) = O(n2)
void BM_lexp_en2(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int n = state.range(0);
    std::vector<Node> graph = get_random_graph(n, n * (n - 2) / 2);
    state.SetComplexityN(n);
    state.ResumeTiming();
    benchmark::DoNotOptimize(lexp(graph));
  }
}

// n=n, e=n => O(en)=O(n2)
void BM_lexm_en(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int n = state.range(0);
    std::vector<Node> graph = get_random_graph(n, n);
    state.SetComplexityN(n);
    state.ResumeTiming();
    benchmark::DoNotOptimize(lexm(graph));
  }
}

// n=n, e=n*n/4 => O(en) = O(n3)
void BM_lexm_en2(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int n = state.range(0);
    std::vector<Node> graph = get_random_graph(n, n * n / 4);
    state.SetComplexityN(n);
    state.ResumeTiming();
    benchmark::DoNotOptimize(lexm(graph));
  }
}

// n=n e=n => O(e'+n) = O(n) average
void BM_fill_en(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int n = state.range(0);
    std::vector<Node> graph = get_random_graph(n, n);
    Order order = lexp(graph);
    state.SetComplexityN(n);
    state.ResumeTiming();
    benchmark::DoNotOptimize(fill(graph, order));
  }
}

// n=n e=n*(n-2)/2 => O(e'+n) = O(n2)
void BM_fill_en2(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int n = state.range(0);
    std::vector<Node> graph = get_random_graph(n, n * (n - 2) / 2);
    Order order = lexp(graph);
    state.SetComplexityN(n);
    state.ResumeTiming();
    benchmark::DoNotOptimize(fill(graph, order));
  }
}

// BENCHMARK(BM_lexp_en)->DenseRange(5, 100, 10)->Complexity(benchmark::oAuto);
// BENCHMARK(BM_lexp_en2)->DenseRange(5, 100, 10)->Complexity(benchmark::oAuto);
BENCHMARK(BM_lexp_en)->DenseRange(100, 1000, 50)->Complexity(benchmark::oAuto);
BENCHMARK(BM_lexp_en2)->DenseRange(100, 1000, 50)->Complexity(benchmark::oAuto);
// BENCHMARK(BM_fill_en)->DenseRange(5, 100, 10)->Complexity(benchmark::oAuto);
// BENCHMARK(BM_fill_en2)->DenseRange(5, 100, 10)->Complexity(benchmark::oAuto);

BENCHMARK_MAIN();