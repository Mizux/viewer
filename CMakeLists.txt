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
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_CMAKE_DIR cmake CACHE PATH "Installation directory for CMake files")
# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

############# SUB PROJECTS ###################
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/sdk/lib CACHE PATH "Build directory for archive")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/sdk/lib CACHE PATH "Build directory for libraries")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/sdk/bin CACHE PATH "Build directory for executables")

set(SUB_PROJECTS viewer viewer_app)
foreach(arg ${SUB_PROJECTS})
  add_subdirectory(${arg})
endforeach(arg ${SUB_PROJECT})

##############################
##  VERSION + CONFIG CMAKE  ##
##############################
configure_file(${PROJECT_NAME}Config.cmake.in
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY)
configure_file(${PROJECT_NAME}ConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" @ONLY)
install(FILES
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
