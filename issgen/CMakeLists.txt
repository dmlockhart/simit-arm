set(ISSGEN_SRCS
  symbol.cpp
  symbol.hpp
  issgen.hpp
  issgen.cpp
  scan_isa.l
  scan_isa.cpp
  parse_isa.y
  parse_isa.cpp
  parse_isa.h
)

if(ENABLE_DEVEL)
  set(SCAN_ISA_L ${CMAKE_CURRENT_SOURCE_DIR}/scan_isa.l)
  set(SCAN_ISA_CPP ${CMAKE_CURRENT_SOURCE_DIR}/scan_isa.cpp)
  FLEX_COMMAND(${SCAN_ISA_L} ${SCAN_ISA_CPP})

  set(PARSE_ISA_Y ${CMAKE_CURRENT_SOURCE_DIR}/parse_isa.y)
  set(PARSE_ISA_CPP ${CMAKE_CURRENT_SOURCE_DIR}/parse_isa.cpp)
  set(PARSE_ISA_H ${CMAKE_CURRENT_SOURCE_DIR}/parse_isa.h)
  BISON_COMMAND(${PARSE_ISA_Y} ${PARSE_ISA_CPP} ${PARSE_ISA_H})
else()
  ASSERT_DEVEL(${SCAN_ISA_CPP} ${PARSE_ISA_CPP} ${PARSE_ISA_H})
endif()

add_executable(issgen ${ISSGEN_SRCS})
