# 
#  Copyright 
# 

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

ADD_DEFINITIONS(-DDEBUG=2)

IF(UNIX)
	ADD_DEFINITIONS(-pthread)
ENDIF(UNIX)

# provide path to library libdynamic-graph.so
LINK_DIRECTORIES(${DYNAMIC_GRAPH_LIBRARY_DIRS})

# Add dynamic-graph compilation flags and link to library libdynamic-graph.so
ADD_DEFINITIONS(${DYNAMIC_GRAPH_CFLAGS})

IF(WIN32)
foreach(dlink ${DYNAMIC_GRAPH_LDFLAGS})
	SET ( ${PROJECT_NAME}_src_LDFLAGS "${${PROJECT_NAME}_src_LDFLAGS}  ${dlink}")
endforeach(dlink)
ENDIF(WIN32)


# CORBA header generation
SET(CORBA_AUTOMATIC_DIRECTORY ${${PROJECT_NAME}_BINARY_DIR}/stubs)

  IF(IS_DIRECTORY ${CORBA_AUTOMATIC_DIRECTORY})
    # Nothing to do
  ELSE(IS_DIRECTORY ${CORBA_AUTOMATIC_DIRECTORY})
    MAKE_DIRECTORY(${CORBA_AUTOMATIC_DIRECTORY})
  ENDIF(IS_DIRECTORY ${CORBA_AUTOMATIC_DIRECTORY})

  SET(ServerCommand_CPP "${CORBA_AUTOMATIC_DIRECTORY}/server-command-corbaSK.cc")
  SET(ServerCommand_Header "${CORBA_AUTOMATIC_DIRECTORY}/server-command-corba.h")
  #SET (sotServerAbstract_plugins_link_flags "${BOOST_THREAD_LIB_NAME}")

  SET(IDL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}")

  IDLFILERULE(${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/server-command-corba.idl
              ${ServerCommand_CPP} ${ServerCommand_Header}
	      ${CORBA_AUTOMATIC_DIRECTORY} ${IDL_INCLUDE_DIR})
	      
  INCLUDE_DIRECTORIES(${CORBA_AUTOMATIC_DIRECTORY})
	      
INSTALL(FILES
	   ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/server-command-corba.idl
	   DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}
	   PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE)
	   

SET(server-command_files ${ServerCommand_CPP} ${ServerCommand_Header} corba-server-generic.cpp)

SET(libs
	server-abstract
	server-command)

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
	ENDIF(UNIX)
	
	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} ${Boost_THREAD_LIBRARY})
		
	IF(WIN32)
		SET_TARGET_PROPERTIES(${lib}
		  PROPERTIES
		  LINK_FLAGS "${${PROJECT_NAME}_src_LDFLAGS}"
		)
	ENDIF(WIN32)
	
	INSTALL(TARGETS ${lib}
  	DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plugin)
ENDFOREACH(lib)

