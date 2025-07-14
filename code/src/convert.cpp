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

#include "convert.h"

#include <boost/range/iterator_range.hpp>

namespace perf_rcsp {
Graph convert_to_graph(const BoostGraph &boost_graph) {
  Graph graph;
  for (auto vertex_index : boost::make_iterator_range(boost::vertices(boost_graph))) {
    const BoostVertex &vertex = boost_graph[vertex_index];
    graph.add_vertex(vertex.site);
  }

  for (const auto &edge_description : boost::make_iterator_range(boost::edges(boost_graph))) {
    const ExtensionData &extension_data = boost_graph[edge_description];
    const Index source_vertex_index = boost_graph[boost::source(edge_description, boost_graph)].index;
    const Index target_vertex_index = boost_graph[boost::target(edge_description, boost_graph)].index;
    graph.add_edge(source_vertex_index, target_vertex_index, extension_data);
  }

  return graph;
}

BoostGraph convert_to_boost_graph(const Graph &graph) {
  BoostGraph boost_graph;

  for (const auto &v : graph.get_vertices()) {
    boost::add_vertex(BoostVertex{v.index, v.site}, boost_graph);
  }

  for (const auto &v : graph.get_vertices()) {
    for (const auto &e : v.out_edges) {
      boost::add_edge(v.index, e.vertex_index, e.data, boost_graph);
    }
  }

  return boost_graph;
}

} // namespace perf_rcsp
