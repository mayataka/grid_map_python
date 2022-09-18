#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>

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
  py::enum_<BufferRegion::Quadrant>(buffer_region, "Quadrant", py::arithmetic())
    .value("Undefined", BufferRegion::Quadrant::Undefined)
    .value("TopLeft", BufferRegion::Quadrant::TopLeft)
    .value("TopRight", BufferRegion::Quadrant::TopRight)
    .value("BottomLeft", BufferRegion::Quadrant::BottomLeft)
    .value("BottomRight", BufferRegion::Quadrant::BottomRight)// 
    .export_values();
  buffer_region
    .def(py::init<Index, Size, BufferRegion::Quadrant>(),
         py::arg("startIndex"), py::arg("size"), py::arg("quadrant"))
    .def(py::init<>())
    .def("clone", [](const BufferRegion& self) {
      auto cloned = self;
      return cloned;
    })
    .def("getStartIndex", &BufferRegion::getStartIndex)
    .def("setStartIndex", &BufferRegion::setStartIndex,
          py::arg("startIndex"))
    .def("getSize", &BufferRegion::getSize)
    .def("setSize", &BufferRegion::setSize,
          py::arg("size"))
    .def("getQuadrant", &BufferRegion::getQuadrant)
    .def("setQuadrant", &BufferRegion::setQuadrant,
         py::arg("type"));

  py::class_<SubmapGeometry>(m, "SubmapGeometry")
    .def(py::init<const GridMap&, const Position&, const Length&, bool&>(),
         py::arg("gridMap"), py::arg("position"), py::arg("length"), py::arg("isSuccess"))
    .def("clone", [](const SubmapGeometry& self) {
      auto cloned = self;
      return cloned;
    })
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
      }, py::arg("layer"))
    .def("__setitem__", [](GridMap& self, const std::string layer, const Matrix& item) {
        self[layer] = item;
      }, py::arg("layer"), py::arg("item"))
    .def("erase", &GridMap::erase,
         py::arg("layer"))
    .def("getLayers", &GridMap::getLayers)
    .def("setBasicLayers", &GridMap::setBasicLayers,
         py::arg("basicLayers"))
    .def("getBasicLayers", &GridMap::getBasicLayers)
    .def("hasBasicLayers", &GridMap::hasBasicLayers)
    .def("hasSameLayers", &GridMap::hasSameLayers,
         py::arg("other"))
    .def("getValueAtPosition", [](const GridMap& self, const std::string& layer, const Position& position, InterpolationMethods interpolationMethods) {
       return self.atPosition(layer, position, interpolationMethods);
     }, py::arg("layer"), py::arg("position"), py::arg("interpolationMethods"))
    .def("setValueAtPosition", [](GridMap& self, const std::string& layer, const Position& position, float value) {
       self.atPosition(layer, position) = value;
     }, py::arg("layer"), py::arg("position"), py::arg("value"))
    .def("getValueAt", [](const GridMap& self, const std::string& layer, const Index& index) {
       return self.at(layer, index);
     }, py::arg("layer"), py::arg("index"))
    .def("setValueAt", [](GridMap& self, const std::string& layer, const Index& index, float value) {
       self.at(layer, index) = value;
     }, py::arg("layer"), py::arg("index"), py::arg("value"))
    .def("getIndex", [](const GridMap& self, const Position& position) {
       Index index;
       const bool success = self.getIndex(position, index);
       return std::make_tuple(success, index);
     })
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

  #define DEFINE_GRID_MAP_ITERATOR_BASICS(NAME) \
    .def(py::self != py::self) \
    .def("getIndex", [](const NAME& self) { \
       return *self; \
    }) \
    .def("increment", [](NAME& self) { \
       return (++self); \
    }) \
    .def("isPastEnd", &NAME::isPastEnd) \

  py::class_<CircleIterator>(m, "CircleIterator")
    .def(py::init<const GridMap&, const Position&, double>(),
         py::arg("gridMap"), py::arg("center"), py::arg("radius"))
    DEFINE_GRID_MAP_ITERATOR_BASICS(CircleIterator);

  py::class_<EllipseIterator>(m, "EllipseIterator")
    .def(py::init<const GridMap&, const Position&, const Length&, double>(),
         py::arg("gridMap"), py::arg("center"), py::arg("length"), py::arg("rotation"))
    DEFINE_GRID_MAP_ITERATOR_BASICS(EllipseIterator)
    .def("getSubmapSize", &EllipseIterator::getSubmapSize);

  py::class_<GridMapIterator>(m, "GridMapIterator")
    .def(py::init<const GridMap&>(),
         py::arg("gridMap"))
    DEFINE_GRID_MAP_ITERATOR_BASICS(GridMapIterator)
    .def("clone", [](const GridMapIterator& self) {
      auto cloned = self;
      return cloned;
    })
    .def("getLinearIndex", &GridMapIterator::getLinearIndex)
    .def("getUnwrappedIndex", &GridMapIterator::getUnwrappedIndex)
    .def("end", &GridMapIterator::end);

  py::class_<LineIterator>(m, "LineIterator")
    .def(py::init<const GridMap&, const Position&, const Position>(),
         py::arg("gridMap"), py::arg("start"), py::arg("end"))
    .def(py::init<const GridMap&, const Index&, const Index>(),
         py::arg("gridMap"), py::arg("start"), py::arg("end"))
    DEFINE_GRID_MAP_ITERATOR_BASICS(LineIterator);

  // TODO: add Polygon and PolygonIterator

  py::class_<SlidingWindowIterator> sliding_window_iterator(m, "SlidingWindowIterator");
  py::enum_<SlidingWindowIterator::EdgeHandling>(sliding_window_iterator, "EdgeHandling", py::arithmetic())
    .value("INSIDE", SlidingWindowIterator::EdgeHandling::INSIDE)
    .value("CROP", SlidingWindowIterator::EdgeHandling::CROP)
    .value("EMPTY", SlidingWindowIterator::EdgeHandling::EMPTY)
    .value("MEAN", SlidingWindowIterator::EdgeHandling::MEAN)
    .export_values();
  sliding_window_iterator
    .def(py::init<const GridMap&, const std::string&, const SlidingWindowIterator::EdgeHandling&, size_t>(),
         py::arg("gridMap"), py::arg("layer"), 
         py::arg("edgeHandling")=SlidingWindowIterator::EdgeHandling::CROP, 
         py::arg("windowSize")=3)
    .def("clone", [](const SlidingWindowIterator& self) {
      auto cloned = self;
      return cloned;
    })
    .def("setWindowLength", &SlidingWindowIterator::setWindowLength,
          py::arg("gridMap"), py::arg("windowLength"))
    .def("increment", [](SlidingWindowIterator& self) { 
       return (++self); 
    }) 
    .def("getData", &SlidingWindowIterator::getData);

  py::class_<SpiralIterator>(m, "SpiralIterator")
    .def(py::init<const GridMap&, Eigen::Vector2d, double>(),
         py::arg("gridMap"), py::arg("center"), py::arg("radius"))
    DEFINE_GRID_MAP_ITERATOR_BASICS(SpiralIterator)
    .def("getCurrentRadius", &SpiralIterator::getCurrentRadius);

  py::class_<SubmapIterator>(m, "SubmapIterator")
    .def(py::init<const SubmapGeometry&>(),
         py::arg("submap"))
    .def(py::init<const GridMap&, const BufferRegion&>(),
         py::arg("gridMap"), py::arg("bufferRegion"))
    .def(py::init<const GridMap&, const Index&, const Size&>(),
         py::arg("gridMap"), py::arg("submapStartIndex"), py::arg("submapSize"))
    DEFINE_GRID_MAP_ITERATOR_BASICS(SubmapIterator)
    .def("clone", [](const SubmapIterator& self) {
      auto cloned = self;
      return cloned;
    })
    .def("getSubmapIndex", &SubmapIterator::getSubmapIndex)
    .def("getSubmapSize", &SubmapIterator::getSubmapSize);

}

} // namespace python
} // namespace grid_map