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

ADD_DEFINITIONS(-DDEBUG=2)

# Make sure the generated CORBA headers will be found.
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/src)

# Add required pre-processor flags.
ADD_DEFINITIONS(${DYNAMIC_GRAPH_CFLAGS})
ADD_DEFINITIONS(${OMNIORB4_CFLAGS})

# provide path to library libdynamic-graph.so
LINK_DIRECTORIES(${DYNAMIC_GRAPH_LIBRARY_DIRS})
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/src)

SET(test_libs
	CorbaConnection)

SET(tests
	dummy
	test_corba_shell)
SET(test_corba_shell_additional_files connecttoshell.cpp)

SET(tools
	coshell)
SET(coshell_additional_files connecttoshell.cpp)

FOREACH(test ${test_libs})
	SET(LIBRARY_NAME ${test})

	ADD_LIBRARY(${LIBRARY_NAME}
	  STATIC
	  ${test}.cpp)

	PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} dynamic-graph)
	PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} omniORB4)

	TARGET_LINK_LIBRARIES(${LIBRARY_NAME}
	  server-command
	  server-abstract)
ENDFOREACH(test)

FOREACH(test ${tests})
	SET(EXECUTABLE_NAME ${test})

	ADD_EXECUTABLE(${EXECUTABLE_NAME}
	  ${test}.cpp
	  ${${test}_additional_files})

	PKG_CONFIG_USE_DEPENDENCY(${EXECUTABLE_NAME} dynamic-graph)
	PKG_CONFIG_USE_DEPENDENCY(${EXECUTABLE_NAME} omniORB4)

	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME}
	  server-command
	  server-abstract
	  ${test_libs})

	ADD_TEST(${test} ${test})
ENDFOREACH(test)

FOREACH(tool ${tools})
	SET(EXECUTABLE_NAME dg-${tool})

	ADD_EXECUTABLE(${EXECUTABLE_NAME}
	  ${tool}.cpp
	  ${${tool}_additional_files})

	PKG_CONFIG_USE_DEPENDENCY(${EXECUTABLE_NAME} dynamic-graph)
	PKG_CONFIG_USE_DEPENDENCY(${EXECUTABLE_NAME} omniORB4)

	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME}
	  server-command
	  server-abstract
	  ${test_libs})

	# FIXME: what is this?
	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} CorbaConnection)
	INSTALL(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
ENDFOREACH(tool)
