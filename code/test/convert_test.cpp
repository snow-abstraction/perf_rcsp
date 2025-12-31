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
// Created by douglas on 7/14/25.
//

#include "../../code/src/convert.h"
#include "../../code/src/example_graphs.h"
#include "../../code/src/rcsp.h"

#include <gtest/gtest.h>
#include <ranges>

using namespace perf_rcsp;
namespace views = std::views;

TEST(convert, convert_and_convert_back_gives_equal_source_target_graph) {
  for (int i = 1; i < 100; i++) {
    int seed = 42 + i;
    // always at least one site but not more deliveries than the model supports.
    int sites_count = i % (N_DELIVERIES - 1) + 1;
    auto source_target_graph = generate(sites_count, seed);
    auto source_target_boost_graph = convert_to_source_target_boost_graph(source_target_graph);
    auto converted_back = convert_to_graph(source_target_boost_graph);
    ASSERT_EQ(source_target_graph, converted_back);
  }
}
