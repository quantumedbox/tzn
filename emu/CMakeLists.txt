cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME tzn-emu)
project(${PROJECT_NAME} LANGUAGES C)

include(FetchContent)

set(VARIABLES "")
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDE_DIRS} ${SDL2_SOURCE_DIR}/include ../include)

if (NOT TZN_MODE)
  set(TZN_MODE "CLI")
endif()

file(GLOB SOURCE_FILES ./src/*.c ./src/devices/*.c)

# TODO Make TZN_MODE depend on env variable
if (TZN_MODE MATCHES "CLI")
  set(TZN_MAIN "src/main/tzncli.c")
  # TEMP
  list(APPEND SOURCE_FILES "src/devices/trm/tznvt1.c")
  list(APPEND SOURCE_FILES "src/devices/ctr/tznlc.c")
  list(APPEND SOURCE_FILES "src/devices/kbt/tzndos.c")
  list(APPEND VARIABLES "TZN_HTRM=1")
  list(APPEND VARIABLES "TZN_HKBT=1")
else()
  message(SEND_ERROR "Unknown mode '${TZN_MODE}', try one of available: CLI")
endif()

add_executable(${PROJECT_NAME} ${TZN_MAIN})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 90)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD_REQUIRED ON)

if (CMAKE_C_COMPILER_ID MATCHES GNU)
  target_compile_options(${PROJECT_NAME} PRIVATE -g3 -Wall -Wextra -Werror -Wpedantic)
  set(COMPILER_RELEASE_OPTIONS -O3)
else()
  message(WARNING "Don't know how to set warning levels for this compiler")
endif()

if (DEFINED ENV{TZN_DUMP_EXEC_STATE})
  list(APPEND VARIABLES "TZN_DUMP_EXEC_STATE=1")
endif()

if (DEFINED ENV{TZN_RELEASE})
  list(APPEND VARIABLES "TZN_RELEASE=1")
  set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
  target_compile_definitions(${PROJECT_NAME} PRIVATE ${VARIABLES})
  target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILER_RELEASE_OPTIONS})
else()
  list(APPEND VARIABLES "TZN_DEBUG=1")
  target_compile_definitions(${PROJECT_NAME} PRIVATE ${VARIABLES})
endif()
