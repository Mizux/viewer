cmake_minimum_required(VERSION 2.8)
project(viewer)

set(BOOST_ROOT /home/clemolgat/.local/share/qi/toolchains/master-linux64/boost)
find_package(Boost REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# Qt Setting
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# OpenSceneGraph Setting
find_package(OpenSceneGraph 2.8 REQUIRED COMPONENTS osg osgViewer)
include_directories(SYSTEM ${OPENSCENEGRAPH_INCLUDE_DIRS})

# The file list for this project
file(GLOB_RECURSE _HDRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*)
qt4_wrap_cpp(_MOC_SRCS ${_HDRS})
file(GLOB_RECURSE _SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*)

add_library(${PROJECT_NAME} SHARED ${_HDRS} ${_SRCS} ${_MOC_SRCS})
target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
  ${QT_LIBRARIES}
  ${OPENSCENEGRAPH_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${_HDRS}")
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/${PROJECT_NAME}" COMPONENT dev)
