#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1600639559/map.o \
	${OBJECTDIR}/Vision.o \
	${OBJECTDIR}/_ext/1600639559/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-Wall -std=gnu++0x
CXXFLAGS=-Wall -std=gnu++0x

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs opencv` `pkg-config --libs roscpp` /home/attilio/andrea/cvblobs8.3_linux/libblob.a  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rosvisionsystem

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rosvisionsystem: /home/attilio/andrea/cvblobs8.3_linux/libblob.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rosvisionsystem: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rosvisionsystem ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1600639559/map.o: /home/attilio/andrea/tracking_vision_system_ros/map.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1600639559
	${RM} $@.d
	$(COMPILE.cc) -g -I../../cvblobs8.3_linux `pkg-config --cflags opencv` `pkg-config --cflags roscpp`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1600639559/map.o /home/attilio/andrea/tracking_vision_system_ros/map.cpp

${OBJECTDIR}/Vision.o: Vision.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../cvblobs8.3_linux `pkg-config --cflags opencv` `pkg-config --cflags roscpp`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/Vision.o Vision.cpp

${OBJECTDIR}/_ext/1600639559/main.o: /home/attilio/andrea/tracking_vision_system_ros/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1600639559
	${RM} $@.d
	$(COMPILE.cc) -g -I../../cvblobs8.3_linux `pkg-config --cflags opencv` `pkg-config --cflags roscpp`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1600639559/main.o /home/attilio/andrea/tracking_vision_system_ros/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rosvisionsystem

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
