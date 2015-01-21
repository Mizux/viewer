cmake_minimum_required(VERSION 2.8.0)
project(Viewer)

set(VIEWER_MAJOR_VERSION 0)
set(VIEWER_MINOR_VERSION 1)
set(VIEWER_PATCH_VERSION 0)
set(VIEWER_VERSION ${VIEWER_MAJOR_VERSION}.${VIEWER_MINOR_VERSION}.${VIEWER_PATCH_VERSION})

##########################
##  INSTALL PARAMETERS  ##
##########################
# Offer the user the choice of overriding the installation directories
#set(CMAKE_INSTALL_PREFIX install)
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKE_DIR lib/CMake/Viewer CACHE PATH "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

##############################
##  COMPILATION PARAMETERS  ##
##############################
# Set this on if you need verbose output from CMake
set(CMAKE_VERBOSE_MAKEFILE ON)

if(UNIX AND NOT APPLE)
	set(C_WARNFLAGS
		#-Werror
		-Wall
		-Wextra
		-Wconversion
		-Wpointer-arith
		-Wcast-align
		-fno-strict-aliasing
		-Wno-long-long
		)
	foreach(arg ${C_WARNFLAGS})
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${arg}")
	endforeach(arg ${C_WARNFLAGS})

	set(CXX_WARNFLAGS
		${C_WARNFLAGS}
		-pedantic
		#-pedantic-errors
		-Weffc++
		-Wold-style-cast
		)
	foreach(arg ${CXX_WARNFLAGS})
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${arg}")
	endforeach(arg ${CXX_WARNFLAGS})
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)

add_subdirectory(viewer)
add_subdirectory(viewer_app)

###################
##  CMAKE STUFF  ##
###################
# Add all targets to the build-tree export set
export(TARGETS viewer viewerApp
  FILE "${PROJECT_BINARY_DIR}/ViewerTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE Viewer)

# Create the FooBarConfig.cmake and FooBarConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(ViewerConfig.cmake.in
  "${PROJECT_BINARY_DIR}/ViewerConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${VIEWER_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(ViewerConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ViewerConfig.cmake" @ONLY)
# ... for both
configure_file(ViewerConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/ViewerConfigVersion.cmake" @ONLY)

# Install the ViewerConfig.cmake and ViewerConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ViewerConfig.cmake"
  "${PROJECT_BINARY_DIR}/ViewerConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT ViewerTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)
