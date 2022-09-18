#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>

#include "grid_map_sdf/SignedDistanceField.hpp"
#include "grid_map_sdf/Gridmap3dLookup.hpp"

namespace grid_map {
namespace signed_distance_field {
namespace python {

namespace py = pybind11;

PYBIND11_MODULE(pygrid_map_sdf, m) {
  py::class_<Gridmap3dLookup> grid_map_3d_lookup(m, "Gridmap3dLookup");
  py::class_<Gridmap3dLookup::size_t_3d>(grid_map_3d_lookup, "size_t_3d")
    .def(py::init<size_t, size_t, size_t>(),
         py::arg("x"), py::arg("y"), py::arg("z"))
    .def(py::init<>())
    .def_readwrite("x", &Gridmap3dLookup::size_t_3d::x)
    .def_readwrite("y", &Gridmap3dLookup::size_t_3d::y)
    .def_readwrite("z", &Gridmap3dLookup::size_t_3d::z);
  grid_map_3d_lookup 
    .def(py::init<const Gridmap3dLookup::size_t_3d&, const Position3&, double>(),
         py::arg("gridSize"), py::arg("gridOrigin"), py::arg("resolution"))
    .def(py::init<>())
    .def_readwrite("gridsize_", &Gridmap3dLookup::gridsize_)
    .def_readwrite("gridOrigin_", &Gridmap3dLookup::gridOrigin_)
    .def_readwrite("gridMaxIndexAsDouble_", &Gridmap3dLookup::gridMaxIndexAsDouble_)
    .def_readwrite("resolution_", &Gridmap3dLookup::resolution_)
    .def("nearestNode", &Gridmap3dLookup::nearestNode,
          py::arg("position"))
    .def("nodePosition", &Gridmap3dLookup::nodePosition,
          py::arg("index"))
    .def("linearIndex", &Gridmap3dLookup::linearIndex,
          py::arg("index"))
    .def("linearSize", &Gridmap3dLookup::linearSize)
    .def_static("getNearestPositiveInteger", &Gridmap3dLookup::getNearestPositiveInteger,
          py::arg("val"), py::arg("max"));

  py::class_<SignedDistanceField>(m, "SignedDistanceField")
    .def(py::init<const GridMap&, const std::string&, double, double>(),
         py::arg("gridMap"), py::arg("elevationLayer"), py::arg("minHeight"), py::arg("maxHeight"))
    .def("value", &SignedDistanceField::value,
          py::arg("position"))
    .def("derivative", [](const SignedDistanceField& self, const Position3& position) {
       return self.derivative(position);
     }, py::arg("position"))
    .def("valueAndDerivative", &SignedDistanceField::valueAndDerivative,
          py::arg("position"))
    .def("size", &SignedDistanceField::size)
    .def("getFrameId", &SignedDistanceField::getFrameId)
    .def("getTime", &SignedDistanceField::getTime)
    .def("filterPoints", &SignedDistanceField::filterPoints,
          py::arg("func"), py::arg("decimation") = -1);
}

} // namespace python
} // namespace signed_distance_field
} // namespace grid_map