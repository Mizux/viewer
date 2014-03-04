cmake_minimum_required(VERSION 2.8)
project(ViewerApp)

# Qt Setting
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

include_directories(SYSTEM ${Viewer_INCLUDE_DIRS})

# The file list for this project
set(${PROJECT_NAME}_SRCS
	src/main.cpp
	)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES}	Viewer)
install(TARGETS ${PROJECT_NAME} DESTINATION .)
