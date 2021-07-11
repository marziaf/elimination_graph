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
    int e = n;
    std::vector<Node> graph = get_random_graph(n, e);
    Order order = Order(n);
    for (int i = 0; i < n; i++) {
      order.alpha[i] = i;
      order.alphainv[i] = i;
    }
    state.ResumeTiming();
    Elimination_graph res = fill(graph, order);
    state.PauseTiming();
    state.SetComplexityN(res.new_edges.size() + e);
  }
}

// n=n e=n*(n-2)/2 => O(e'+n) = O(n2)
void BM_fill_en2(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    int n = state.range(0);
    int e = n * (n - 1) / 2;
    std::vector<Node> graph = get_random_graph(n, e);
    Order order = Order(n);
    for (int i = 0; i < n; i++) {
      order.alpha[i] = i;
      order.alphainv[i] = i;
    }
    state.SetComplexityN(n);
    state.ResumeTiming();
    Elimination_graph res = fill(graph, order);
    state.PauseTiming();
    state.SetComplexityN(res.new_edges.size() + e);
  }
}

// BENCHMARK(BM_lexm_en)->DenseRange(10, 500, 20)->Complexity(benchmark::oAuto);
// BENCHMARK(BM_lexm_en2)->DenseRange(10, 200,
// 40)->Complexity(benchmark::oAuto); BENCHMARK(BM_lexp_en)->DenseRange(10, 500,
// 20)->Complexity(benchmark::oAuto); BENCHMARK(BM_lexp_en2)->DenseRange(10,
// 500, 20)->Complexity(benchmark::oAuto); BENCHMARK(BM_fill_en)->DenseRange(10,
// 500, 10)->Complexity(benchmark::oAuto);
// BENCHMARK(BM_fill_en2)->DenseRange(10, 500,
// 10)->Complexity(benchmark::oAuto);

BENCHMARK_MAIN();