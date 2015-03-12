cmake_minimum_required(VERSION 3.0)
get_filename_component(PROJ_DIR_NAME "${CMAKE_SOURCE_DIR}" NAME)
string(SUBSTRING "${PROJ_DIR_NAME}" 3 -1 PROJECT_NAME)
project(${PROJECT_NAME} NONE)

if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type..." FORCE)
endif()
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

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SXC_LIB_ROOT})
include(SXC_Macros)
SXC_SETUP_COMPILER()
enable_language(C)
SXC_SET_COMPILER_FLAGS()

get_filename_component(BOARD_DIR_NAME "${PROJECT_SOURCE_DIR}" DIRECTORY)
get_filename_component(BOARD_DIR_NAME "${BOARD_DIR_NAME}" NAME)
string(TOLOWER "${BOARD_DIR_NAME}" BOARD_DIR_NAME)
if("${BOARD_DIR_NAME}" STREQUAL "xinc2_dev")
  set(HW_CFG "devboard")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__BOARD_XINC2_DEV__")
elseif("${BOARD_DIR_NAME}" STREQUAL "ginger_bamboo")
  set(HW_CFG "ginger")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__BOARD_GINGER__")
else()
  message(ERROR "Undefined board name!")
endif()
file(GLOB_RECURSE SOURCES *.c)
file(GLOB_RECURSE HEADERS *.h*)
foreach(header ${HEADERS})
  get_filename_component(header_dir ${header} DIRECTORY)
  list (APPEND HEADER_DIRS ${header_dir})
endforeach()
if (NOT "${HEADER_DIRS}" STREQUAL "")
  list (REMOVE_DUPLICATES HEADER_DIRS)
endif()
include_directories (${HEADER_DIRS} ${SXC_LIB_ROOT} ${NEWLIB_ROOT} ${LIBCXX_ROOT})

ADD_FIRMWARE_OBJ(${PROJECT_NAME} ${PROJECT_BINARY_DIR} ${HEADERS} ${SOURCES})
SXC_FIRMWARE_OBJ_TO_HEX(out_hex "${PROJECT_SOURCE_DIR}/main.gen"
  ${PROJECT_NAME} "-D" "HW_CFG=${HW_CFG}"
)
