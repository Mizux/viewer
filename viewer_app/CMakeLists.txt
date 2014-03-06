cmake_minimum_required(VERSION 2.8)
project(viewerapp)

# Qt Setting
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

include_directories(${viewer_INCLUDE_DIRS})

# The file list for this project
set(_SRCS src/main.cpp)

add_executable(${PROJECT_NAME} ${_SRCS})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES}	viewer)

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
