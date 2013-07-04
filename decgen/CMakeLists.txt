set(DECGEN_SRCS
  decode.cpp
  decode_table.cpp
  huffman.cpp
  decode_tree.cpp
  main.cpp
  decode_theiling.cpp
  decode.h
  decode_table.h
  huffman.h
  decode_tree.h
  decode_theiling.h
  bin_pattern.cpp
  bin_pattern.hpp
  undef.cpp
  inst_type.h
  scan_idef.c
  parse_idef.cpp
  parse_idef.h
)

FLEX_COMMAND(scan_idef.l scan_idef.c)
BISON_COMMAND(parse_idef.y parse_idef.cpp parse_idef.h)

add_executable(decgen ${DECGEN_SRCS})
