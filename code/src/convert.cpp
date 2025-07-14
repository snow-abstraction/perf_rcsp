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

namespace perf_rcsp {
Graph convert(const BoostGraph &bg) {
  Graph g;
  for (auto _ : boost::make_iterator_range(boost::vertices(bg))) {
    g.add_vertex();
  }

  for (auto e : boost::make_iterator_range(boost::edges(bg))) {
    const ExtensionData &extension_data = bg[e];
    const Index s = bg[boost::source(e, bg)].index;
    const Index t = bg[boost::target(e, bg)].index;
    g.add_edge(s, t, extension_data);
  }

  return g;
}

} // namespace perf_rcsp
