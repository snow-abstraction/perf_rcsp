// Performance experiments for Resource Constrained Shortest Path Problem.
// Copyright (C) 2025 Douglas Wayne Potter
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General
// Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
// details.
//
// You should have received a copy of the GNU Affero General Public License along with this program. If not, see
// <https://www.gnu.org/licenses/>.
//

#include "convert.h"
#include "example_graphs.h"

#include <benchmark/benchmark.h>

constexpr int random_seed_base = 1235;

static void BM_BoostRCSP20(benchmark::State &state) {
  for (auto _ : state) {
    // 20 different instances to reduce variance
    for (int random_seed_offset = 0; random_seed_offset < 20; ++random_seed_offset) {
      const perf_rcsp::State initial_state{};
      state.PauseTiming();
      perf_rcsp::SourceTargetBoostGraph s_t_g;
      int random_seed = random_seed_base + random_seed_offset;
      perf_rcsp::generate(static_cast<int>(state.range(0)), random_seed, s_t_g);
      state.ResumeTiming();
      auto solutions = find_boost_solutions(s_t_g, initial_state);
      // It is intended that the generated instance should have some solutions.
      ASSERT_ALWAYS(!solutions.nondominated_end_states.empty());
    }
  }
}

static void BM_RCSP20(benchmark::State &state) {
  for (auto _ : state) {
    // 20 different instances to reduce variance
    for (int random_seed_offset = 0; random_seed_offset < 20; ++random_seed_offset) {
      const perf_rcsp::State initial_state{};
      state.PauseTiming();
      perf_rcsp::SourceTargetBoostGraph s_t_g;
      int random_seed = random_seed_base + random_seed_offset;
      perf_rcsp::generate(static_cast<int>(state.range(0)), random_seed, s_t_g);
      auto graph = convert_to_graph(s_t_g.graph);
      state.ResumeTiming();
      auto solutions = find_solutions(graph, s_t_g.source_vertex, s_t_g.target_vertex, initial_state);
      // It is intended that the generated instance should have some solutions.
      ASSERT_ALWAYS(!solutions.nondominated_end_states.empty());
    }
  }
}

static void BM_BoostRCSP1(benchmark::State &state) {
  for (auto _ : state) {
    const perf_rcsp::State initial_state{};
    state.PauseTiming();
    perf_rcsp::SourceTargetBoostGraph s_t_g;
    perf_rcsp::generate(static_cast<int>(state.range(0)), random_seed_base, s_t_g);
    state.ResumeTiming();
    auto solutions = find_boost_solutions(s_t_g, initial_state);
    // It is intended that the generated instance should have some solutions.
    ASSERT_ALWAYS(!solutions.nondominated_end_states.empty());
  }
}

static void BM_RCSP1(benchmark::State &state) {
  for (auto _ : state) {

    const perf_rcsp::State initial_state{};
    state.PauseTiming();
    perf_rcsp::SourceTargetBoostGraph s_t_g;
    perf_rcsp::generate(static_cast<int>(state.range(0)), random_seed_base, s_t_g);
    auto graph = convert_to_graph(s_t_g.graph);
    state.ResumeTiming();
    auto solutions = find_solutions(graph, s_t_g.source_vertex, s_t_g.target_vertex, initial_state);
    // It is intended that the generated instance should have some solutions.
    ASSERT_ALWAYS(!solutions.nondominated_end_states.empty());
  }
}

static void customer_sites_counts_4_16(benchmark::internal::Benchmark *b) {
  for (int customer_sites_count = 4; customer_sites_count <= 16; customer_sites_count += 1) {
    b->Args({customer_sites_count});
  }
}

static void customer_sites_counts_4_20(benchmark::internal::Benchmark *b) {
  for (int customer_sites_count = 4; customer_sites_count <= 20; customer_sites_count += 1) {
    b->Args({customer_sites_count});
  }
}

BENCHMARK(BM_BoostRCSP20)->Unit(benchmark::kMillisecond)->Apply(customer_sites_counts_4_16);
BENCHMARK(BM_RCSP20)->Unit(benchmark::kMillisecond)->Apply(customer_sites_counts_4_20);
BENCHMARK(BM_BoostRCSP1)->Unit(benchmark::kMillisecond)->DenseRange(1, 25, 1);
BENCHMARK(BM_RCSP1)->Unit(benchmark::kMillisecond)->DenseRange(1, 25, 1);

BENCHMARK_MAIN();
