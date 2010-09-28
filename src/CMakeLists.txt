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

IF(UNIX)
	ADD_DEFINITIONS(-pthread)
ENDIF(UNIX)

# provide path to library libdynamic-graph.so
LINK_DIRECTORIES(${DYNAMIC_GRAPH_LIBRARY_DIRS})

# The main include dir
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

# Add dynamic-graph compilation flags and link to library libdynamic-graph.so
ADD_DEFINITIONS(${DYNAMIC_GRAPH_CFLAGS})

IF(WIN32)
foreach(dlink ${DYNAMIC_GRAPH_LDFLAGS} ${omniORB4_link_FLAGS})
	SET ( ${PROJECT_NAME}_src_LDFLAGS "${${PROJECT_NAME}_src_LDFLAGS}  ${dlink}")
endforeach(dlink)
ENDIF(WIN32)

ADD_DEFINITIONS(${omniORB4_cflags})


# CORBA header generation
SET(CORBA_AUTOMATIC_DIRECTORY ${${PROJECT_NAME}_BINARY_DIR}/stubs)

  IF(IS_DIRECTORY ${CORBA_AUTOMATIC_DIRECTORY})
    # Nothing to do
  ELSE(IS_DIRECTORY ${CORBA_AUTOMATIC_DIRECTORY})
    MAKE_DIRECTORY(${CORBA_AUTOMATIC_DIRECTORY})
  ENDIF(IS_DIRECTORY ${CORBA_AUTOMATIC_DIRECTORY})

  SET(ServerCommand_CPP "${CORBA_AUTOMATIC_DIRECTORY}/server-command-corbaSK.cc")
  SET(ServerCommand_Header "${CORBA_AUTOMATIC_DIRECTORY}/server-command-corba.h")

  SET(IDL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}")

  IDLFILERULE(${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/server-command-corba.idl
              ${ServerCommand_CPP} ${ServerCommand_Header}
	      ${CORBA_AUTOMATIC_DIRECTORY} ${IDL_INCLUDE_DIR})

  INCLUDE_DIRECTORIES(${CORBA_AUTOMATIC_DIRECTORY})

INSTALL(FILES
	   ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/server-command-corba.idl
	   DESTINATION share/${PROJECT_NAME}
	   PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE)


SET(server-command_files ${ServerCommand_CPP} ${ServerCommand_Header} corba-server-generic.cpp)
SET(server-abstract_files ${ServerCommand_CPP} ${ServerCommand_Header} corba-server-generic.cpp)
SET(server-command_dependencies server-abstract)
SET(distant-shell_dependencies
	${CMAKE_INSTALL_PREFIX}/lib/plugin/contiifstream${CMAKE_SHARED_LIBRARY_SUFFIX}
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
	  PREFIX ""
	  INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib/plugin)

	IF (UNIX)
		TARGET_LINK_LIBRARIES(${lib} ${DYNAMIC_GRAPH_LIBRARIES})
		TARGET_LINK_LIBRARIES(${lib} ${Boost_LIBRARIES})
		TARGET_LINK_LIBRARIES(${lib} ${omniORB4_link_FLAGS})
		TARGET_LINK_LIBRARIES(${lib} "pthread")
	ENDIF(UNIX)

	TARGET_LINK_LIBRARIES(${lib} ${${lib}_dependencies})

	IF(WIN32)
		SET_TARGET_PROPERTIES(${lib}
		  PROPERTIES
		  LINK_FLAGS "${${PROJECT_NAME}_src_LDFLAGS}"
		)
	ENDIF(WIN32)

	INSTALL(TARGETS ${lib}
  	DESTINATION lib/plugin)
ENDFOREACH(lib)

