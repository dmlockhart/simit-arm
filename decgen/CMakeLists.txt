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
  scan_idef.l
  scan_idef.c
  parse_idef.y
  parse_idef.cpp
  parse_idef.h
)

if(ENABLE_DEVEL)
  set(SCAN_IDEF_L ${CMAKE_CURRENT_SOURCE_DIR}/scan_idef.l)
  set(SCAN_IDEF_C ${CMAKE_CURRENT_SOURCE_DIR}/scan_idef.c)
  FLEX_COMMAND(${SCAN_IDEF_L} ${SCAN_IDEF_C})

  set(PARSE_IDEF_Y ${CMAKE_CURRENT_SOURCE_DIR}/parse_idef.y)
  set(PARSE_IDEF_CPP ${CMAKE_CURRENT_SOURCE_DIR}/parse_idef.cpp)
  set(PARSE_IDEF_H ${CMAKE_CURRENT_SOURCE_DIR}/parse_idef.h)
  BISON_COMMAND(${PARSE_IDEF_Y} ${PARSE_IDEF_CPP} ${PARSE_IDEF_H})
else()
  ASSERT_DEVEL(${SCAN_IDEF_C} ${PARSE_IDEF_CPP} ${PARSE_IDEF_H})
endif()

add_executable(decgen ${DECGEN_SRCS})
