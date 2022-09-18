# grid_map_python

A Python wrapper of [grid_map](https://github.com/ANYbotics/grid_map) to run it without C++ or ROS.

## Build and install (ROS environment is required in the build)
```
source /opt/ros/noetic/setup.bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make install -j8
```