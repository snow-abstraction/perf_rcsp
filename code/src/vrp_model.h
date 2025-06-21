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

#ifndef DATA_H
#define DATA_H

#include "util.h"

#include <bitset>

namespace perf_rcsp {
using Index = size_t;

constexpr int N_DELIVERIES = 32;
constexpr int NOT_A_DELIVERY_MARKER = N_DELIVERIES;

struct ExtensionData {
  Index index = -1; // added for use with and comparison with r_c_shortest_paths
  int earliest_time = 0;
  int latest_time = 0;
  int cost_change = 0;
  int time_change = 0;
  int energy_change = 0;
  int delivery_index = NOT_A_DELIVERY_MARKER;
};

struct State { // Also known as a "resource container".
  int cost = 0;
  int time = 0;
  int energy = 0;
  std::bitset<N_DELIVERIES> delivered; // The i-th bit is set if the i-th delivery has been delivered
  // Define operators to satisfy
  // https://www.boost.org/doc/libs/1_88_0/libs/graph/doc/r_c_shortest_paths.html
  // "the Less-Than operator for ResourceContainer must compare one or more resource(s) whose resource consumption(s)
  // along any arc is/are non-decreasing in order for the algorithm to work properly."
  bool operator<=(const State &rhs) const { return time <= rhs.time; }
  auto operator<=>(const State &rhs) const {
    if (*this <= rhs) {
      return rhs <= *this ? std::strong_ordering::equivalent : std::strong_ordering::less;
    }
    return std::strong_ordering::greater;
  }
  bool operator==(const State &) const = default;
};

// Return true if and only if the lhs State dominate over or equal to the rhs State.
inline bool is_dominate(const State &lhs, const State &rhs) {
  if (lhs.cost > rhs.cost) {
    return false;
  }

  if (lhs.time > rhs.time) {
    return false;
  }

  if (lhs.energy < rhs.energy) {
    return false;
  }

  for (size_t i = 0; i < lhs.delivered.size(); ++i) {
    // Is lhs not a subset of rhs w.r.t. deliveries?
    if (rhs.delivered.test(i) && !lhs.delivered.test(i)) {
      return false;
    }
  }

  return true;
}

// Extend the update the value of the new_state by "extending" old_state i.e. applying
// the extension logic to the old_state and the extension_data.
// Returns true only if the "extension" would result in a valid state.
inline bool extend(const State &old_state, const ExtensionData &extension_data, State &new_state) {
  if (extension_data.latest_time < old_state.time) {
    return false;
  }

  if (old_state.energy < -extension_data.energy_change) {
    return false;
  }

  const bool is_delivery = extension_data.delivery_index < NOT_A_DELIVERY_MARKER;
  if (is_delivery) {
    if (bool already_collected = old_state.delivered.test(extension_data.delivery_index); already_collected) {
      return false;
    }
  }

  new_state = old_state;
  new_state.cost += extension_data.cost_change;
  new_state.time += extension_data.time_change;
  new_state.energy += extension_data.energy_change;
  if (is_delivery) {
    new_state.delivered.set(extension_data.delivery_index);
  }

  return true;
}
} // namespace perf_rcsp

#endif // DATA_H
