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
  py::class_<pcl::PCLHeader>(m, "PCLHeader")
    .def(py::init<>())
    .def_readwrite("seq", &pcl::PCLHeader::seq)
    .def_readwrite("stamp", &pcl::PCLHeader::stamp)
    .def_readwrite("frame_id", &pcl::PCLHeader::frame_id);

  py::class_<pcl::PCLPointField>(m, "PCLPointField")
    .def(py::init<>())
    .def_readwrite("name", &pcl::PCLPointField::name)
    .def_readwrite("offset", &pcl::PCLPointField::offset)
    .def_readwrite("datatype", &pcl::PCLPointField::datatype)
    .def_readwrite("count", &pcl::PCLPointField::count);

  py::class_<pcl::PCLPointCloud2>(m, "PCLPointCloud2")
    .def(py::init<>())
    .def(py::self += py::self) 
    .def("__add__", [](pcl::PCLPointCloud2& c1, const pcl::PCLPointCloud2& c2) { return c1 + c2; })
    .def("__radd__", [](pcl::PCLPointCloud2& c1, const pcl::PCLPointCloud2& c2) { return c1 + c2; })
    .def_readwrite("header", &pcl::PCLPointCloud2::header)
    .def_readwrite("height", &pcl::PCLPointCloud2::height)
    .def_readwrite("width", &pcl::PCLPointCloud2::width)
    .def_readwrite("fields", &pcl::PCLPointCloud2::fields)
    .def_readwrite("is_bigendian", &pcl::PCLPointCloud2::is_bigendian)
    .def_readwrite("point_step", &pcl::PCLPointCloud2::point_step)
    .def_readwrite("row_step", &pcl::PCLPointCloud2::row_step)
    .def_readwrite("data", &pcl::PCLPointCloud2::data)
    .def_readwrite("is_dense", &pcl::PCLPointCloud2::is_dense)
    .def_static("concatenateInPlace", [](pcl::PCLPointCloud2& c1, const pcl::PCLPointCloud2& c2) {
        return pcl::PCLPointCloud2::concatenate(c1, c2);
     }, py::arg("c1"), py::arg("c2"))
    .def_static("concatenate", [](const pcl::PCLPointCloud2& c1, const pcl::PCLPointCloud2& c2) {
        pcl::PCLPointCloud2 cout;
        const bool success = pcl::PCLPointCloud2::concatenate(c1, c2, cout);
        return std::make_tuple(success, cout);
     }, py::arg("c1"), py::arg("c2"));

  py::class_<pcl::Vertices>(m, "Vertices")
    .def(py::init<>())
    .def_readwrite("vertices", &pcl::Vertices::vertices);

  py::class_<pcl::PolygonMesh>(m, "PolygonMesh")
    .def(py::init<>())
    .def(py::self += py::self) 
    .def("__add__", [](pcl::PolygonMesh& m1, const pcl::PolygonMesh& m2) { return m1 + m2; })
    .def("__radd__", [](pcl::PolygonMesh& m1, const pcl::PolygonMesh& m2) { return m1 + m2; })
    .def_readwrite("header", &pcl::PolygonMesh::header)
    .def_readwrite("cloud", &pcl::PolygonMesh::cloud)
    .def_readwrite("polygons", &pcl::PolygonMesh::polygons);

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