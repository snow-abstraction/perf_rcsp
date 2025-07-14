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

#ifndef GRAPH_H
#define GRAPH_H

#include "vrp_model.h"

#include <vector>

namespace perf_rcsp {

struct TargetEdge {
  Index vertex_index = {};
  ExtensionData data = {};
};

struct Vertex {
  Index index = -1;
  Site site = {-1, -1};
  std::vector<TargetEdge> out_edges;
};

struct EdgeLocation {
  Index source_vertex_index = 0;
  Index out_edge_index = 0;
};

class Graph {
  std::vector<Vertex> vertices = {};
  std::vector<EdgeLocation> edges = {};

public:
  Index add_vertex(const Site &site) {
    Index index = vertices.size();
    vertices.emplace_back(index, site, std::vector<TargetEdge>());
    return index;
  }

  Index add_edge(Index source_vertex_index, Index target_vertex_index, ExtensionData data) {
    ASSERT_ALWAYS(source_vertex_index < vertices.size());
    ASSERT_ALWAYS(target_vertex_index < vertices.size());
    auto &out_edges = vertices[source_vertex_index].out_edges;
    edges.emplace_back(source_vertex_index, out_edges.size());
    out_edges.emplace_back(target_vertex_index, data);
    return edges.size() - 1;
  }

  [[nodiscard]] const ExtensionData &get_extension_data(Index edge_index) const {
    ASSERT_ALWAYS(edge_index < edges.size());
    const auto &edge_location = edges[edge_index];
    return vertices[edge_location.source_vertex_index].out_edges[edge_location.out_edge_index].data;
  }

  [[nodiscard]] const std::vector<Vertex> &get_vertices() const { return vertices; }
};

constexpr size_t ROOT_MARKER = 0;
struct LabelHistory {
  size_t parent_label_tree_index = ROOT_MARKER;
  size_t label_tree_index = ROOT_MARKER;
  EdgeLocation edge_location = {}; // meaningless at root
};

struct Label {
  State s = {};
  bool dominated = true;
  size_t label_tree_index = -1;
};

} // namespace perf_rcsp

#endif // GRAPH_H
