cmake_minimum_required(VERSION 2.8)

#####################
##  Documentation  ##
#####################
add_subdirectory("doc")

###################
##  Compilation  ##
###################
# Boost Setting
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

set(_NAME viewer)

# The file list for this project
set(_HDRS
	include/viewer/adapter_widget.hpp
	include/viewer/image_geometry.hpp
	include/viewer/image_viewer_2d.hpp
	include/viewer/image_viewer_2d_widget.hpp
	include/viewer/image_viewer_3d.hpp
	include/viewer/image_viewer_3d_widget.hpp
	include/viewer/osg_headers.hpp
	include/viewer/point_cloud_geometry.hpp
	include/viewer/tools.hpp
	include/viewer/type.hpp)
qt4_wrap_cpp(_MOC_SRCS ${_HDRS})

set(_SRCS
	src/adapter_widget.cpp
	src/image_geometry.cpp
	src/image_viewer_2d.cpp
	src/image_viewer_2d_widget.cpp
	src/image_viewer_3d.cpp
	src/image_viewer_3d_widget.cpp
	src/point_cloud_geometry.cpp
	src/tools.cpp)

add_library(${_NAME} SHARED ${_HDRS} ${_SRCS} ${_MOC_SRCS})
target_link_libraries(${_NAME}
	${Boost_LIBRARIES}
	${QT_LIBRARIES}
	${OPENSCENEGRAPH_LIBRARIES})
target_include_directories(${_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

set_target_properties(${_NAME} PROPERTIES PUBLIC_HEADER "${_HDRS}")
install(TARGETS ${_NAME}
	# IMPORTANT: Add the viewer library to the "export-set"
	EXPORT ViewerTargets
	RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
	LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
	PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/viewer" COMPONENT dev)
