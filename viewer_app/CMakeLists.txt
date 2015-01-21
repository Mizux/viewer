cmake_minimum_required(VERSION 2.8)

###################
##  Compilation  ##
###################
# Qt Setting
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(_NAME viewerApp)

include_directories(../viewer/include)

# The file list for this project
set(_SRCS src/main.cpp)

add_executable(${_NAME} ${_SRCS})
target_link_libraries(${_NAME} viewer)

install(TARGETS ${_NAME}
	# IMPORTANT: Add the bar executable to the "export-set"
	EXPORT ViewerTargets
	RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
