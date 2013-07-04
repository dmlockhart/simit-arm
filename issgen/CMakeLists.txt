set(ISSGEN_SRCS
  symbol.cpp
  symbol.hpp
  issgen.hpp
  issgen.cpp
  scan_isa.cpp
  parse_isa.cpp
  parse_isa.h
)

FLEX_COMMAND(scan_isa.l scan_isa.cpp)
BISON_COMMAND(parse_isa.y parse_isa.cpp parse_isa.h)

add_executable(issgen ${ISSGEN_SRCS})
