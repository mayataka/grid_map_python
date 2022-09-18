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
    .def("setGeometry", 
          static_cast<void (GridMap::*)(const Length& length, const double, const Position&)>(&GridMap::setGeometry),
          py::arg("length"), py::arg("resolution"), py::arg("position")=Position::Zero())
    .def("setGeometry", 
          static_cast<void (GridMap::*)(const SubmapGeometry&)>(&GridMap::setGeometry),
          py::arg("geometry"))
    .def("add", 
          static_cast<void (GridMap::*)(const std::string&, const double)>(&GridMap::add),
          py::arg("layer"), py::arg("value")=NAN)
    .def("add", 
          static_cast<void (GridMap::*)(const std::string&, const Matrix&)>(&GridMap::add),
          py::arg("layer"), py::arg("data"))
    .def("exists", &GridMap::exists,
         py::arg("layer"))
    .def("get", 
          static_cast<const Matrix& (GridMap::*)(const std::string&) const>(&GridMap::get),
          py::arg("layer"))
    .def("get", 
          static_cast<Matrix& (GridMap::*)(const std::string&)>(&GridMap::get),
          py::arg("layer"))
    .def("__getitem__", [](const GridMap& self, const std::string& layer) {
        return self[layer];
      })
    .def("__setitem__", [](GridMap& self, const std::string layer, const Matrix& item) {
        self[layer] = item;
      })
    .def("erase", &GridMap::erase,
         py::arg("layer"))
    .def("getLayers", &GridMap::getLayers)
    .def("setBasicLayers", &GridMap::setBasicLayers,
         py::arg("basicLayers"))
    .def("getBasicLayers", &GridMap::getBasicLayers)
    .def("hasBasicLayers", &GridMap::hasBasicLayers)
    .def("hasSameLayers", &GridMap::hasSameLayers,
         py::arg("other"));
}

} // namespace python
} // namespace grid_map