#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>

#include "grid_map_pcl/GridMapPclConverter.hpp"

namespace grid_map {
namespace python {

namespace py = pybind11;

PYBIND11_MODULE(pygrid_map_pcl, m) {
  py::class_<GridMapPclConverter>(m, "GridMapPclConverter")
    .def(py::init<>())
    .def_static("initializeFromPolygonMesh", [](const pcl::PolygonMesh& mesh, const double resolution, const grid_map::GridMap& gridMap) {
      auto ret = gridMap;
      const bool success = GridMapPclConverter::initializeFromPolygonMesh(mesh, resolution, ret);
      return std::make_tuple(success, ret);
    })
    .def_static("addLayerFromPolygonMesh", [](const pcl::PolygonMesh& mesh, const std::string& lay, const grid_map::GridMap& gridMap) {
      auto ret = gridMap;
      const bool success = GridMapPclConverter::addLayerFromPolygonMesh(mesh, lay, ret);
      return std::make_tuple(success, ret);
    });
}

} // namespace python
} // namespace grid_map