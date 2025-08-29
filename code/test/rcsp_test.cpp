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
//
// Created by douglas on 7/25/25.
//

#include "../../code/src/convert.h"
#include "../../code/src/example_graphs.h"
#include "../../code/src/rcsp.h"
#include "../../code/src/rcsp_boost_graph.h"

#include <gtest/gtest.h>
#include <ranges>

using namespace perf_rcsp;
namespace views = std::views;

TEST(rcsp, boost_gives_identical_number_of_optimal_states) {
  for (int i = 1; i < 100; i++) {
    SourceTargetBoostGraph s_t_g;
    int seed = 42 + i;
    // small for fast solve times.
    int sites_count = i % 5 + 1;
    generate(sites_count, seed, s_t_g);

    auto boost_solutions = find_boost_solutions(s_t_g, State{});

    auto graph = convert_to_graph(s_t_g.graph);
    auto solutions = find_ping_pong_solutions(graph, s_t_g.source_vertex, s_t_g.target_vertex, State{});

    ASSERT_EQ(boost_solutions.nondominated_end_states.size(), solutions.nondominated_end_states.size());
  }
}
