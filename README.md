# nmath

A header-only library of a bunch of small math, geometry and vectorization stuff that I find myself having to constantly copy over or rewrite for new projects.
Additionally, nmath is unencumbered by certain slightly annoying licenses which unfortunately seem to plague good SIMD libraries out there in particular, and it has no external dependencies.

Some parts are practically just wrappers for STL or CRT functions, but I still prefer having them aliased and collected in one normalized place where the implementations can be easily switched out with no changes to the calling application.

nmath is licensed under the MIT license.

nmath's SIMD functionality does not use AVX512 instructions, as the end-user hardware adoption for those is way too poor to make sense in any general use.