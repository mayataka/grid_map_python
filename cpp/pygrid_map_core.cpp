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
    .def("getPosition", &SubmapGeometry::getPosition)
    .def("getRequestedIndexInSubmap", &SubmapGeometry::getRequestedIndexInSubmap)
    .def("getSize", &SubmapGeometry::getSize)
    .def("getResolution", &SubmapGeometry::getResolution)
    .def("getStartIndex", &SubmapGeometry::getStartIndex);

  py::class_<GridMap>(m, "GridMap")
    .def(py::init<const std::vector<std::string>&>(),
         py::arg("layers"))
    .def(py::init<>())
    .def("exists", &GridMap::exists,
         py::arg("layer"));
}

} // namespace python
} // namespace grid_map