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

#include "rcsp_boost_graph.h"

#include <boost/graph/graphviz.hpp>
#include <spdlog/fmt/fmt.h>

namespace perf_rcsp {

void save_to_dot_file(const BoostGraph &graph, const char *filename) {

  auto vertex_writer = [&](std::ostream &out, auto v) {
    const auto &desc = graph[v];
    out << fmt::format("[label=\"{}\" pos=\"{},{}!\"]", desc.index, desc.site.x, desc.site.y);
  };

  auto edge_writer = [&](std::ostream &out, auto e) {
    const auto &desc = graph[e];
    out << fmt::format(
      "[label=\"{},tw:[{},{}],cost:{}\nduration:{},energy:{}{}\"]", desc.index, desc.earliest_time, desc.latest_time,
      desc.cost_change, desc.time_change, desc.energy_change,
      desc.delivery_index < NOT_A_DELIVERY_MARKER ? fmt::format("\ndelivery:{}", desc.delivery_index) : ""
    );
  };

  std::ofstream ofs(filename);
  boost::write_graphviz(ofs, graph, vertex_writer, edge_writer);
};

} // namespace perf_rcsp
