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

// Note: boost graph does not define the equality operator and some
// answers only get mixed up with the concept of graph isomorphism. Here
// we just want simple equality.
bool equal_boost_graphs(const BoostGraph &lhs, const BoostGraph &rhs) {
  if (boost::num_edges(lhs) != boost::num_edges(rhs)) {
    return false;
  }

  if (boost::num_vertices(lhs) != boost::num_vertices(rhs)) {
    return false;
  }
  // check vertices
  for (auto [lhs_index, rhs_index] :
       views::zip(boost::make_iterator_range(boost::vertices(lhs)), boost::make_iterator_range(boost::vertices(rhs)))) {
    if (lhs_index != rhs_index) {
      return false;
    }
    const BoostVertex &lhs_vertex = lhs[lhs_index];
    const BoostVertex &rhs_vertex = rhs[rhs_index];
    if (lhs_vertex != rhs_vertex) {
      return false;
    }
  }
  // check edges
  for (auto [lhs_description, rhs_description] :
       views::zip(boost::make_iterator_range(boost::edges(lhs)), boost::make_iterator_range(boost::edges(rhs)))) {
    const Index lhs_source_vertex_index = lhs[boost::source(lhs_description, lhs)].index;
    const Index rhs_source_vertex_index = rhs[boost::source(rhs_description, rhs)].index;
    if (lhs_source_vertex_index != rhs_source_vertex_index) {
      return false;
    }

    const Index lhs_target_vertex_index = lhs[boost::target(lhs_description, lhs)].index;
    const Index rhs_target_vertex_index = rhs[boost::target(rhs_description, rhs)].index;
    if (lhs_target_vertex_index != rhs_target_vertex_index) {
      return false;
    }

    const ExtensionData &lhs_extension_data = lhs[lhs_description];
    const ExtensionData &rhs_extension_data = rhs[rhs_description];
    if (lhs_extension_data != rhs_extension_data) {
      return false;
    }
  }

  return true;
}

TEST(convert, convert_and_convert_back_gives_equal_boost_graph) {
  for (int i = 1; i < 100; i++) {
    SourceTargetBoostGraph s_t_g;
    int seed = 42 + i;
    // always at least one site but not more deliveries than the model supports.
    int sites_count = i % (N_DELIVERIES - 1) + 1;
    generate(sites_count, seed, s_t_g);
    auto graph = convert_to_graph(s_t_g.graph);
    auto converted_back = convert_to_boost_graph(graph);
    ASSERT_TRUE(equal_boost_graphs(s_t_g.graph, converted_back));
  }
}
