cmake_minimum_required( VERSION 2.8 )
PROJECT(AirHockey)

find_path(ASSIMP_INC Assimp)
find_library(ASSIMP_LIB Assimp)

find_path(GLUT_INC GLUT)
find_library(GLUT_LIB GLUT)

find_path(GLM_INC GLM)

find_path(OPENGL_INC OpenGL)
find_library(OPENGL_LIB OpenGL)

find_path(GLEW_INC GLEW)
find_library(GLEW_LIB GLEW)

include_directories(include)
include_directories(${ASSIMP_INC})
include_directories(${GLUT_INC})
include_directories(${GLM_INC})
include_directories(${OPENGL_INC})
include_directories(${GLEW_INC})

file(GLOB_RECURSE SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp)
file(GLOB_RECURSE INC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/*.h)
file(GLOB_RECURSE RESOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS resources/*)

foreach(RF ${RESOURCE_FILES})
	file(COPY ${RF} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach(RF ${RESOURCE_FILES})

add_executable(AirHockey ${SRCS} ${INC})
target_link_libraries(AirHockey ${ASSIMP_LIB})
target_link_libraries(AirHockey ${GLUT_LIB})
target_link_libraries(AirHockey ${OPENGL_LIB})
target_link_libraries(AirHockey ${GLEW_LIB})
#target_link_libraries(AirHockey opengl32.lib glut32.lib glew32.lib)