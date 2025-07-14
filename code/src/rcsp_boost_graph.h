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

#ifndef RSCP_BOOST_GRAPH_H
#define RSCP_BOOST_GRAPH_H

#include "vrp_model.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/r_c_shortest_paths.hpp>

namespace perf_rcsp {
struct Site {
  float x;
  float y;
  auto operator<=>(const Site &) const = default;
};

struct Vertex {
  Index index = -1;
  Site site = {-1, -1};
};

using BoostGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, Vertex, ExtensionData>;

struct SourceTargetBoostGraph {
  Index source_vertex = -1;
  Index target_vertex = -1;
  BoostGraph graph;
};

struct RCSPSolutions {
  // These two vector should have the same size. Applying all edges of the ith element of pareto_optimal_solutions
  // to the initial state should result in the ith end_states.
  std::vector<std::vector<boost::graph_traits<BoostGraph>::edge_descriptor>> pareto_optimal_solutions;
  std::vector<State> end_states;
};

// output the graph to the DOT file format to standard out.
// See: https://graphviz.org/doc/info/lang.html
// The dot file that saved includes site positions as vertex positions
// which the neato graph layout algorithm will use when i.e. creating a
// svg: dot -Kneato -Tsvg graph.dot -o graph.svg
void output_graph_as_dot(const BoostGraph &graph, bool show_travel_edges_label, std::ostream &ofs);

RCSPSolutions find_solutions(const SourceTargetBoostGraph &graph, const State &initial_state);

} // namespace perf_rcsp

#endif // RSCP_BOOST_GRAPH_H
