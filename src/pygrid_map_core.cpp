#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>

#include "grid_map_core/grid_map_core.hpp"

namespace grid_map {
namespace python {

namespace py = pybind11;

PYBIND11_MODULE(pygrid_map_core, m) {
  py::class_<SubmapGeometry>(m, "SubmapGeometry")
    .def(py::init<const GridMap&, const Position&, const Length&, bool&>(),
         py::arg("gridMap"), py::arg("position"), py::arg("length"), py::arg("isSuccess"))
    .def("getGridMap", &SubmapGeometry::getGridMap)
    .def("getLength", &SubmapGeometry::getLength)
    .def("getStartIndex", &SubmapGeometry::getStartIndex);
}

} // namespace python
} // namespace grid_map