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

find_package(Protobuf REQUIRED)

#set (PROTO_SRCS ${PROJECT_SOURCE_DIR}/src/timetests.cpp)
#set (PROTO_HDRS ${PROJECT_SOURCE_DIR}/include/timetests.hpp)
#ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/msg)

# Set the directories where include files can be found.
include_directories (include)
include_directories (${INCLUDE_PATHS})
include_directories (${std_msgs_PACKAGE_PATH}/msg_gen/cpp/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(PROTOBUF_FOUND)
    INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
    PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${PROJECT_SOURCE_DIR}/msg/timetests.proto)
ENDIF(PROTOBUF_FOUND)

rosbuild_init()
rosbuild_find_ros_package(saetta_msgs REQUIRED)
rosbuild_add_boost_directories()

#set paths
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(PATH_SRC ${PROJECT_SOURCE_DIR}/src)
set(PATH_INC ${PROJECT_SOURCE_DIR}/include)
set(INCLUDE_PATHS ${PATH_INC} ${PROTOBUF_INCLUDE_DIRS})
link_directories(${LIBRARY_OUTPUT_PATH})




# Add dynamic reconfigure API.
#rosbuild_find_ros_package (dynamic_reconfigure)
#include (${dynamic_reconfigure_PACKAGE_PATH}/cmake/cfgbuild.cmake)
#gencfg ()


#common commands for building c++ executables and libraries
rosbuild_add_executable (timing_root ${PROJECT_SOURCE_DIR}/src/timing_root.cpp ${PROTO_SRCS} ${PROTO_HDRS})
add_library(timing_msgs ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(timing_root timing_msgs ${PROTOBUF_LIBRARIES})
