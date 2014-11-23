cmake_minimum_required(VERSION 2.8)
set(_NAME ViewerApp)

# Qt Setting
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

include_directories(SYSTEM ${Viewer_INCLUDE_DIRS})

# The file list for this project
set(_SRCS src/main.cpp)

add_executable(${_NAME} ${_SRCS})
target_link_libraries(${_NAME} ${QT_LIBRARIES} Viewer)
install(TARGETS ${_NAME} DESTINATION .)
