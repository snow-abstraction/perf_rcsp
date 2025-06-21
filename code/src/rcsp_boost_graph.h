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

// save the graph to the DOT file format.
// See: https://graphviz.org/doc/info/lang.html
// The dot file that saved includes site positions as vertex positions
// which the neato graph layout algorithm will use when i.e. creating a
// svg: dot -Kneato -Tsvg graph.dot -o graph.svg
void save_to_dot_file(const BoostGraph &graph, const char *filename);

class Extension // TODO: replace with lambda?
{
public:
  bool operator()(
    const BoostGraph &g,
    State &new_cont,
    const State &old_cont,
    const boost::graph_traits<BoostGraph>::edge_descriptor &ed
  ) const {
    const ExtensionData &extension_data = get(boost::edge_bundle, g)[ed];
    return extend(old_cont, extension_data, new_cont);
  }
};

class Dominance // TODO: replace with lambda?
{
public:
  bool operator()(const State &res_cont_1, const State &res_cont_2) const {
    return is_dominate(res_cont_1, res_cont_2);
  }
};

} // namespace perf_rcsp

#endif // RSCP_BOOST_GRAPH_H
