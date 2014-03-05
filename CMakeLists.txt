cmake_minimum_required(VERSION 2.8.0)
project(ViewerQT)

set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 1)
set(${PROJECT_NAME}_PATCH_VERSION 0)
set(${PROJECT_NAME}_VERSION
 ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_VERSION})

# Set this on if you need verbose output from CMake
set(CMAKE_VERBOSE_MAKEFILE ON)

################################
######  Compilation Flags ######
################################
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

##################   INSTALL PARAMETERS    ################
# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKE_DIR lib/CMake/${PROJECT_NAME} CACHE PATH "Installation directory for CMake files")

# Default include directories
include_directories("${PROJECT_SOURCE_DIR}/include")

set(PROJECT_LIST viewer viewer_app)
foreach(arg ${PROJECT_LIST})
  add_subdirectory(${arg})
endforeach(arg ${PROJECT_LIST})

# Add all targets to the build-tree export set
export(TARGETS ${PROJECT_LIST} FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE ${PROJECT_NAME})

# Create the ProjectConfig.cmake and ProjectConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(${PROJECT_NAME}Config.cmake.in "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${FOOBAR_CMAKE_DIR}/${REL_INCLUDE_DIR}")

configure_file(${PROJECT_NAME}Config.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake" @ONLY)
configure_file(${PROJECT_NAME}ConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" @ONLY)

# Install the ProjectConfig.cmake and ProjectConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
# Install the export set for use with the install-tree
install(EXPORT ${PROJECT_NAME}Targets DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
