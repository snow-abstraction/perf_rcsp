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
#include <boost/graph/r_c_shortest_paths.hpp>
#include <iosfwd>
#include <spdlog/fmt/fmt.h>

namespace perf_rcsp {

void output_graph_as_dot(const BoostGraph &graph, bool show_travel_edges_label, std::ostream &ofs) {

  auto vertex_writer = [&](std::ostream &out, auto v) {
    const auto &desc = graph[v];
    out << fmt::format("[label=\"{}\" pos=\"{},{}!\"]", desc.index, desc.site.x, desc.site.y);
  };

  auto edge_writer = [&](std::ostream &out, auto e) {
    const auto &desc = graph[e];
    if (!show_travel_edges_label) {
      if (graph[boost::source(e, graph)].index != graph[boost::target(e, graph)].index) {
        return;
      }
    }
    out << fmt::format(
      "[label=\"{},tw:[{},{}],cost:{}\nduration:{},energy:{}{}\"]", desc.index, desc.earliest_time, desc.latest_time,
      desc.cost_change, desc.time_change, desc.energy_change,
      desc.delivery_index < NOT_A_DELIVERY_MARKER ? fmt::format("\ndelivery:{}", desc.delivery_index) : ""
    );
  };

  boost::write_graphviz(ofs, graph, vertex_writer, edge_writer);
};

RCSPSolutions find_solutions(const SourceTargetBoostGraph &graph, const State &initial_state) {
  class Extension {
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

  class Dominance {
  public:
    bool operator()(const State &res_cont_1, const State &res_cont_2) const {
      return is_dominate(res_cont_1, res_cont_2);
    }
  };

  RCSPSolutions solutions;
  const auto &g = graph.graph;
  boost::r_c_shortest_paths(
    g, get(&Vertex::index, g), get(&ExtensionData::index, g), graph.source_vertex, graph.target_vertex,
    solutions.pareto_optimal_solutions, solutions.end_states, initial_state, Extension(), Dominance()
  );

  return solutions;
}

} // namespace perf_rcsp
