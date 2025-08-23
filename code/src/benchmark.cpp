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

static void BM_BoostRCSP(benchmark::State &state) {
  for (auto _ : state) {
    // 20 different instance to reduce variance
    for (int random_seed = 0; random_seed < 20; ++random_seed) {
      const perf_rcsp::State initial_state{};
      state.PauseTiming();
      perf_rcsp::SourceTargetBoostGraph s_t_g;
      perf_rcsp::generate(static_cast<int>(state.range(0)), random_seed, s_t_g);
      state.ResumeTiming();
      auto solutions = find_boost_solutions(s_t_g, initial_state);
      // It is intended that the generated instance should have some solutions.
      ASSERT_ALWAYS(!solutions.nondominated_end_states.empty());
    }
  }
}

static void BM_RCSP(benchmark::State &state) {
  for (auto _ : state) {
    // 20 different instance to reduce variance
    for (int random_seed = 0; random_seed < 20; ++random_seed) {
      const perf_rcsp::State initial_state{};
      state.PauseTiming();
      perf_rcsp::SourceTargetBoostGraph s_t_g;
      perf_rcsp::generate(static_cast<int>(state.range(0)), random_seed, s_t_g);
      auto graph = convert_to_graph(s_t_g.graph);
      state.ResumeTiming();
      auto solutions = find_solutions(graph, s_t_g.source_vertex, s_t_g.target_vertex, initial_state);
      // It is intended that the generated instance should have some solutions.
      ASSERT_ALWAYS(!solutions.nondominated_end_states.empty());
    }
  }
}

static void customer_sites_counts_4_15(benchmark::internal::Benchmark *b) {
  for (int customer_sites_count = 4; customer_sites_count <= 15; customer_sites_count += 1) {
    b->Args({customer_sites_count});
  }
}

static void customer_sites_counts_4_20(benchmark::internal::Benchmark *b) {
  for (int customer_sites_count = 4; customer_sites_count <= 20; customer_sites_count += 1) {
    b->Args({customer_sites_count});
  }
}

BENCHMARK(BM_BoostRCSP)->Unit(benchmark::kMillisecond)->Apply(customer_sites_counts_4_15);
BENCHMARK(BM_RCSP)->Unit(benchmark::kMillisecond)->Apply(customer_sites_counts_4_20);

BENCHMARK_MAIN();
