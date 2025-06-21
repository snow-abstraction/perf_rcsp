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

#ifndef UTIL_H
#define UTIL_H

#include <cstdlib>
#include <spdlog/spdlog.h>

#define ASSERT_ALWAYS(expr)                                                                                            \
  do {                                                                                                                 \
    if (!(expr)) [[unlikely]] {                                                                                        \
      spdlog::critical("Assertion failed: [{}]\n\tWhere: {}:{}", #expr, __FILE__, __LINE__);                           \
      std::abort();                                                                                                    \
    }                                                                                                                  \
  } while (0)

#endif // UTIL_H
