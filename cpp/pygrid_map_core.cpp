#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>

#include "grid_map_core/grid_map_core.hpp"

namespace grid_map {
namespace python {

namespace py = pybind11;

PYBIND11_MODULE(pygrid_map_core, m) {
  py::enum_<InterpolationMethods>(m, "InterpolationMethods", py::arithmetic())
    .value("INTER_NEAREST", InterpolationMethods::INTER_NEAREST)
    .value("INTER_LINEAR", InterpolationMethods::INTER_LINEAR)
    .value("INTER_CUBIC_CONVOLUTION", InterpolationMethods::INTER_CUBIC_CONVOLUTION)
    .value("INTER_CUBIC", InterpolationMethods::INTER_CUBIC)
    .export_values();

  py::class_<BufferRegion> buffer_region(m, "BufferRegion");
  buffer_region
    .def(py::init<Index, Size, BufferRegion::Quadrant>(),
         py::arg("startIndex"), py::arg("size"), py::arg("quadrant"))
    .def(py::init<>())
    .def("getStartIndex", &BufferRegion::getStartIndex)
    .def("setStartIndex", &BufferRegion::setStartIndex,
          py::arg("startIndex"))
    .def("getSize", &BufferRegion::getSize)
    .def("setSize", &BufferRegion::setSize,
          py::arg("size"))
    .def("getQuadrant", &BufferRegion::getQuadrant)
    .def("setQuadrant", &BufferRegion::setQuadrant,
         py::arg("type"));
  py::enum_<BufferRegion::Quadrant>(buffer_region, "Quadrant", py::arithmetic())
    .value("Undefined", BufferRegion::Quadrant::Undefined)
    .value("TopLeft", BufferRegion::Quadrant::TopLeft)
    .value("TopRight", BufferRegion::Quadrant::TopRight)
    .value("BottomLeft", BufferRegion::Quadrant::BottomLeft)
    .value("BottomRight", BufferRegion::Quadrant::BottomRight)// 
    .export_values();

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
    .def("clone", [](const GridMap& self) {
      auto cloned = self;
      return cloned;
    })
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
         py::arg("other"))
    .def("atPosition", 
          static_cast<float& (GridMap::*)(const std::string&, const Position&)>(&GridMap::atPosition),
          py::arg("layer"), py::arg("position"))
    .def("atPosition", 
          static_cast<float (GridMap::*)(const std::string&, const Position&, InterpolationMethods) const>(&GridMap::atPosition),
          py::arg("layer"), py::arg("position"), py::arg("interpolationMethods")=InterpolationMethods::INTER_NEAREST)
    .def("at", 
          static_cast<float& (GridMap::*)(const std::string&, const Index&)>(&GridMap::at),
          py::arg("layer"), py::arg("index"))
    .def("at", 
          static_cast<float (GridMap::*)(const std::string&, const Index&) const>(&GridMap::at),
          py::arg("layer"), py::arg("index"))
    .def("getIndex", [](const GridMap& self, const Position& position) {
       Index index;
       const bool success = self.getIndex(position, index);
       return std::make_tuple(success, index);
     })
//     .def("getPosition", &GridMap::getPosition,
//          py::arg("layer"), py::arg("position"))
    .def("isInside", &GridMap::isInside,
         py::arg("position"))
    .def("isValid", 
          static_cast<bool (GridMap::*)(const Index&) const>(&GridMap::isValid),
         py::arg("index"))
    .def("isValid", 
          static_cast<bool (GridMap::*)(const Index&, const std::string&) const>(&GridMap::isValid),
         py::arg("index"), py::arg("layer"))
    .def("getPosition3", [](const GridMap& self, const std::string& layer, const Index& index) {
       Position3 position;
       const bool success = self.getPosition3(layer, index, position);
       return std::make_tuple(success, position);
     }, py::arg("layer"), py::arg("index"))
    .def("getVector", [](const GridMap& self, const std::string& layerPrefix, const Index& index) {
       Eigen::Vector3d vector;
       const bool success = self.getVector(layerPrefix, index, vector);
       return std::make_tuple(success, vector);
     }, py::arg("layerPrefix"), py::arg("index"))
    .def("getSubmap", [](const GridMap& self, const Position& position, const Length& length) {
       bool success = false;
       auto submap = self.getSubmap(position, length, success);
       return std::make_tuple(success, submap);
     }, py::arg("position"), py::arg("length"))
    .def("getSubmap", [](const GridMap& self, const Position& position, const Length& length, const Index& indexInSubmap) {
       bool success = false;
       auto indexInSubmapRet = indexInSubmap;
       auto submap = self.getSubmap(position, length, indexInSubmapRet, success);
       return std::make_tuple(success, indexInSubmapRet, submap);
     }, py::arg("position"), py::arg("length"), py::arg("indexInSubmap"))
    .def("getTransformedMap", &GridMap::getTransformedMap,
         py::arg("transform"), py::arg("heightLayerName"), py::arg("newFrameId"), py::arg("sampleRatio")=0.0)
    .def("setPosition", &GridMap::setPosition,
         py::arg("position"))
    .def("move", 
          static_cast<bool (GridMap::*)(const Position&, std::vector<BufferRegion>&)>(&GridMap::move),
          py::arg("position"), py::arg("bufferRegion"))
    .def("move", 
          static_cast<bool (GridMap::*)(const Position&)>(&GridMap::move),
          py::arg("position"))
    .def("addDataFrom", &GridMap::addDataFrom,
         py::arg("other"), py::arg("extendMap"), py::arg("overwiteData"), py::arg("copyAllLayers"), 
         py::arg("layers")=std::vector<std::string>())
    .def("extendToInclude", &GridMap::extendToInclude,
         py::arg("other"))
    .def("clear", &GridMap::clear,
         py::arg("layer"))
    .def("clearBasic", &GridMap::clearBasic)
    .def("clearAll", &GridMap::clearAll)
    .def("setTimestamp", &GridMap::setTimestamp,
         py::arg("timestamp"))
    .def("getTimestamp", &GridMap::getTimestamp)
    .def("resetTimestamp", &GridMap::resetTimestamp)
    .def("setFrameId", &GridMap::setFrameId,
         py::arg("frameId"))
    .def("getFrameId", &GridMap::getFrameId)
    .def("getLength", &GridMap::getLength)
    .def("getPosition", [](const GridMap& self) {
       return self.getPosition();
     })
    .def("getResolution", &GridMap::getResolution)
    .def("getSize", &GridMap::getSize)
    .def("setStartIndex", &GridMap::setStartIndex,
         py::arg("startIndex"))
    .def("getStartIndex", &GridMap::getStartIndex)
    .def("isDefaultStartIndex", &GridMap::isDefaultStartIndex)
    .def("convertToDefaultStartIndex", &GridMap::convertToDefaultStartIndex)
    .def("getClosestPositionInMap", &GridMap::getClosestPositionInMap,
         py::arg("position"));
}

} // namespace python
} // namespace grid_map