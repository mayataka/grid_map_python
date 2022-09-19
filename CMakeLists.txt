cmake_minimum_required(VERSION 3.11)
project(grid_map_python CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(Eigen3 REQUIRED)
find_package(PCL REQUIRED QUIET COMPONENTS common features filters io segmentation surface)

include(FetchContent)
include(ExternalProject)

# build grid_map libraries
set(GRID_MAP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/thirdparty/grid_map)
set(GRID_MAP_SUBBUILD_DIR ${PROJECT_SOURCE_DIR}/thirdparty/grid_map-build)
FetchContent_GetProperties(grid_map)
if(NOT grid_map_POPULATED)
FetchContent_Populate(
  grid_map
  GIT_REPOSITORY https://github.com/ANYbotics/grid_map
  GIT_TAG        ros2
  SOURCE_DIR ${GRID_MAP_SOURCE_DIR}
  SUBBUILD_DIR ${GRID_MAP_SUBBUILD_DIR}
)
endif()

set(GRID_MAP_CORE_SOURCE_DIR ${GRID_MAP_SOURCE_DIR}/grid_map_core)
include(${GRID_MAP_CORE_SOURCE_DIR}/cmake/grid_map_core-extras.cmake)
add_library(grid_map_core SHARED
  ${GRID_MAP_CORE_SOURCE_DIR}/src/GridMap.cpp
  ${GRID_MAP_CORE_SOURCE_DIR}/src/GridMapMath.cpp
  ${GRID_MAP_CORE_SOURCE_DIR}/src/SubmapGeometry.cpp
  ${GRID_MAP_CORE_SOURCE_DIR}/src/BufferRegion.cpp
  ${GRID_MAP_CORE_SOURCE_DIR}/src/Polygon.cpp
  ${GRID_MAP_CORE_SOURCE_DIR}/src/CubicInterpolation.cpp
  ${GRID_MAP_CORE_SOURCE_DIR}/src/iterators/GridMapIterator.cpp
  ${GRID_MAP_CORE_SOURCE_DIR}/src/iterators/SubmapIterator.cpp
  ${GRID_MAP_CORE_SOURCE_DIR}/src/iterators/CircleIterator.cpp
  ${GRID_MAP_CORE_SOURCE_DIR}/src/iterators/EllipseIterator.cpp
  ${GRID_MAP_CORE_SOURCE_DIR}/src/iterators/SpiralIterator.cpp
  ${GRID_MAP_CORE_SOURCE_DIR}/src/iterators/PolygonIterator.cpp
  ${GRID_MAP_CORE_SOURCE_DIR}/src/iterators/LineIterator.cpp
  ${GRID_MAP_CORE_SOURCE_DIR}/src/iterators/SlidingWindowIterator.cpp
)
target_include_directories(grid_map_core PUBLIC
  ${GRID_MAP_CORE_SOURCE_DIR}/include
  ${EIGEN3_INCLUDE_DIR}
)
set(GRID_MAP_PCL_SOURCE_DIR ${GRID_MAP_SOURCE_DIR}/grid_map_pcl)
add_library(grid_map_pcl SHARED
  ${GRID_MAP_PCL_SOURCE_DIR}/src/GridMapPclConverter.cpp
)
target_include_directories(grid_map_pcl PUBLIC
  ${GRID_MAP_PCL_SOURCE_DIR}/include
  ${GRID_MAP_CORE_SOURCE_DIR}/include
  ${EIGEN3_INCLUDE_DIR}
  ${PCL_INCLUDE_DIRS}
)
target_link_libraries(grid_map_pcl PUBLIC 
  grid_map_core
  ${PCL_LIBRARIES}
)
add_dependencies(grid_map_pcl grid_map_core)
set(GRID_MAP_SDF_SOURCE_DIR ${GRID_MAP_SOURCE_DIR}/grid_map_sdf)
add_library(grid_map_sdf SHARED
  ${GRID_MAP_SDF_SOURCE_DIR}/src/SignedDistanceField.cpp
)
target_include_directories(grid_map_sdf PUBLIC
  ${GRID_MAP_SDF_SOURCE_DIR}/include
  ${GRID_MAP_CORE_SOURCE_DIR}/include
  ${EIGEN3_INCLUDE_DIR}
  ${PCL_INCLUDE_DIRS}
)
target_link_libraries(grid_map_sdf PUBLIC 
  grid_map_core
  ${PCL_LIBRARIES}
)
add_dependencies(grid_map_sdf grid_map_core)

# add pybind11
set(PYBIND11_SOURCE_DIR ${PROJECT_SOURCE_DIR}/thirdparty/pybind11)
set(PYBIND11_SUBBUILD_DIR ${PROJECT_SOURCE_DIR}/thirdparty/pybind11-build)
FetchContent_GetProperties(pybind11)
if(NOT pybind11_POPULATED)
FetchContent_Populate(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git 
  GIT_TAG        master
  SOURCE_DIR ${PYBIND11_SOURCE_DIR}
  SUBBUILD_DIR ${PYBIND11_SUBBUILD_DIR}
)
endif()
add_subdirectory(${PYBIND11_SOURCE_DIR})

set(GRID_MAP_PYTHON_VERSION ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR})
if(NOT DEFINED PYTHON_INSTALL_DIR)
  set(PYTHON_INSTALL_DIR $ENV{HOME}/.local/lib/python${GRID_MAP_PYTHON_VERSION}/site-packages/grid_map)
endif()

macro(pybind11_add_grid_map_module MODULE)
pybind11_add_module(
  py${MODULE}
  SHARED 
  ${PROJECT_SOURCE_DIR}/cpp/py${MODULE}.cpp
)
add_dependencies(py${MODULE} ${MODULE})
endmacro()

macro(pybind11_grid_map_include_directories MODULE)
target_include_directories(
  py${MODULE}
  PRIVATE
  ${EIGEN3_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${GRID_MAP_CORE_SOURCE_DIR}/include
  ${GRID_MAP_PCL_SOURCE_DIR}/include
  ${GRID_MAP_SDF_SOURCE_DIR}/include
)
endmacro()

macro(pybind11_grid_map_link_libraries MODULE)
target_link_libraries(
  py${MODULE}
  PRIVATE
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/lib${MODULE}.so>
  $<INSTALL_INTERFACE:${PYTHON_INSTALL_DIR}/grid_map/lib${MODULE}.so>
)
endmacro()

pybind11_add_grid_map_module(grid_map_core)
pybind11_grid_map_include_directories(grid_map_core)
pybind11_grid_map_link_libraries(grid_map_core)

pybind11_add_grid_map_module(grid_map_pcl)
pybind11_grid_map_include_directories(grid_map_pcl)
pybind11_grid_map_link_libraries(grid_map_pcl)

pybind11_add_grid_map_module(grid_map_sdf)
pybind11_grid_map_include_directories(grid_map_sdf)
pybind11_grid_map_link_libraries(grid_map_sdf)

pybind11_add_module(pcl SHARED ${PROJECT_SOURCE_DIR}/cpp/pcl.cpp)
target_include_directories(pcl PUBLIC
  ${EIGEN3_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${GRID_MAP_CORE_SOURCE_DIR}/include
  ${GRID_MAP_PCL_SOURCE_DIR}/include
  ${GRID_MAP_SDF_SOURCE_DIR}/include
)
target_link_libraries(pcl PRIVATE ${PCL_LIBRARIES})

# Install files
file(GLOB GRID_MAP_PYTHON_BINDINGS ${PROJECT_BINARY_DIR}/*.cpython*)
file(GLOB GRID_MAP_SHARED_LIBS ${PROJECT_BINARY_DIR}/*.so)
file(GLOB GRID_MAP_PYTHON_FILES ${PROJECT_SOURCE_DIR}/python/*.py)
install(
  FILES ${GRID_MAP_PYTHON_BINDINGS} ${GRID_MAP_PYTHON_FILES} ${GRID_MAP_SHARED_LIBS}
  DESTINATION ${PYTHON_INSTALL_DIR}
)
