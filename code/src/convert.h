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

#ifndef CONVERT_H
#define CONVERT_H

#include "rcsp.h"
#include "rcsp_boost_graph.h"

namespace perf_rcsp {
Graph convert_to_graph(const BoostGraph &boost_graph);

BoostGraph convert_to_boost_graph(const Graph &graph);

} // namespace perf_rcsp

#endif // CONVERT_H
