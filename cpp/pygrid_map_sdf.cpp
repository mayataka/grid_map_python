#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>

#include "grid_map_sdf/SignedDistanceField.hpp"

namespace grid_map {
namespace signed_distance_field {
namespace python {

namespace py = pybind11;

PYBIND11_MODULE(pygrid_map_sdf, m) {
  py::class_<SignedDistanceField>(m, "SignedDistanceField")
    .def(py::init<>())
    .def("calculateSignedDistanceField", &SignedDistanceField::calculateSignedDistanceField,
          py::arg("gridMap"), py::arg("layer"), py::arg("heightClearance"))
    .def("getDistanceAt", &SignedDistanceField::getDistanceAt, 
          py::arg("position"))
    .def("getDistanceGradientAt", &SignedDistanceField::getDistanceGradientAt, 
          py::arg("position"))
    .def("getInterpolatedDistanceAt", &SignedDistanceField::getInterpolatedDistanceAt, 
          py::arg("position"));
//     .def("convertToPointCloud", &SignedDistanceField::convertToPointCloud, 
//           py::arg("points"));
}

} // namespace python
} // namespace signed_distance_field
} // namespace grid_map