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

#include "example_graphs.h"

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <cmath>
#include <random>
#include <ranges>
#include <vector>

namespace perf_rcsp {
namespace views = std::ranges::views;

std::vector<Site> create_sites(const int sites_count, std::mt19937 &gen) {
  std::vector<Site> sites;
  // Calculated a small grid that is big enough so it will not be difficult
  // to find unique random positions for sites.
  const int grid_side_length = 10 + std::sqrt(2 * sites_count);
  std::uniform_int_distribution<> coordinate_distribution(0, grid_side_length - 1);
  while (sites.size() < sites_count) {
    bool found_empty_position = false;
    // Use fixed iteration loop to avoid infinite loop.
    for (int i = 0; i < 100000; i++) {
      if (Site p = {static_cast<float>(coordinate_distribution(gen)), static_cast<float>(coordinate_distribution(gen))};
          !std::ranges::contains(sites, p)) {
        sites.emplace_back(p);
        found_empty_position = true;
        break;
      }
    }
    ASSERT_ALWAYS(found_empty_position);
  }

  return sites;
}
bool add_site_to_site_travel_edges(
  const std::vector<Site> &sites,
  const int latest_time,
  Index &extension_index,
  BoostGraph &graph
) {
  auto enumerated_sites = views::enumerate(sites);
  for (auto [from_vertex_index, from_site] : enumerated_sites) {
    for (auto [to_vertex_index, to_site] : enumerated_sites) {
      if (from_vertex_index == to_vertex_index) {
        continue;
      }
      float x_diff = (from_site.x - to_site.x);
      float y_diff = (from_site.y - to_site.y);
      float distance = std::sqrt(10 * (x_diff * x_diff + y_diff * y_diff));
      ASSERT_ALWAYS(0 < distance);
      boost::add_edge(
        from_vertex_index, to_vertex_index,
        ExtensionData(extension_index++, 0, latest_time, 2 * distance, distance, -distance, NOT_A_DELIVERY_MARKER),
        graph
      );
    }
  }
  return false;
}

void generate(const int sites_count, const int seed, SourceTargetBoostGraph &s_t_graph) {
  s_t_graph.source_vertex = 0;
  s_t_graph.target_vertex = sites_count;
  auto &graph = s_t_graph.graph;
  graph.clear();

  std::mt19937 gen(seed);
  const std::vector<Site> sites = create_sites(sites_count, gen);

  // add one vertex for each site
  for (auto [index, site] : views::enumerate(sites)) {
    boost::add_vertex(Vertex{boost::numeric_cast<Index>(index), site}, graph);
  }

  // add one extra vertex to be used as the target vertex and offset position for visualization
  boost::add_vertex(Vertex{sites.size(), Site{sites[0].x + 0.5f, sites[0].y + 0.5f}}, graph);

  const int latest_time = 100;
  Index extension_index = 0; // also the edge index
  // always add charger/fueling edge at index 0
  boost::add_edge(0, 0, ExtensionData(extension_index++, 0, latest_time, 3, 2, 4, NOT_A_DELIVERY_MARKER), graph);

  // add more chargers/fueling edges randomly
  std::uniform_real_distribution<> charger_distribution(0, 1);
  for (Index i = 1; i < sites_count; ++i) {
    if (charger_distribution(gen) < 0.15) {
      // add charger/fueling at index
      boost::add_edge(i, i, ExtensionData{extension_index++, 0, latest_time, 3, 2, 4, NOT_A_DELIVERY_MARKER}, graph);
    }
  }

  // add delivery edges
  for (Index i = 1; i < sites_count; ++i) {
    boost::add_edge(i, i, ExtensionData(extension_index++, 0, latest_time, 0, 0, 0, static_cast<int>(i)), graph);
  }

  add_site_to_site_travel_edges(sites, latest_time, extension_index, graph);

  // add a structural edge from source (index = 0) to target (index = sites_count)
  boost::add_edge(
    0, sites_count, ExtensionData(extension_index++, 0, latest_time, 0, 0, 0, NOT_A_DELIVERY_MARKER), graph
  );
}

} // namespace perf_rcsp
