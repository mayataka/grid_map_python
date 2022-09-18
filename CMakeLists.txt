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
set(GRID_MAP_INSTALL_DIR ${PROJECT_SOURCE_DIR}/grid_map-install)
FetchContent_GetProperties(grid_map)
if(NOT grid_map_POPULATED)
FetchContent_Populate(
  grid_map
  GIT_REPOSITORY https://github.com/ANYbotics/grid_map
  GIT_TAG        master
  SOURCE_DIR ${GRID_MAP_SOURCE_DIR}
  SUBBUILD_DIR ${GRID_MAP_SUBBUILD_DIR}
)
endif()
macro(add_grid_map_external_project MODULE)
ExternalProject_Add(
  ${MODULE}
  SOURCE_DIR ${GRID_MAP_SOURCE_DIR}/${MODULE}
  BUILD_IN_SOURCE TRUE
  INSTALL_DIR ${GRID_MAP_INSTALL_DIR}
  CONFIGURE_COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCATKIN_ENABLE_TESTING=OFF
)
endmacro()

add_grid_map_external_project(grid_map_core)

add_grid_map_external_project(grid_map_sdf)
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
  ${GRID_MAP_INSTALL_DIR}/include
  ${EIGEN3_INCLUDE_DIRS}
  ${GRID_MAP_SOURCE_DIR}/grid_map_pcl/include 
  ${PCL_INCLUDE_DIRS}
)
endmacro()

macro(pybind11_grid_map_link_libraries MODULE)
target_link_libraries(
  py${MODULE}
  PRIVATE
  $<BUILD_INTERFACE:${GRID_MAP_INSTALL_DIR}/lib/lib${MODULE}.so>
  $<INSTALL_INTERFACE:${PYTHON_INSTALL_DIR}/grid_map/lib${MODULE}.so>
)
endmacro()

pybind11_add_grid_map_module(grid_map_core)
pybind11_grid_map_include_directories(grid_map_core)
pybind11_grid_map_link_libraries(grid_map_core)

pybind11_add_grid_map_module(grid_map_sdf)
pybind11_grid_map_include_directories(grid_map_sdf)
pybind11_grid_map_link_libraries(grid_map_sdf)

pybind11_add_module(pygrid_map_pcl SHARED 
  ${PROJECT_SOURCE_DIR}/cpp/pygrid_map_pcl.cpp
  ${GRID_MAP_SOURCE_DIR}/grid_map_pcl/src/GridMapPclConverter.cpp
)
target_include_directories(pygrid_map_pcl PRIVATE
  ${GRID_MAP_SOURCE_DIR}/grid_map_pcl/include 
  ${GRID_MAP_SOURCE_DIR}/grid_map_core/include
  ${PCL_INCLUDE_DIRS}
)
target_link_libraries(pygrid_map_pcl PRIVATE 
  ${PCL_LIBRARIES} 
  $<BUILD_INTERFACE:${GRID_MAP_INSTALL_DIR}/lib/libgrid_map_core.so>
  $<INSTALL_INTERFACE:${PYTHON_INSTALL_DIR}/grid_map/libgrid_map_core.so>
)

# Install files
file(GLOB GRID_MAP_PYTHON_BINDINGS ${PROJECT_BINARY_DIR}/*.cpython*)
file(GLOB GRID_MAP_PYTHON_FILES ${PROJECT_SOURCE_DIR}/python/*.py)
file(GLOB GRID_MAP_SHARED_LIBS ${GRID_MAP_INSTALL_DIR}/lib/*.so)
install(
  FILES ${GRID_MAP_PYTHON_BINDINGS} ${GRID_MAP_PYTHON_FILES} ${GRID_MAP_SHARED_LIBS}
  DESTINATION ${PYTHON_INSTALL_DIR}
)
