cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME tzn-emu)
project(${PROJECT_NAME} LANGUAGES C)

set(VARIABLES "")
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)
include_directories(${INCLUDE_DIRS} ${SDL2_SOURCE_DIR}/include ../include)

set(MODE AUTO CACHE STRING "Variants: CLI")
option(DUMP_EXEC_STATE "Debug option for logging every dispatching step" OFF)

if (${MODE} MATCHES AUTO)
  set(MODE CLI)
endif()

# todo: FREE mode that would be able to skip command line parsing in CRT, for example
if(${MODE} MATCHES CLI)
  set(TZN_MAIN "src/main/tzncli.c")
  list(APPEND VARIABLES "-DTZN_MODE=CLI")
else()
  message(SEND_ERROR "Unknown mode '${MODE}', try one of available: CLI")
endif()

add_executable(${PROJECT_NAME} ${TZN_MAIN})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 90)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD_REQUIRED ON)

if(CMAKE_C_COMPILER_ID MATCHES GNU OR CMAKE_C_COMPILER_ID MATCHES Clang)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wpedantic)
  set(COMPILER_RELEASE_OPTIONS -O3)
  set(COMPILER_DEBUG_OPTIONS -g3)
else()
  message(WARNING "Unhandled compiler identity")
endif()

if (DUMP_EXEC_STATE)
  list(APPEND VARIABLES "-DTZN_DUMP_EXEC_STATE=1")
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
  list(APPEND VARIABLES "-DNDEBUG")
  set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
  target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILER_RELEASE_OPTIONS})
  # todo: Invoke `strip`
else()
  target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILER_DEBUG_OPTIONS})
  # Debug
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE ${VARIABLES})
