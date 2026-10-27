#include "pybind11.hpp"

void geometry_submodule(pybind11::module& module);

void geometry_module(pybind11::module& module) {
    geometry_submodule(module);
}
