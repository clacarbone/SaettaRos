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
rosbuild_find_ros_package(saetta_msgs REQUIRED)
rosbuild_add_boost_directories()
find_package(PkgConfig REQUIRED)
pkg_check_modules(OpenCV REQUIRED opencv)
pkg_check_modules(gtk+ REQUIRED gtk+-2.0)
#find_package(OpenCV 2.4.0 REQUIRED)
#find_package(GTK REQUIRED)

#if(gtk+_FOUND)
#	message(STATUS "Gtk found!")
#	message(STATUS "GTK IncludeDir: " ${gtk+_INCLUDEDIR})
#	message(STATUS "GTK Include_Dirs: " ${gtk+_INCLUDE_DIRS})
#endif(gtk+_FOUND)

#if(OpenCV_FOUND)
#	message(STATUS "OpenCV found!")
#	message(STATUS "OpenCV IncludeDir: " ${OpenCV_INCLUDEDIR})
#	message(STATUS "OpenCV Include_Dirs: " ${OpenCV_INCLUDE_DIRS})
#	message(STATUS "OpenCV libs: " ${OpenCV_LIBRARIES})
#endif(OpenCV_FOUND)

#set paths
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(PATH_SRC ${PROJECT_SOURCE_DIR}/src)
set(PATH_CVBLOBS ${PATH_SRC}/cvblobs8.3_linux)
set(PATH_INC ${PROJECT_SOURCE_DIR}/include)
set(INCLUDE_PATHS ${PATH_INC} ${PATH_CVBLOBS} ${OpenCV_CFLAGS} ${gtk+_CFLAGS})
link_directories(${LIBRARY_OUTPUT_PATH})

#set filename holders
set(SRC_CVBLOBS ${PATH_CVBLOBS}/blob.cpp
		${PATH_CVBLOBS}/BlobContour.cpp
		${PATH_CVBLOBS}/BlobOperators.cpp
		${PATH_CVBLOBS}/BlobProperties.cpp
		${PATH_CVBLOBS}/BlobResult.cpp
		${PATH_CVBLOBS}/ComponentLabeling.cpp)

# Set the directories where include files can be found.
include_directories (include)
include_directories (${INCLUDE_PATHS})
include_directories (${gtk+_INCLUDE_DIRS})
include_directories (${OpenCV_INCLUDE_DIRS})
include_directories (${saetta_msgs_PACKAGE_PATH}/msg_gen/cpp/include)


# Add dynamic reconfigure API.
#rosbuild_find_ros_package (dynamic_reconfigure)
#include (${dynamic_reconfigure_PACKAGE_PATH}/cmake/cfgbuild.cmake)
#gencfg ()

#include (${opencv_PACKAGE_PATH}/OpenCVConfig.cmake)
#Find_Package(opencv REQUIRED)

#if(NOT SDL_FOUND)
#    message( FATAL ERROR "SDL not found!")
#endif(NOT SDL_FOUND)

#common commands for building c++ executables and libraries
rosbuild_add_library(vision ${PATH_SRC}/Vision.cpp)
rosbuild_add_library(blob ${SRC_CVBLOBS})
# add imported target
#add_library(blobs STATIC IMPORTED)
# point the imported target at the real file
#set_property(TARGET blobs PROPERTY IMPORTED_LOCATION ${PATH_CVBLOBS}/libblob.a)
target_link_libraries(blob ${gtk+_LIBRARIES})
target_link_libraries(vision ${OpenCV_LIBRARIES} blob)
rosbuild_add_executable(vispos ${PATH_SRC}/main.cpp)
target_link_libraries(vispos vision)
rosbuild_link_boost(vispos system)
