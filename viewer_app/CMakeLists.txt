cmake_minimum_required(VERSION 2.8)
project(viewerapp)

# Qt Setting
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# The file list for this project
file(GLOB_RECURSE _SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*)

add_executable(${PROJECT_NAME} ${_SRCS})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES}	viewer)

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
