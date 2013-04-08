cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)
rosbuild_init()
rosbuild_genmsg()

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
set(coverage RelWithDebInfo)
set(CMAKE_CXX_FLAGS "-std=c++0x")

find_package(Protobuf REQUIRED)


# Set the directories where include files can be found.
include_directories (include)
include_directories (${INCLUDE_PATHS})
include_directories (${std_msgs_PACKAGE_PATH}/msg_gen/cpp/include)
include_directories (${CMAKE_CURRENT_BINARY_DIR})


rosbuild_init()
rosbuild_find_ros_package(saetta_msgs REQUIRED)
rosbuild_add_boost_directories()

#set paths
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(PATH_SRC ${PROJECT_SOURCE_DIR}/src)
set(PATH_INC ${PROJECT_SOURCE_DIR}/include)
set(INCLUDE_PATHS ${PATH_INC}) # ${PROTOBUF_INCLUDE_DIRS})
link_directories(${LIBRARY_OUTPUT_PATH})




# Add dynamic reconfigure API.
#rosbuild_find_ros_package (dynamic_reconfigure)
#include (${dynamic_reconfigure_PACKAGE_PATH}/cmake/cfgbuild.cmake)
#gencfg ()


#common commands for building c++ executables and libraries
rosbuild_add_executable (ts_generator ${PROJECT_SOURCE_DIR}/src/timestats_gen.cpp)
rosbuild_add_executable (ts_monitor ${PROJECT_SOURCE_DIR}/src/timestats_aggregator.cpp)
add_library(timing_class ${PROJECT_SOURCE_DIR}/src/timing_class.cpp)
#target_link_libraries(timing_class timing_msgs)
target_link_libraries(ts_generator timing_class)
target_link_libraries(ts_monitor timing_class)