# perf_rcsp
Copyright (C) 2025 Douglas Wayne Potter

This *Performance - Resource Constrained Shortest Path* project explores the performance (latency)
of [boost graphs rscp](https://www.boost.org/doc/libs/1_88_0/libs/graph/doc/r_c_shortest_paths.html) versus
a custom implementation. This comparison is limited to dummy problems that are representative of more
complex problems of interest to me (and maybe you). I am interested in both if any easy speed-ups
are possible on the CPU and understanding if simple GPU (e.g. CUDA) implementations are possible.

## This project is licensed under the GNU Affero General Public License v3.0

See the [LICENSE](./LICENSE) file for details.

## Notes

1. I'll use "route generation" for a simple VRP as my dummy problem for RCSP. Some aspects will be naive because those
   are not the aspects I want to explore in this project.
