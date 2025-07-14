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
#include "../../code/src/rcsp_graph.h"

#include <gtest/gtest.h>

using namespace perf_rcsp;

// TODO: test name
// TODO: parameterize of size and seeds
TEST(equivalent_solutions, test1) {
  int seed = 42;
  const State initial_state{};
  SourceTargetBoostGraph s_t_g;
  generate(5, seed, s_t_g);

  // TODO: rename type?
  RCSPSolutions solutions = find_solutions(s_t_g, initial_state);

  auto graph = convert(s_t_g.graph);

  // TODO: rename type?
  RCSP_Result result = run(graph, s_t_g.source_vertex, s_t_g.target_vertex, initial_state);

  // align member names of RCSPSolutions and RCSP_Result

  ASSERT_EQ(solutions.end_states.size(), result.nondominated_states.size());
}
