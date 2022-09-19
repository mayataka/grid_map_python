#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>

#include "grid_map_pcl/GridMapPclConverter.hpp"
#include "grid_map_sdf/SignedDistanceField.hpp"

namespace grid_map {
namespace python {

namespace py = pybind11;

PYBIND11_MODULE(pcl, m) {
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

  py::class_<pcl::PointXYZI>(m, "PointXYZI")
    .def(py::init<float>(),
          py::arg("intensity")=0.0)
    .def("clone", [](const pcl::PointXYZI& self) {
      auto cloned = self;
      return cloned;
    })
    .def_readwrite("x", &pcl::PointXYZI::x)
    .def_readwrite("y", &pcl::PointXYZI::y)
    .def_readwrite("z", &pcl::PointXYZI::z)
    .def_readwrite("intensity", &pcl::PointXYZI::intensity)
    .def("__str__", [](const pcl::PointXYZI& self) {
      std::stringstream ss;
      ss << self;
      return ss.str();
    });

  py::class_<pcl::PointCloud<pcl::PointXYZI>>(m, "PointCouldPointXYZI")
    .def(py::init<uint32_t, uint32_t, const pcl::PointXYZI&>(),
         py::arg("width"), py::arg("height"), py::arg("value")=pcl::PointXYZI())
    .def(py::init<>())
    .def("clone", [](const pcl::PointCloud<pcl::PointXYZI>& self) {
      auto cloned = self;
      return cloned;
    })
    .def(py::self += py::self) 
    .def("__add__", [](pcl::PointCloud<pcl::PointXYZI>& c1, const pcl::PointCloud<pcl::PointXYZI>& c2) { return c1 + c2; })
    .def("__radd__", [](pcl::PointCloud<pcl::PointXYZI>& c1, const pcl::PointCloud<pcl::PointXYZI>& c2) { return c1 + c2; })
    .def("getValueAt", [](const pcl::PointCloud<pcl::PointXYZI>& self, int column, int row) {
       return self.at(column, row);
     }, py::arg("column"), py::arg("row"))
    .def("setValueAt", [](pcl::PointCloud<pcl::PointXYZI>& self, int column, int row, const pcl::PointXYZI& value) {
       self.at(column, row) = value;
     }, py::arg("column"), py::arg("row"), py::arg("value"))
    .def("__getitem__", [](const pcl::PointCloud<pcl::PointXYZI>& self, int column, int row) {
       return self(column, row);
     }, py::arg("column"), py::arg("row"))
    .def("__setitem__", [](pcl::PointCloud<pcl::PointXYZI>& self, int column, int row, const pcl::PointXYZI& item) {
       self(column, row) = item;
     }, py::arg("column"), py::arg("row"), py::arg("item"))
    .def("isOrganized", &pcl::PointCloud<pcl::PointXYZI>::isOrganized)
    .def("getMatrixXfMap", [](const pcl::PointCloud<pcl::PointXYZI>& self, int dim, int stride, int offset) {
      return self.getMatrixXfMap(dim, stride, offset);
     }, py::arg("dim"), py::arg("stride"), py::arg("offset"))
    .def("setMatrixXfMap", [](pcl::PointCloud<pcl::PointXYZI>& self, int dim, int stride, int offset, const Eigen::MatrixXf& value) {
      self.getMatrixXfMap(dim, stride, offset) = value;
     }, py::arg("dim"), py::arg("stride"), py::arg("offset"), py::arg("value"))
    .def("getMatrixXfMap", [](const pcl::PointCloud<pcl::PointXYZI>& self) {
      return self.getMatrixXfMap();
     })
    .def("setMatrixXfMap", [](pcl::PointCloud<pcl::PointXYZI>& self, const Eigen::MatrixXf& value) {
      self.getMatrixXfMap() = value;
     })
    .def("size", &pcl::PointCloud<pcl::PointXYZI>::size)
    .def("reserve", &pcl::PointCloud<pcl::PointXYZI>::reserve,
         py::arg("size"))
    .def("empty", &pcl::PointCloud<pcl::PointXYZI>::empty)
    .def("resisze", &pcl::PointCloud<pcl::PointXYZI>::resize,
         py::arg("size"))
    .def("getValueAt", [](const pcl::PointCloud<pcl::PointXYZI>& self, int n) {
       return self.at(n);
     }, py::arg("n"))
    .def("setValueAt", [](pcl::PointCloud<pcl::PointXYZI>& self, int n, const pcl::PointXYZI& value) {
       self.at(n) = value;
     }, py::arg("n"), py::arg("value"))
    .def("__getitem__", [](const pcl::PointCloud<pcl::PointXYZI>& self, int n) {
       return self[n];
     }, py::arg("n"))
    .def("__setitem__", [](pcl::PointCloud<pcl::PointXYZI>& self, int n, const pcl::PointXYZI& item) {
       self[n] = item;
     }, py::arg("n"), py::arg("item"))
    .def("push_back", &pcl::PointCloud<pcl::PointXYZI>::push_back,
         py::arg("pt"))
    .def("clear", &pcl::PointCloud<pcl::PointXYZI>::clear)
    .def("__str__", [](const pcl::PointXYZ& self) {
      std::stringstream ss;
      ss << self;
      return ss.str();
    });
}

} // namespace python
} // namespace grid_map