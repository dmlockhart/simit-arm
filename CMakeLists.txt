project(SimIt-ARM C CXX)

# Required cmake version
cmake_minimum_required(VERSION 2.6.0)

# global needed variables
set(APPLICATION_NAME ${PROJECT_NAME})

set(APPLICATION_VERSION_MAJOR "3")
set(APPLICATION_VERSION_MINOR "0")
set(APPLICATION_VERSION_PATCH "0")

set(APPLICATION_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}")

message(STATUS "Project name is ${CMAKE_PROJECT_NAME}")
message(STATUS "Project source dir is ${CMAKE_SOURCE_DIR}")
message(STATUS "Project binary dir is ${CMAKE_BINARY_DIR}")
message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")

option(ENABLE_DEVEL "Enable development features" OFF)
option(ENABLE_DECGEN "Build decoder generator" OFF)
option(ENABLE_ISSGEN "Build instruction set generator" OFF)
option(ENABLE_JIT "Enable dynamic-compiled simulation engine" OFF)

add_definitions("-O3 -finline-limit=5000 -fomit-frame-pointer")

set(DECGEN_EXECUTABLE ${CMAKE_BINARY_DIR}/decgen/decgen)
set(ISSGEN_EXECUTABLE ${CMAKE_BINARY_DIR}/issgen/issgen)

if(ENABLE_DEVEL)
  find_package(FLEX)
  find_package(BISON)
endif(ENABLE_DEVEL)

include(CheckIncludeFiles)
CHECK_INCLUDE_FILES("inttypes.h" HAVE_INTTYPES_H)
CHECK_INCLUDE_FILES("stdint.h" HAVE_STDINT_H)
CHECK_INCLUDE_FILES("sys/resource.h" HAVE_SYS_RESOURCE_H)
CHECK_INCLUDE_FILES("sys/time.h" HAVE_SYS_TIME_H)

include(CheckTypeSize)
CHECK_TYPE_SIZE("char" SIZEOF_CHAR)
CHECK_TYPE_SIZE("short" SIZEOF_SHORT)
CHECK_TYPE_SIZE("int" SIZEOF_INT)
CHECK_TYPE_SIZE("long" SIZEOF_LONG)
CHECK_TYPE_SIZE("long long" SIZEOF_LONG_LONG)

include(TestBigEndian)
TEST_BIG_ENDIAN(WORDS_BIGENDIAN)

configure_file(${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_SOURCE_DIR}/config.h)

macro(FLEX_COMMAND input output)
  add_custom_command(
    OUTPUT
      ${output}
    COMMAND
      ${FLEX_EXECUTABLE}
    ARGS
      --outfile=${output} ${input}
    DEPENDS
      ${input}
  )
  set_source_files_properties(${output} GENERATED)
endmacro()

macro(BISON_COMMAND input output defines)
  add_custom_command(
    OUTPUT
      ${output}
      ${defines}
    COMMAND
      ${BISON_EXECUTABLE}
    ARGS
      --output=${output} --defines=${defines} ${input}
    DEPENDS
      ${input}
  )
  set_source_files_properties(${output} GENERATED)
  set_source_files_properties(${defines} GENERATED)
endmacro()

macro(ASSERT_DEVEL)
  foreach(SRC ${ARGN})
    if(NOT EXISTS ${SRC})
      message(FATAL_ERROR "${SRC} not found.
      You should run cmake with -DENABLE_DEVEL=1")
    endif()
  endforeach(SRC)
endmacro()

include_directories(. emulator decgen issgen jit)

if(ENABLE_ISSGEN)
  add_subdirectory(issgen)
endif()

if(ENABLE_DECGEN)
  add_subdirectory(decgen)
endif()

add_subdirectory(emulator)

if(ENABLE_JIT)
  add_subdirectory(jit)
endif()
