configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build_dll.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/build_dll.hpp)

set(DYNARMEMU_SRCS
  dyn_arm_emul.cpp
  dyn_arm_emul.hpp
  arm_iss_jit.cpp
  auto_emitc.h
  arch_gen.hpp
  arm_gen.cpp
  arm_gen.hpp
)

ISSGEN_COMMAND()

add_library(dynarmemu STATIC ${DYNARMEMU_SRCS} ${DYNARMEMU_ISS_SRCS})

set(EMA_JIT_SRCS
  dyn_main.cpp
  dyn_arm_emul.cpp
)

add_executable(ema_jit ${EMA_JIT_SRCS})
set_target_properties(ema_jit PROPERTIES LINK_FLAGS "-export-dynamic -ldl -pthread")
target_link_libraries(ema_jit dynarmemu armemu)
add_dependencies(ema_jit dynarmenu armemu)


set(DECOMP_SERVER_SRCS
  decomp_server_main.cpp
  decomp_server.cpp
  decomp_server.hpp
)

add_executable(decomp_server ${DECOMP_SERVER_SRCS})
set_target_properties(decomp_server PROPERTIES LINK_FLAGS "-ldl")
target_link_libraries(decomp_server dynarmemu armemu)
add_dependencies(decomp_server dynarmenu armemu)

install(
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/arch_jit.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/build_dll.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dyn_arm_emul.hpp
  DESTINATION
    include
)
