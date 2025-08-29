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

#ifndef RCSP_GRAPH_H
#define RCSP_GRAPH_H

#include "graph.h"
#include "vrp_model.h"

namespace perf_rcsp {

struct Solutions {
  // These two vector should have the same size. Applying all edges of the ith element of pareto_optimal_solutions
  // to the initial state should result in the ith end_states.

  // Edges of each path are in reverse order
  std::vector<std::vector<EdgeLocation>> nondominated_paths;
  std::vector<State> nondominated_end_states;
};

Solutions find_ping_pong_solutions(const Graph &g, Index source_index, Index target_index, State initial_state);

} // namespace perf_rcsp

#endif // RCSP_GRAPH_H
