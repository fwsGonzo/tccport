cmake_minimum_required(VERSION 2.8.9)
if (NOT DEFINED ENV{INCLUDEOS_PREFIX})
  set(ENV{INCLUDEOS_PREFIX} /usr/local)
endif()

# Use toolchain (if needed)
set(CMAKE_TOOLCHAIN_FILE $ENV{INCLUDEOS_PREFIX}/includeos/i686-elf-toolchain.cmake)

# Name of your project
project (libseed)

# Name of your IncludeOS library
set(LIBRARY_NAME "tcc")

# Source files to be built into your IncludeOS library
set(SOURCES
  tinycc/tcc.c      tinycc/libtcc.c   tinycc/tccpp.c
  tinycc/tccgen.c   tinycc/tccelf.c   tinycc/tccasm.c
  tinycc/tccrun.c   tinycc/i386-gen.c tinycc/i386-asm.c
  )
add_definitions(-DTCC_TARGET_I386 -DCONFIG_TCCBOOT -DCONFIG_TCC_NO_RUNTIME)

# Necessary includes to build your library
set(LOCAL_INCLUDES
  "include"
  )

# include library build script
include($ENV{INCLUDEOS_PREFIX}/includeos/library.cmake)


# INSTALLATION (OPTIONAL):

# If CMAKE_INSTALL_PREFIX is not set, install to source directory
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

# Where to install library
install(TARGETS ${LIBRARY_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
