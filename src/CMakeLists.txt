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

# Relative plug-in directory (i.e. lib/plugin for instance)
# as exposed by the dynamic-graph pkg-config file.
SET(PLUGINDIR ${DYNAMIC_GRAPH_PLUGINDIRNAME})
SET(FULLPLUGINDIR ${CMAKE_INSTALL_PREFIX}/${PLUGINDIR})

INCLUDE(../cmake/idl.cmake)

# Declare that dynamic-graph shared library is being built.
ADD_DEFINITIONS(-DBUILDING_DG_MIDDLEWARE)

ADD_DEFINITIONS(-DDEBUG=2)

# Make sure the generated CORBA headers will be found.
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/src)

# Use dynamic-graph.
ADD_DEFINITIONS(${DYNAMIC_GRAPH_CFLAGS})
LINK_DIRECTORIES(${DYNAMIC_GRAPH_LIBRARY_DIRS})

# Use omniORB4.
ADD_DEFINITIONS(${OMNIORB4_CFLAGS})
LINK_DIRECTORIES(${OMNIORB4_LIBRARY_DIRS})

GENERATE_IDL_FILE(
  server-command-corba
  ${CMAKE_SOURCE_DIR}/include/dg-middleware)

INSTALL(FILES
  ${CMAKE_SOURCE_DIR}/include/dg-middleware/server-command-corba.idl
  DESTINATION ${IDLDIR}
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE)

SET(server-command_files
  server-command-corbaSK.cc
  server-command-corba.hh
  corba-server-generic.cpp)

SET(server-abstract_files
  server-command-corbaSK.cc
  server-command-corba.hh
  corba-server-generic.cpp)

SET(server-command_dependencies server-abstract)

# Depend on a non-local plug-in.
SET(distant-shell_dependencies
  ${DYNAMIC_GRAPH_PLUGINDIR}/contiifstream${CMAKE_SHARED_LIBRARY_SUFFIX}
  exception-dgm)

SET(libs
  server-abstract
  server-command
  distant-shell
  exception-dgm)

FOREACH(lib ${libs})
  ADD_LIBRARY(${lib}
    SHARED
    ${lib}.cpp
    ${${lib}_files})

  SET_TARGET_PROPERTIES(${lib}
    PROPERTIES
    SOVERSION ${PROJECT_VERSION}
    PREFIX "")

  TARGET_LINK_LIBRARIES(${lib} ${DYNAMIC_GRAPH_LIBRARIES})
  TARGET_LINK_LIBRARIES(${lib} ${Boost_LIBRARIES})
  TARGET_LINK_LIBRARIES(${lib} ${OMNIORB4_LIBRARIES})

  TARGET_LINK_LIBRARIES(${lib} ${${lib}_dependencies})

  # Warning: *NEVER* rely on dynamic-graph full plug-in directory for
  # installation as the prefix of dynamic-graph and dg-middleware may
  # be different. Instead use the relative one and really install in
  # the prefix asked by the user.
  INSTALL(TARGETS ${lib} DESTINATION ${PLUGINDIR})
ENDFOREACH(lib)
