# Ariadne Geometry

[![macOS Status](https://github.com/ariadne-cps/geometry/actions/workflows/macos.yml/badge.svg)](https://github.com/ariadne-cps/geometry/actions/workflows/macos.yml)
[![Ubuntu Status](https://github.com/ariadne-cps/geometry/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/ariadne-cps/geometry/actions/workflows/ubuntu.yml)
[![Windows Status](https://github.com/ariadne-cps/geometry/actions/workflows/win.yml/badge.svg)](https://github.com/ariadne-cps/geometry/actions/workflows/win.yml)
[![Sanitizers Status](https://github.com/ariadne-cps/geometry/actions/workflows/sanitizers.yml/badge.svg)](https://github.com/ariadne-cps/geometry/actions/workflows/sanitizers.yml)
[![Coverage Status](https://github.com/ariadne-cps/geometry/actions/workflows/coverage.yml/badge.svg)](https://github.com/ariadne-cps/geometry/actions/workflows/coverage.yml)
[![codecov](https://codecov.io/gh/ariadne-cps/geometry/branch/main/graph/badge.svg)](https://codecov.io/gh/ariadne-cps/geometry)

Standalone C++20 geometry layer of Ariadne: points, boxes, geometric set interfaces and wrappers, validated predicates and finite unions of intervals.

## Dependencies

Geometry depends directly on [ariadne-cps/algebra](https://github.com/ariadne-cps/algebra), and on [ariadne-cps/configuration](https://github.com/ariadne-cps/configuration) for shared build configuration. Algebra provides Interval, Numeric and other lower libraries transitively.

## Build and test

```sh
git clone --recurse-submodules https://github.com/ariadne-cps/geometry.git
cd geometry
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel --target tests
ctest --test-dir build --output-on-failure
```

## Install

```sh
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --parallel --target ariadne-geometry bindings
cmake --install build-release --prefix /path/to/install
```

The C++ library installs as `ariadne-geometry` together with the public headers and dependency bundle. With Python >= 3.11 and its development module, the build also creates an installable `pyariadne` extension containing Geometry and the lower-level Algebra/Interval/Numeric bindings.

## Tutorials

- C++: `tutorials/tutorial_geometry/tutorial_geometry.cpp` (build separately against an installed Geometry library).
- Python: `python/tutorials/tutorial_geometry.py`.

## License

GNU General Public License v3.0 (or later where individual source files specify it).
