cmake_minimum_required (VERSION 3.16)

project ("hw6 project" LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(
	"${CMAKE_CURRENT_LIST_DIR}/CMakeIn"
	"${CMAKE_CURRENT_LIST_DIR}/CMakeIn.h"
)

# dependencies
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_LIST_DIR}/3rd")
include_directories(${THIRD_PARTY_DIR})
include_directories(${OPENGL_INCLUDE_DIR})


# Include directories for XQuartz
if(APPLE)
    include_directories(/opt/X11/include)
    link_directories(/opt/X11/lib)
endif()

add_subdirectory("${THIRD_PARTY_DIR}/freeglut")
include_directories("${THIRD_PARTY_DIR}/freeglut/include")

add_subdirectory("${THIRD_PARTY_DIR}/shapeLib")

set(TARGET_NAME "hw6_lib")
add_library(
	${TARGET_NAME}
	"Geometry.cpp"
	"QuadTree.cpp"
	"QuadTreeTest.cpp"
)
target_link_libraries(
	${TARGET_NAME}
	PUBLIC "freeglut_static"
	${OPENGL_LIBRARIES}
)

set(TARGET_NAME "hw6_app")
add_executable(
	${TARGET_NAME}
	"hw6.cpp"
)
target_link_libraries(
	${TARGET_NAME}
	PRIVATE "shape_lib"
	PRIVATE "hw6_lib"
	${OPENGL_LIBRARIES} 
	${OPENGL_LIBRARIES}
   	 "-framework OpenGL"  # Link to the OpenGL framework
    	"-framework GLUT"    # Link to the GLUT framework
)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
