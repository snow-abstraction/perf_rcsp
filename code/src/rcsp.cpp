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

#include "rcsp.h"

#include <algorithm>
#include <ranges>
#include <vector>

namespace perf_rcsp {

namespace views = std::views;

// extend_and_handle_domination extends state to create a new state and handles domination:
// do not add a new state if it is dominated and marking other states as dominated if the
// new state dominates them.
void extend_and_handle_domination(
  const Label &old_label,
  const ExtensionData &extension_data,
  std::vector<Label> &next_labels, // next labels at target vertex
  std::vector<Label> &curr_labels, // current labels at target vertex
  Index node_index,
  Index out_edge_index,
  std::vector<LabelHistory> &label_tree
) {

  // TODO: consider inlining extend and avoid creating new_state until it is
  //  more certain that it would be valid.
  State new_state;
  if (!extend(old_label.s, extension_data, new_state)) {
    return;
  }

  for (auto &l : curr_labels) {
    if (is_dominate(l.s, new_state)) {
      return;
    }
  }

  for (auto &l : next_labels) {
    if (is_dominate(l.s, new_state)) {
      return;
    }
  }

  for (auto &l : next_labels) {
    if (is_dominate(new_state, l.s)) {
      l.dominated = true;
    }
  }

  for (auto &l : curr_labels) {
    if (is_dominate(new_state, l.s)) {
      l.dominated = true;
    }
  }

  size_t tree_index = label_tree.size();
  label_tree.emplace_back(old_label.label_tree_index, tree_index, EdgeLocation{node_index, out_edge_index});
  next_labels.emplace_back(new_state, false, tree_index);
}

Solutions find_solutions(const Graph &g, Index source_index, Index target_index, State initial_state) {
  // Note: the design tried to avoid pointer chasing when compared with boost::r_c_shortest_paths
  // but there are cons as well, such as how "early" we discover a state is dominated.

  ASSERT_ALWAYS(source_index != target_index);
  const auto &vs = g.get_vertices();
  ASSERT_ALWAYS(vs[target_index].out_edges.empty());
  std::vector<std::vector<Label>> curr(vs.size());
  std::vector<std::vector<Label>> next(vs.size());
  std::vector<LabelHistory> label_tree;
  std::vector<size_t> vertex_indices;
  for (const auto &v : vs) {
    vertex_indices.push_back(v.index);
  }

  { // set up label for initial state
    size_t label_tree_index = 0;
    curr[source_index].emplace_back(initial_state, false, label_tree_index);
    label_tree.push_back(LabelHistory{ROOT_MARKER, label_tree_index, source_index, 0});
  }

  bool states_not_target = true;
  while (states_not_target) {
    // TODO: add some heuristic to prefer creating states that won't be dominated earlier, e.g.
    //  order vertices by low average or median cost.
    states_not_target = false;
    // We skip propagating curr labels at target and get directly to next.
    ASSERT_ALWAYS(next[target_index].empty());
    std::swap(curr[target_index], next[target_index]);

    for (auto &labels : curr) {
      const auto [first, last] =
        std::ranges::remove_if(labels.begin(), labels.end(), [](const Label &l) { return l.dominated; });
      labels.erase(first, last);
      std::ranges::sort(labels, [](auto lhs, auto rhs) { return lhs.s.time < rhs.s.time; });
    }
    //
    std::ranges::sort(vertex_indices, [&curr](auto lhs_index, auto rhs_index) {
      const auto &lhs = curr[lhs_index];
      const auto &rhs = curr[rhs_index];
      if (lhs.empty() || rhs.empty()) {
        if (!rhs.empty()) {
          return true;
        }
        return false;
      }
      return lhs.front().s.time < rhs.front().s.time;
    });

    for (auto vertex_index : vertex_indices) {
      const auto &v = vs[vertex_index];
      auto &vertex_labels = curr[vertex_index];
      if (vertex_labels.empty()) {
        continue;
      }
      states_not_target = true;
      // The idea behind looping edge and then states is for more performant memory access
      // patterns: write all states to one target vertex before switching to another target vertex
      for (size_t out_edge_index = 0; out_edge_index != v.out_edges.size(); ++out_edge_index) {
        const auto &e = v.out_edges[out_edge_index];
        next[e.vertex_index].reserve(next[e.vertex_index].size() + vertex_labels.size());
        for (const auto &l : vertex_labels) {
          if (l.dominated) {
            continue;
          }
          extend_and_handle_domination(
            l, e.data, next[e.vertex_index], curr[e.vertex_index], v.index, out_edge_index, label_tree
          );
        }
      }
      vertex_labels.clear();
    }

    // TODO: instead of skipping dominated above. We could filter out dominated here, thinking
    //  of parallelization.
    std::swap(curr, next);
  }

  for (const auto [index, lh] : views::enumerate(label_tree)) {
    ASSERT_ALWAYS(static_cast<Index>(index) == lh.label_tree_index);
  }

  auto nondominated_target_labels = curr[target_index] | views::filter([](auto l) { return !l.dominated; });
  auto nondominated_target_states = nondominated_target_labels | views::transform([](auto l) { return l.s; });

  std::vector<std::vector<EdgeLocation>> nondominated_paths;
  for (const auto &l : nondominated_target_labels) {
    std::vector<EdgeLocation> path;
    Index label_tree_index = l.label_tree_index;
    while (label_tree_index != ROOT_MARKER) {
      const auto &lh = label_tree[label_tree_index];
      path.push_back(lh.edge_location);
      label_tree_index = lh.parent_label_tree_index;
    }
    nondominated_paths.push_back(path);
  }

  return Solutions{
    nondominated_paths, std::vector<State>(nondominated_target_states.begin(), nondominated_target_states.end())
  };
}

} // namespace perf_rcsp
