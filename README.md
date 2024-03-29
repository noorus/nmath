# nmath

A header-only library of a bunch of small math, geometry and vectorization stuff that I find myself having to constantly copy over or rewrite for new projects.
Additionally, nmath is unencumbered by certain slightly annoying licenses which unfortunately seem to plague good SIMD libraries out there in particular, and it has no external dependencies.

Some parts are practically just wrappers for STL or CRT functions, but I still prefer having them aliased and collected in one normalized place where the implementations can be easily switched out with no changes to the calling application.

nmath is licensed under the MIT license.

nmath's SIMD functionality does not use AVX512 instructions, as the end-user hardware adoption for those is way too poor to make sense in any general use.

### nmath currently implements
- `vec4f` 128-bit 4x float SIMD vector
- `vec8f` 256-bit 8x float SIMD vector
- `vec4d` 256-bit 4x double SIMD vector

### common members
- construct from aligned or unaligned mem values, given scalars or default to zero
- temporal & nontemporal load & store
- plus, minus, and, or, xor, multiply, divide, equality, inequality operators
- componentwise round, ceil, floor, trunc
- componentwise sqrt
- sum of elements
- fused multiply & add, fused multiply & sub
