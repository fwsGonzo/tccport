cmake_minimum_required(VERSION 2.8.9)
project(service)
if (NOT DEFINED ENV{INCLUDEOS_PREFIX})
  set(ENV{INCLUDEOS_PREFIX} /usr/local)
endif()

set(SERVICE_NAME "TCC test")
set(BINARY       "tccport")

# Maximum memory can be hard-coded into the binary
set(MAX_MEM 10)

# Source files to be linked with OS library parts to form bootable image
set(SOURCES
  service.cpp       tcc.cpp
  )

# To add your own include paths:
set(LOCAL_INCLUDES "libtcc")

set(LIBRARIES ../libtcc/libtcc.a)

# list of drivers:
set(DRIVERS
  #virtionet   # Virtio networking
  silent_start
  #heap_debugging
  )

set(PLUGINS)

# include service build script
include($ENV{INCLUDEOS_PREFIX}/includeos/service.cmake)
