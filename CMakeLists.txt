cmake_minimum_required(VERSION 2.8.0)
project(Viewer)

# Set this on if you need verbose output from CMake
#set(CMAKE_VERBOSE_MAKEFILE ON)

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

#INSTALL(
#  FILES public/bin/fonts/arial.ttf
#  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
#  DESTINATION ./fonts)

##################   INSTALL PARAMETERS    ################
set(CMAKE_INSTALL_PREFIX install)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/sdk/bin)
set(ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/sdk/lib)
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/sdk/lib)

add_subdirectory(viewer)
add_subdirectory(viewer_app)
