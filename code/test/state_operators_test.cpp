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

#include "../../code/src/vrp_model.h"

#include <gtest/gtest.h>

using namespace perf_rcsp;

TEST(state, equality) {
  State s1{.cost = 2, .time = 3, .energy = 5, .delivered = {}};
  s1.delivered.set(1);
  State s2{.cost = 2, .time = 3, .energy = 5, .delivered = {}};
  s2.delivered.set(1);
  ASSERT_EQ(s1, s2);
  s2.delivered.set(0);
  ASSERT_NE(s1, s2);
}

TEST(state, less_than_or_equal__identical) {
  State s1{.cost = 2, .time = 3, .energy = 5, .delivered = {}};
  State s2{.cost = 2, .time = 3, .energy = 5, .delivered = {}};
  ASSERT_LE(s1, s2);
  ASSERT_LE(s2, s1);
}

TEST(state, less_than_or_equal__cost_does_not_matter) {
  State s1{.cost = 2, .time = 3, .energy = 5, .delivered = {}};
  State s2{.cost = 10, .time = 3, .energy = 5, .delivered = {}};
  ASSERT_LE(s1, s2);
  s2.cost = 2;
  s1.cost = 10;
  ASSERT_LE(s1, s2);
}

TEST(state, greater_than) {
  State s1{.cost = 10, .time = 3, .energy = 5, .delivered = {}};
  State s2{.cost = 2, .time = 3, .energy = 5, .delivered = {}};
  ASSERT_LE(s1, s2);
  s1.time = 5;
  ASSERT_GT(s1, s2);
}
