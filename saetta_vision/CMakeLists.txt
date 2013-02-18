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

#set paths
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(INC_PATH_01 ${PROJECT_SOURCE_DIR}/cvblobs8.3_linux/)
set(INCLUDE_PATHS ${INC_PATH_01})

link_directories(${LIBRARY_OUTPUT_PATH})
# Set the directories where include files can be found.
include_directories (include)
include_directories (${INCLUDE_PATHS})
# Add dynamic reconfigure API.
#rosbuild_find_ros_package (dynamic_reconfigure)
#include (${dynamic_reconfigure_PACKAGE_PATH}/cmake/cfgbuild.cmake)
#gencfg ()

find_package(OpenCV 2.4.0 REQUIRED)
#include (${opencv_PACKAGE_PATH}/OpenCVConfig.cmake)
#Find_Package(opencv REQUIRED)

#if(NOT SDL_FOUND)
#    message( FATAL ERROR "SDL not found!")
#endif(NOT SDL_FOUND)

#common commands for building c++ executables and libraries
rosbuild_add_library(vision Vision.cpp)
# add imported target
add_library(blobs STATIC IMPORTED)
# point the imported target at the real file
set_property(TARGET blobs PROPERTY IMPORTED_LOCATION ../cvblobs8.3_linux/libblob.a)
target_link_libraries(vision ${OpenCV_LIBS} ${PROJECT_SOURCE_DIR}/cvblobs8.3_linux/libblob.a)
rosbuild_add_executable(vispos main.cpp)
target_link_libraries(vispos vision)
