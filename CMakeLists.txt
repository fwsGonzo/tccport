cmake_minimum_required(VERSION 2.8.9)
project(service)
if (NOT DEFINED ENV{INCLUDEOS_PREFIX})
  set(ENV{INCLUDEOS_PREFIX} /usr/local)
endif()

set(SERVICE_NAME "TCC test")
set(BINARY       "tccport")

# Maximum memory can be hard-coded into the binary
set(MAX_MEM 100)

# Source files to be linked with OS library parts to form bootable image
set(SOURCES
  service.cpp       tcc.cpp
  tinycc/tcc.c      tinycc/libtcc.c   tinycc/tccpp.c
  tinycc/tccgen.c   tinycc/tccelf.c   tinycc/tccasm.c
  tinycc/tccrun.c   tinycc/i386-gen.c tinycc/i386-asm.c
  )
add_definitions(-DTCC_TARGET_I386 -DCONFIG_TCCBOOT -DCONFIG_TCC_NO_RUNTIME)

# To add your own include paths:
set(LOCAL_INCLUDES "include")

# Adding memdisk (expects my.disk to exist in current dir):
#set(MEMDISK .)

# list of drivers:
set(DRIVERS
  #virtionet   # Virtio networking
  silent_start
  #heap_debugging
  )

set(PLUGINS)

# include service build script
include($ENV{INCLUDEOS_PREFIX}/includeos/service.cmake)
