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

macro(ASSERT_DEVEL)
  foreach(SRC ${ARGN})
    if(NOT EXISTS ${SRC})
      message(FATAL_ERROR "${SRC} not found.
      You should re-run cmake with -DENABLE_DEVEL=1")
    endif()
  endforeach()
endmacro()

macro(FLEX_COMMAND inname outname)
  set(input ${CMAKE_CURRENT_SOURCE_DIR}/${inname})
  set(output ${CMAKE_CURRENT_SOURCE_DIR}/${outname})
  if(ENABLE_DEVEL)
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
  else()
    ASSERT_DEVEL(${output})
  endif()
endmacro()

macro(BISON_COMMAND inname outname defname)
  set(input ${CMAKE_CURRENT_SOURCE_DIR}/${inname})
  set(output ${CMAKE_CURRENT_SOURCE_DIR}/${outname})
  set(defines ${CMAKE_CURRENT_SOURCE_DIR}/${defname})
  if(ENABLE_DEVEL)
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
  else()
    ASSERT_DEVEL(${output} ${defines})
  endif()
endmacro()

set(ARM_ISA ${CMAKE_SOURCE_DIR}/issgen/arm.isa)
set(ISS_DEPLOY_SRCS
  ${CMAKE_SOURCE_DIR}/emulator/arm_iss.cpp
  ${CMAKE_SOURCE_DIR}/emulator/arm_iss.hpp
  ${CMAKE_SOURCE_DIR}/emulator/arm_inst.def
  ${CMAKE_SOURCE_DIR}/jit/arm_gen.cpp
  ${CMAKE_SOURCE_DIR}/jit/arm_gen.hpp
)
macro(ISSGEN_COMMAND)
  if(ENABLE_ISSGEN)
    add_custom_command(
      OUTPUT
        ${ISS_DEPLOY_SRCS}
      COMMAND
        ${ISSGEN_EXECUTABLE}
      ARGS
        -p arm ${ARM_ISA}
      COMMAND
        mv
      ARGS
        arm_gen.cpp arm_gen.hpp ../jit
      DEPENDS
        ${ARM_ISA} ${ISSGEN_EXECUTABLE}
      WORKING_DIRECTORY
        ${CMAKE_SOURCE_DIR}/emulator
    )
    foreach(SRC ${ISS_DEPLOY_SRCS})
      set_source_files_properties(${SRC} GENERATED)
    endforeach()
  else()
    foreach(SRC ${ISS_DEPLOY_SRCS})
      if(NOT EXISTS ${output})
        message(FATAL_ERROR "${output} not found.
        You should re-run cmake with -DENABLE_ISSGEN=1")
      endif()
    endforeach()
  endif()
endmacro()

macro(DECGEN_COMMAND inname outname)
  set(input ${CMAKE_CURRENT_SOURCE_DIR}/${inname})
  set(output ${CMAKE_CURRENT_SOURCE_DIR}/${outname})
  if(ENABLE_DECGEN)
    add_custom_command(
      OUTPUT
        ${output}
      COMMAND
        ${DECGEN_EXECUTABLE}
      ARGS
        -i -g 0.25 -o ${output} ${input}
      DEPENDS
        ${DECGEN_EXECUTABLE} ${input}
      WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
    set_source_files_properties(${output} GENERATED)
  else()
    if(NOT EXISTS ${output})
      message(FATAL_ERROR "${output} not found.
      You should re-run cmake with -DENABLE_DECGEN=1")
    endif()
  endif()
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
