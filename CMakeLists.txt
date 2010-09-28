# Copyright 2010, François Bleibel, JRL, CNRS/AIST
#
# This file is part of dg-middleware.
# dg-middleware is free software: you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# dg-middleware is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# dg-middleware. If not, see <http://www.gnu.org/licenses/>.

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(PROJECT_NAME dg-middleware)
project(${PROJECT_NAME})

SET(PROJECT_VERSION 1.0)

SET(CMAKE_VERBOSE_MAKEFILE true)

IF(WIN32)
  SET(LIBDIR_KW "/LIBPATH:")
  SET(LIBINCL_KW "")
  SET(LIB_EXT ".lib")
ENDIF(WIN32)

IF(UNIX)
  SET(LIBDIR_KW "-L")
  SET(LIBINCL_KW "-l")
  SET(LIB_EXT "")
ENDIF(UNIX)

#
# Look for dependencies
#

# where are use-specific cmake modules
SET(CMAKE_MODULE_PATH ${${PROJECT_NAME}_SOURCE_DIR}/CMakeModules)


# Load Pkgconfig macros
INCLUDE(FindPkgConfig)
SET(PACKAGE_REQUIREMENTS "")

# --- BOOST ---------------------------------------------------
SET(Boost_THREAD_FOUND 0)
FIND_PACKAGE(Boost COMPONENTS thread program_options REQUIRED)
IF(Boost_THREAD_FOUND)

IF(UNIX)
	ADD_DEFINITIONS(-pthread)
ENDIF(UNIX)
ADD_DEFINITIONS(-DHAVE_LIBBOOST_THREAD)

ELSE(Boost_THREAD_FOUND)
  MESSAGE("WARNING: Boost thread not found. Trying without the thread library")
  FIND_PACKAGE(Boost)
  IF(NOT Boost_FOUND)
	MESSAGE(SEND_ERROR "!! ERROR: Boost not found.")
  ENDIF(NOT Boost_FOUND)
ENDIF(Boost_THREAD_FOUND)



INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

#
#  Dependency to package dynamic-graph
#
SET(DYNAMIC_GRAPH_FOUND 0)
SET(DYNAMIC_GRAPH_REQUIRED "dynamic-graph >= 1.0")
PKG_CHECK_MODULES(DYNAMIC_GRAPH REQUIRED ${DYNAMIC_GRAPH_REQUIRED})
EXEC_PROGRAM(${PKG_CONFIG_EXECUTABLE} ARGS  --variable=docdir dynamic-graph
  OUTPUT_VARIABLE DYNAMIC_GRAPH_DOCDIR)
MESSAGE(STATUS "pkg-config: ${PKG_CONFIG_EXECUTABLE}")
MESSAGE(STATUS "DYNAMIC_GRAPH_DOCDIR=${DYNAMIC_GRAPH_DOCDIR}")
SET(DYNAMIC_GRAPH_FOUND 1)

IF(NOT ${DYNAMIC_GRAPH_FOUND})
  MESSAGE(FATAL_ERROR "Check that package dynamic-graph is installed in a directory pointed out by PKG_CONFIG_PATH.")
ENDIF(NOT ${DYNAMIC_GRAPH_FOUND})
  
IF(PACKAGE_REQUIREMENTS STREQUAL "")
  SET(PACKAGE_REQUIREMENTS ${DYNAMIC_GRAPH_REQUIRED})
ELSE(PACKAGE_REQUIREMENTS STREQUAL "")
  SET(PACKAGE_REQUIREMENTS "${PACKAGE_REQUIREMENTS}, ${DYNAMIC_GRAPH_REQUIRED}")
ENDIF(PACKAGE_REQUIREMENTS STREQUAL "")

# --- OmniOrb ------------------------------------------------
INCLUDE(${CMAKE_MODULE_PATH}/UseOmniORB4.cmake)
MESSAGE(STATUS "omniORB4: ${OMNIORB4_DIR}")
#SET (${PROJECT_NAME}_CXX_FLAGS ${${PROJECT_NAME}_CXX_FLAGS} 
#    -I${${PROJECT_NAME}_BINARY_DIR}/stubs)
    
#
# pkg-config file
#

SET(install_pkg_prefix "\${prefix}")
SET(install_pkg_exec_prefix "\${exec_prefix}")
SET(install_pkg_libdir "\${libdir}")
SET(install_pkg_datarootdir "\${datarootdir}")
SET(install_pkg_include_dir "\${includedir}")

SET(libs
	server-abstract
	server-command
	distant-shell
	exception-dgm)
	
SET(libs-to-pc
	server-command
	distant-shell
	exception-dgm)
	
FOREACH(lib ${libs-to-pc})
	SET(pc-libs "${pc-libs} ${LIBINCL_KW}${lib}${CMAKE_IMPORT_LIBRARY_SUFFIX}")
ENDFOREACH(lib)

IF(UNIX)
  SET (${PROJECT_NAME}_LDFLAGS "-Wl,-R${install_pkg_libdir} ${pc-libs} ${${PROJECT_NAME}_LDFLAGS}")
ENDIF(UNIX)

IF(WIN32)
  SET (${PROJECT_NAME}_LDFLAGS "${pc-libs}")
ENDIF(WIN32)

# Install the file in the appropriate place.
SET(install_libdir_pkgconfig ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.pc.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
  DESTINATION ${install_libdir_pkgconfig}	
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE)
  
  # Uninstall target.
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
  

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(unitTesting)

#
# Documentation produced by doxygen
#
INCLUDE(FindDoxygen)
IF(DOXYGEN_FOUND)
  add_subdirectory(doc)
ENDIF(DOXYGEN_FOUND)
