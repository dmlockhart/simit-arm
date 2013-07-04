set(ARMEMU_SRCS
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
  sa_io.cpp
  sa_io.h
  arm_io.h
  armmmu.h
  armmmu.cpp
  tlb.h
  read_elf.h
  auto_disasm.h
  decode.h
  auto_impl.h
  syscall.h
  parse_conf.cpp
  parse_conf.h
  scan_conf.cpp
  simconf.cpp
  simconf.h
  arch.hpp
  arm_iss.cpp
  arm_iss.hpp
  arm_dec.h
  arm_dec2.h
)

ISSGEN_COMMAND()

DECGEN_COMMAND(arm_inst.def arm_dec.h)
DECGEN_COMMAND(arm_inst2.def arm_dec2.h)

FLEX_COMMAND(scan_conf.l scan_conf.cpp)
BISON_COMMAND(parse_conf.y parse_conf.cpp parse_conf.h)


# arm_iss_sys object is compiled with -DSIMIT_SYSTEM_LEVEL enabled
# TODO find more appropriate way to run custom compilation command
set(ARM_ISS_CPP ${CMAKE_CURRENT_SOURCE_DIR}/arm_iss.cpp)
set(ARM_ISS_SYS_OBJ ${CMAKE_CURRENT_BINARY_DIR}/arm_iss_sys.o)
add_custom_command(
  OUTPUT
    ${ARM_ISS_SYS_OBJ}
  COMMAND
    ${CMAKE_CXX_COMPILER}
  ARGS
    -c -I${CMAKE_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/emulator -DSIMIT_SYSTEM_LEVEL
    -o ${ARM_ISS_SYS_OBJ} ${ARM_ISS_CPP}
  DEPENDS
    ${ARM_ISS_CPP}
)

add_library(armemu STATIC ${ARMEMU_SRCS} ${ARM_ISS_SYS_OBJ})

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
