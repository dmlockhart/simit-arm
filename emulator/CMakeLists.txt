set(LIBARMEMU_SRCS
  armemul.cpp
  emumem.cpp
  read_elf.cpp
  syscall.cpp
  loader.h
  loader.cpp
  decode.cpp
  armsupp.cpp
  armcopro.cpp
  armcopro.h
  pxa_io.cpp
  pxa_io.h
  debug.cpp
  util.cpp
  arm_iss.cpp
  arm_iss.hpp
  sa_io.cpp
  sa_io.h
  arm_io.h
  arm_dec.h
  arm_dec2.h
  armmmu.h
  armmmu.cpp
  tlb.h
  read_elf.h
  auto_disasm.h
  decode.h
  auto_impl.h
  syscall.h
  parse_conf.y
  parse_conf.cpp
  parse_conf.h
  scan_conf.l
  scan_conf.cpp
  simconf.cpp
  simconf.h
  arch.hpp
  arm_inst.def
  arm_dec.h
  arm_inst2.def
  arm_dec2.h
)

macro(DECGEN_COMMAND input output)
  add_custom_command(
    OUTPUT
      ${output}
    COMMAND
      ${DECGEN_EXECUTABLE}
    ARGS
      -i -g 0.25 -o ${output} ${input}
    DEPENDS
      ${DECGEN_EXECUTABLE} ${input}
  )
  set_source_files_properties(${output} GENERATED)
endmacro()

set(ARM_INST_DEF ${CMAKE_CURRENT_SOURCE_DIR}/arm_inst.def)
set(ARM_INST2_DEF ${CMAKE_CURRENT_SOURCE_DIR}/arm_inst2.def)
set(ARM_DEC_H ${CMAKE_CURRENT_SOURCE_DIR}/arm_dec.h)
set(ARM_DEC2_H ${CMAKE_CURRENT_SOURCE_DIR}/arm_dec2.h)
if(ENABLE_DECGEN)
  DECGEN_COMMAND(${ARM_INST_DEF} ${ARM_DEC_H})
  DECGEN_COMMAND(${ARM_INST2_DEF} ${ARM_DEC2_H})
else()
  foreach({SRC ${ARM_DEC_H} ${ARM_DEC2_H})
    if(NOT EXISTS ${SRC})
      message(FATAL_ERROR "${SRC} not found.
      You should re-run cmake with -DENABLE_DECGEN=1")
    endif()
  endforeach()
endif()

set(SCAN_CONF_L ${CMAKE_CURRENT_SOURCE_DIR}/scan_conf.l)
set(SCAN_CONF_CPP ${CMAKE_CURRENT_SOURCE_DIR}/scan_conf.cpp)
set(PARSE_CONF_Y ${CMAKE_CURRENT_SOURCE_DIR}/parse_conf.y)
set(PARSE_CONF_CPP ${CMAKE_CURRENT_SOURCE_DIR}/parse_conf.cpp)
set(PARSE_CONF_H ${CMAKE_CURRENT_SOURCE_DIR}/parse_conf.h)
if(ENABLE_DEVEL)
  FLEX_COMMAND(${SCAN_CONF_L} ${SCAN_CONF_CPP})
  BISON_COMMAND(${PARSE_CONF_Y} ${PARSE_CONF_CPP} ${PARSE_CONF_H})
else()
  ASSERT_DEVEL(${SCAN_CONF_CPP} ${PARSE_CONF_CPP} ${PARSE_CONF_H})
endif()

set(EMU_ISS_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/arm_iss.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/arm_iss.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/arm_inst.def
  ${CMAKE_CURRENT_SOURCE_DIR}/arm_gen.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/arm_gen.hpp
)
set(ARM_ISA ${CMAKE_CURRENT_SOURCE_DIR}/arm.isa)
if(ENABLE_ISSGEN)
  add_custom_command(
    OUTPUT
      ${EMU_ISS_SRCS}
    COMMAND
      ${ISSGEN_EXECUTABLE}
    ARGS
      -p arm ${ARM_ISA}
    DEPENDS
      ${ARM_ISA} ${ISSGEN_EXECUTABLE}
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_SOURCE_DIR}
  )
  foreach(SRC ${EMU_ISS_SRCS})
    set_source_files_properties(${SRC} GENERATED)
  endforeach()
else()
  foreach(SRC ${EMU_ISS_SRCS})
    if(NOT EXISTS ${SRC})
      message(FATAL_ERROR "${SRC} not found.
      You should run cmake with -DENABLE_ISSGEN=1")
    endif()
  endforeach()
endif()

# arm_iss_sys object is compiled with -DSIMIT_SYSTEM_LEVEL enabled
# TODO find more appropriate way to run custom compilation command
set(ARM_ISS_SYS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/arm_iss.cpp)
set(ARM_ISS_SYS_OBJ ${CMAKE_CURRENT_BINARY_DIR}/arm_iss_sys.o)
add_custom_command(
  OUTPUT
    ${ARM_ISS_SYS_OBJ}
  COMMAND
    ${CMAKE_CXX_COMPILER}
  ARGS
    -c -I${CMAKE_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/emulator -DSIMIT_SYSTEM_LEVEL
    -o ${ARM_ISS_SYS_OBJ} ${ARM_ISS_SYS_SRC}
  DEPENDS
    ${ARM_ISS_SYS_SRC}
)

add_library(armemu STATIC ${LIBARMEMU_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/arm_iss_sys.o)

set(EMULATOR_SRCS
  main.cpp
)

add_executable(ema ${EMULATOR_SRCS})
target_link_libraries(ema armemu)
add_dependencies(ema armemu)

install(
  PROGRAMS
    ${CMAKE_CURRENT_BINARY_DIR}/ema
  DESTINATION
    bin
)

install(
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/arch.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/arm_io.h
    ${CMAKE_CURRENT_SOURCE_DIR}/armcopro.h
    ${CMAKE_CURRENT_SOURCE_DIR}/armemul.h
    ${CMAKE_CURRENT_SOURCE_DIR}/armmmu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bittypes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/emumem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/misc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tlb.h
  DESTINATION
    include
)
