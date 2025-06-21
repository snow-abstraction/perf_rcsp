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
// Created by douglas on 6/21/25.
//

#ifndef EXAMPLE_GRAPHS_H
#define EXAMPLE_GRAPHS_H

#include "rcsp_boost_graph.h"

namespace perf_rcsp {
struct SourceTargetBoostGraph {
  Index source_vertex = -1;
  Index target_vertex = -1;
  BoostGraph graph;
};

// The function generate generates random SourceTargetBoostGraph
// to run a RCSP algorithm on.
//
// Note: use a reference instead of returning a SourceTargetBoostGraph, since
// it is unclear if copying or moving boost::graph works correctly.
void generate(int sites_count, int seed, SourceTargetBoostGraph &s_t_graph);

} // namespace perf_rcsp

#endif // EXAMPLE_GRAPHS_H
