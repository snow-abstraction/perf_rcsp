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

#include "example_graphs.h"
#include "rcsp_boost_graph.h"

#include <filesystem>
#include <iostream>
#include <spdlog/fmt/fmt.h>
#include <string>

int main(int argc, char *argv[]) {
  using namespace perf_rcsp;
  try {
    if (argc != 4) {
      if (argc == 0) {
        throw std::invalid_argument("argc is 0");
      }
      std::filesystem::path program_path(argv[0]);
      std::string program_name = program_path.filename().string();
      fmt::println(
        R"(
{0} outputs an example graph in the DOT format to standard out.

Usage: {0} <sites count> <seed> <show travel edges' labels>
example: {0} 10 42 0)",
        program_name
      );
      return 2;
    }

    const int sites_count = std::atoi(argv[1]);
    const int seed = std::atoi(argv[2]);
    const bool show_travel_edges = std::atoi(argv[3]) != 0;

    SourceTargetBoostGraph s_t_g;
    generate(sites_count, seed, s_t_g);
    output_graph_as_dot(s_t_g.graph, show_travel_edges, std::cout);
  } catch (const std::exception &e) {
    fmt::println("exception occurred: {}", e.what());
    return 1;
  }
  return 0;
}
