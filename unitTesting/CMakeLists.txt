# 
#  Copyright 
# 


SET(CORBA_AUTOMATIC_DIRECTORY ${${PROJECT_NAME}_BINARY_DIR}/stubs)
INCLUDE_DIRECTORIES(
	${CORBA_AUTOMATIC_DIRECTORY}
	${OMNIORB4_DIR}/include)


ADD_DEFINITIONS(-DDEBUG=2)

# provide path to library libdynamic-graph.so
LINK_DIRECTORIES(${DYNAMIC_GRAPH_LIBRARY_DIRS})


INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/src)

ADD_DEFINITIONS(${omniORB4_cflags})

# Add dynamic-graph compilation flags and link to library libdynamic-graph.so
ADD_DEFINITIONS(${DYNAMIC_GRAPH_CFLAGS})
	
SET(test_libs
	CorbaConnection)
	
SET(tests
	dummy
	coshell)
	
FOREACH(test ${test_libs})
	SET(LIBRARY_NAME ${test})
	
	ADD_LIBRARY(${LIBRARY_NAME}
	  SHARED
	  ${test}.cpp)
	
	SET_TARGET_PROPERTIES(${LIBRARY_NAME}
		PROPERTIES
		LINK_FLAGS "${${PROJECT_NAME}_src_LDFLAGS} ${omniORB4_link_FLAGS} -lboost_program_options-mt")
	
	TARGET_LINK_LIBRARIES(${LIBRARY_NAME}
	  server-command
	  server-abstract
	  ${BOOST_THREAD_LIB_NAME})
	
	TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${DYNAMIC_GRAPH_LIBRARIES})
ENDFOREACH(test)

FOREACH(test ${tests})
	SET(EXECUTABLE_NAME ${test}_exe)
	
	ADD_EXECUTABLE(${EXECUTABLE_NAME}
	  ${test}.cpp)
	
	SET_TARGET_PROPERTIES(${EXECUTABLE_NAME}
		PROPERTIES
		LINK_FLAGS "${${PROJECT_NAME}_src_LDFLAGS} ${omniORB4_link_FLAGS} -lboost_program_options-mt")
	
	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME}
	  server-command
	  server-abstract
	  ${BOOST_THREAD_LIB_NAME}
	  ${test_libs})
	
	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} ${DYNAMIC_GRAPH_LIBRARIES})
ENDFOREACH(test)