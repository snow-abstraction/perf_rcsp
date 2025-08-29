# perf_rcsp

Copyright (C) 2025 Douglas Wayne Potter

This *Performance - Resource Constrained Shortest Path* project explores the performance (latency)
of [boost graphs rscp](https://www.boost.org/doc/libs/1_88_0/libs/graph/doc/r_c_shortest_paths.html) versus
a custom implementation. This comparison is limited to dummy problems that are representative of more
complex problems of interest to me (and maybe you). I am interested if easy speed-ups are possible.
I am also interested in creating my own simpler implementation (versus boost graph) since then I could
implement problem specific performance improvements.

The implementation tries to avoid some of the indirection and pointer chasing found the boost
implementation by storing all the labels (and states) in contiguous memory for each node.

I would also like to thank the authors of the boost algorithm for providing a correct reference.
That I manage to achieve better performance for instances for a RCSP problem my own choosing is no
surprise. It is almost always easy to achieve better performance given problem and instance specifics and specific
hardware.

## This project is licensed under the GNU Affero General Public License v3.0

See the [LICENSE](./LICENSE) file for details.

## Notes

1. I'll use "route generation" for a simple VRP as my dummy problem for RCSP. Some aspects will be naive because those
   are not the aspects I want to explore in this project.

### Development notes:

1. If you add CMake Profile to be built in CLion, you need to go to the vcpkg menu, choose edit and check the added
   CMake profile in "Add Vcpkg integration to existing CMake profiles". Otherwise, CMake won't find the packages
   such as `boost-graph`, `gtest`, etc.
2. Linux commands for CPU frequency scaling:
   ```shell
   cpupower frequency-set --governor performance
   cpupower frequency-set --governor powersave
   cpupower frequency-info -o proc
   ```
3. Setup jupyter-lab via
   ```shell
   pipx install jupyterlab
   pipx inject jupyterlab pandas jupyter-ruff matplotlib
   ```
