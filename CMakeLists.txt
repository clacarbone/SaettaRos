cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
set(coverage RelWithDebInfo)

rosbuild_init()

#set paths
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CORE_LIB_PATH ${PROJECT_SOURCE_DIR}/core)
set(NETUS_LIB_PATH ${PROJECT_SOURCE_DIR}/netus2pic)
set(AUSIL_LIB_PATH ${PROJECT_SOURCE_DIR}/ausiliarie)
set(SENSOR_LIB_PATH ${PROJECT_SOURCE_DIR}/sensor)
set(INC_PATH_01 ${PROJECT_SOURCE_DIR}/cfg/cpp/)
set(INC_PATH_02 ${PROJECT_SOURCE_DIR}/msg_gen/cpp/include)
set(INCLUDE_PATHS ${INC_PATH_01} ${INC_PATH_02} ${NETUS_LIB_PATH} ${CORE_LIB_PATH} ${AUSIL_LIB_PATH} ${SENSOR_LIB_PATH})
# Have ROS autogenerate code used in messages.
#rosbuild_genmsg ()

#uncomment if you have defined services
#rosbuild_gensrv()

link_directories(${LIBRARY_OUTPUT_PATH})

#add_custom_target(base SOURCES ${PROJECT_SOURCE_DIR}src/base.cpp)
#add_custom_target(teleop_saetta SOURCES ${PROJECT_SOURCE_DIR}src/saetta_teleop.cpp)

# Set the directories where include files can be found.
include_directories (include)
include_directories (${INCLUDE_PATHS})
#MESSAGE("Include paths")
#MESSAGE("${INCLUDE_PATHS}")
#MESSAGE("${INC_PATH_02}")

# Add dynamic reconfigure API.
rosbuild_find_ros_package (dynamic_reconfigure)
include (${dynamic_reconfigure_PACKAGE_PATH}/cmake/cfgbuild.cmake)
gencfg ()

Find_Package(SDL REQUIRED)
if(NOT SDL_FOUND)
    message( FATAL ERROR "SDL not found!")
endif(NOT SDL_FOUND)

#common commands for building c++ executables and libraries
rosbuild_add_library(base src/base.cpp)
#rosbuild_add_library(serverWIFI src/serverWIFI.cpp)
rosbuild_add_library(core src/Saetta_Base_core.cpp)
rosbuild_add_library(joystick src/joystick.c)
rosbuild_add_library(robotcore core/robot_core.c)
rosbuild_add_library(serial_comm netus2pic/serial_comm.c)
rosbuild_add_library(robot_comm netus2pic/robot_comm.c)
rosbuild_add_library(ausiliarie ausiliarie/funzioni_ausiliarie.c)
rosbuild_add_library(robot_sensors sensor/robot_sensors.c)
rosbuild_add_library(pic2netus core/pic2netus.c)
rosbuild_add_boost_directories()
rosbuild_add_executable(base src/base.cpp)
rosbuild_add_executable(teleop_saetta src/saetta_teleop.cpp)
rosbuild_add_executable(shellgui src/shellgui.cpp)
rosbuild_link_boost(base thread)
target_link_libraries(base core pic2netus serial_comm robot_comm ausiliarie robot_sensors ncurses)
target_link_libraries(robot_sensors ausiliarie robotcore)
target_link_libraries(robotcore ausiliarie m)
target_link_libraries(teleop_saetta joystick ${SDL_LIBRARY})
target_link_libraries(robot_comm serial_comm)
target_link_libraries(pic2netus robot_sensors ausiliarie m robotcore)
target_link_libraries(shellgui ncurses pthread)
