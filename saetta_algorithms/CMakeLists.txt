cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
set(coverage RelWithDebInfo)
set(CMAKE_CXX_FLAGS "-std=c++0x")
rosbuild_init()


rosbuild_find_ros_package(saetta_msgs)

#set paths
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

link_directories(${LIBRARY_OUTPUT_PATH})

# Set the directories where include files can be found.
include_directories (include)
include_directories (${saetta_msgs_PACKAGE_PATH}/msg_gen/cpp/include)

rosbuild_find_ros_package (tf)

#common commands for building c++ executables and libraries
rosbuild_add_boost_directories()
rosbuild_add_executable(rigidform src/rigidform.cpp)
rosbuild_link_boost(rigidform thread)
target_link_libraries(rigidform ncurses rt tf m)
