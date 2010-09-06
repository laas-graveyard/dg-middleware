# 
#  Copyright 
# 

SET(CORBA_AUTOMATIC_DIRECTORY ${${PROJECT_NAME}_BINARY_DIR}/stubs)
INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/include
	${CORBA_AUTOMATIC_DIRECTORY}
	${OMNIORB4_DIR}/include)


ADD_DEFINITIONS(-DDEBUG=2)

# provide path to library libdynamic-graph.so
LINK_DIRECTORIES(${DYNAMIC_GRAPH_LIBRARY_DIRS})

LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/src)

ADD_DEFINITIONS(${omniORB4_cflags})

# Add dynamic-graph compilation flags and link to library libdynamic-graph.so
ADD_DEFINITIONS(${DYNAMIC_GRAPH_CFLAGS})
	
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
	SET(EXECUTABLE_NAME ${test})
	
	ADD_EXECUTABLE(${EXECUTABLE_NAME}
	  ${test}.cpp
	  ${${test}_additional_files})
	
	SET_TARGET_PROPERTIES(${EXECUTABLE_NAME}
		PROPERTIES
		LINK_FLAGS "${${PROJECT_NAME}_src_LDFLAGS} ${omniORB4_link_FLAGS} -lboost_program_options-mt")
	
	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME}
	  server-command
	  server-abstract
	  ${BOOST_THREAD_LIB_NAME}
	  ${test_libs})
	
	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} ${DYNAMIC_GRAPH_LIBRARIES})
	
	ADD_TEST(${test} ${test})
ENDFOREACH(test)

FOREACH(tool ${tools})
	SET(EXECUTABLE_NAME ${tool})
	
	ADD_EXECUTABLE(${EXECUTABLE_NAME}
	  ${tool}.cpp
	  ${${tool}_additional_files})
	
	SET_TARGET_PROPERTIES(${EXECUTABLE_NAME}
		PROPERTIES
		LINK_FLAGS "${${PROJECT_NAME}_src_LDFLAGS} ${omniORB4_link_FLAGS} -lboost_program_options-mt")
	
	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME}
	  server-command
	  server-abstract
	  ${BOOST_THREAD_LIB_NAME}
	  ${test_libs})
	
	MESSAGE("boost libs are  ${BOOST_PROGRAM_OPTIONS_LIB_NAME}")
	
	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} ${DYNAMIC_GRAPH_LIBRARIES} CorbaConnection)
	INSTALL(TARGETS ${EXECUTABLE_NAME} DESTINATION bin/sot)
ENDFOREACH(tool)