# RECOMMENDED BLOCK #
# This block is recommended at the start to ensure that by default, a release
# build is generated. This prevents possible debug code from inflating the size
# of the generated hex image, or from not working altogether!
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type..." FORCE)
endif()
# END OF RECOMMENDED BLOCK #

# REQUIRED BLOCK - MODIFIABLE #
# Default Values from SXC Cross-Compiler Installer - it is recommended to amend
# this to suit the user's install dir for the sake of convenience
if (CMAKE_HOST_WIN32)
  set(SXC_ROOT "C:/Program Files (x86)/SXC")
else()
  set(SXC_ROOT "/usr/local")
endif()
set(SXC_BIN_ROOT "${SXC_ROOT}/bin" CACHE PATH
  "Path to SXC toolchain root with sxc cross-compiler binaries."
)
set(SXC_INCLUDES_ROOT "${SXC_ROOT}/include" CACHE PATH
  "Path to SXC toolchain root with header directories."
)
set(SXC_LIB_ROOT ${SXC_INCLUDES_ROOT}/sxc-lib CACHE PATH
  "Path to header directory of sxc-lib."
)
if(NOT EXISTS ${SXC_LIB_ROOT})
  message(FATAL_ERROR
    "SXC_LIB_ROOT is invalid. ${SXC_LIB_ROOT} does not exist."
  )
endif()
# END OF REQUIRED BLOCK - MODIFIABLE #

# REQUIRED BLOCK - MODIFIABLE #
set(PROJECT_NAME "Wireless_Example_Project") # amend this to suit your project
cmake_minimum_required(VERSION 2.8) # more advanced users may use/require a newer
# version of CMake
# END OF REQUIRED BLOCK - MODIFIABLE #

# REQUIRED BLOCK #
project(${PROJECT_NAME} NONE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SXC_LIB_ROOT})
include(SXC_Macros)
SXC_SETUP_COMPILER()
enable_language(C)
enable_language(CXX)
SXC_SET_COMPILER_FLAGS()
# END OF REQUIRED BLOCK #

# REQUIRED BLOCK - MODIFIABLE #
# This block does a glob-recursive search for all headers and sources in your
# project source directory. Also searches another directory for any headers
# common to multiple example projects. This is not modular, but should suffice
# for very small projects. More sophisticated projects which re-use large blocks
# of code in distinct, unrelated components should use more discriminating logic
# and different variables to store different lists of source and header files.
file (GLOB_RECURSE SOURCES *.c*)
file (GLOB_RECURSE HEADERS *.h)
foreach(header ${HEADERS})
  get_filename_component(header_dir ${header} DIRECTORY)
  list (APPEND HEADER_DIRS ${header_dir})
endforeach()
if (NOT "${HEADER_DIRS}" STREQUAL "")
  list (REMOVE_DUPLICATES HEADER_DIRS)
endif()
# END OF REQUIRED BLOCK - MODIFIABLE #

# REQUIRED BLOCK #
include_directories (${HEADER_DIRS} ${SXC_LIB_ROOT} ${NEWLIB_ROOT} ${LIBCXX_ROOT})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__BOARD_GINGER__")
# END OF REQUIRED BLOCK #

# REQUIRED BLOCK - MODIFIABLE #
# This block fetches all of the header and source files under HEADERS and
# SOURCES and mashes them into a single main static lib which is named whatever
# the PROJECT_NAME is and placed in PROJECT_BINARY_DIR. More sophisticated
# projects which re-use large blocks of code in distinct, unrelated components
# should use additional ADD_FIRMWARE_OBJ calls, link them using
# target_link_libraries(), then compile the main one using
# SXC_FIRMWARE_OBJ_TO_HEX().
ADD_FIRMWARE_OBJ(${PROJECT_NAME} ${PROJECT_BINARY_DIR} ${HEADERS} ${SOURCES})
SXC_FIRMWARE_OBJ_TO_HEX(out_hex "${PROJECT_SOURCE_DIR}/main.gen" ${PROJECT_NAME}
  "-D" "HW_CFG=ginger"
)
# END OF REQUIRED BLOCK - MODIFIABLE #
