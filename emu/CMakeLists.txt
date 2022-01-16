cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME tzn-emu)
project(${PROJECT_NAME} LANGUAGES C)

include(FetchContent)
include(TestBigEndian)

set(VARIABLES "")

test_big_endian(TZN_IS_BIG_ENDIAN)
if(TZN_IS_BIG_ENDIAN)
    list(APPEND VARIABLES "TZN_IS_LITTLE_ENDIAN=0")
else()
    list(APPEND VARIABLES "TZN_IS_LITTLE_ENDIAN=1")
endif()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)

# FetchContent_Declare(
#   SDL2
#   GIT_REPOSITORY https://github.com/libsdl-org/SDL
#   GIT_TAG main
# )
# FetchContent_MakeAvailable(SDL2)

include_directories(${INCLUDE_DIRS} ${SDL2_SOURCE_DIR}/include ../)

if (NOT TZN_MODE)
  set(TZN_MODE "CLI")
endif()

if (TZN_MODE MATCHES "CLI")
  set(TZN_MAIN "src/main-impl/tzn-main-cli.c")
  set(TZN_TERMINAL "src/terminal-impl/tzn-terminal-vt100.c")
  list(APPEND VARIABLES "TZN_HAS_TERMINAL=1")
else()
  message(SEND_ERROR "Unknown mode '${TZN_MODE}', try one of available: CLI")
endif()

file(GLOB SOURCE_FILES ./src/*.c)
add_executable(${PROJECT_NAME} ${TZN_MAIN} ${TZN_TERMINAL} ${SOURCE_FILES})
# target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)

if (CMAKE_C_COMPILER_ID MATCHES GNU)
  set(ALL_WARNINGS -Wall -Wextra -Werror)
else()
  message(WARNING "Don't know how to set warning levels for this compiler")
endif()
target_compile_options(${PROJECT_NAME} PRIVATE ${ALL_WARNINGS})

if (DEFINED ENV{"TZN_DUMP_EXEC_STATE"})
  list(APPEND VARIABLES "TZN_DUMP_EXEC_STATE=1")
endif()

if (CMAKE_BUILD_TYPE MATCHES "Release")
  set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
  target_compile_definitions(${PROJECT_NAME} PRIVATE ${VARIABLES})
endif()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
  list(APPEND VARIABLES "TZN_DEBUG=1")
  target_compile_definitions(${PROJECT_NAME} PRIVATE ${VARIABLES})
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 90)
