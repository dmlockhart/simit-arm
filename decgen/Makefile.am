
DECLEX := scan_idef.l
DECLEX_SRC := scan_idef.c

DECPARSE := parse_idef.y
DECPARSE_SRC := parse_idef.cpp
DECPARSE_HDR := parse_idef.h

#This happens to be a turbulent time for bison.
#Older version 1.28 will generate parse_idef.cpp.h as the definition file.
#While new version  1.35 will generate parse_idef.hpp as the definition file.
#And bison does not allow overriding the default definition file name.
#So I have to do it myself in a stupid way.
$(DECPARSE_SRC): $(DECPARSE)
	@YACC@ -d $^
	mv y.tab.c $@
	mv y.tab.h $(DECPARSE_HDR)

$(DECPARSE_HDR): $(DECPARSE)

$(DECLEX_SRC): $(DECLEX)
	@LEX@ -o$@ $^


noinst_PROGRAMS = decgen32 #decgen64
decgen32_SOURCES = decode.cpp decode_table.cpp huffman.cpp decode_tree.cpp \
	main.cpp decode_theiling.cpp \
	decode.h decode_table.h huffman.h decode_tree.h \
	decode_theiling.h \
	bin_pattern.cpp bin_pattern.hpp undef.cpp \
	inst_type.h $(DECPARSE_SRC) $(DECPARSE_HDR) $(DECLEX_SRC)


HDRS = decode.h decode_table.h huffman.h decode_tree.h decode_theiling.h \
	inst_type.h $(DECPARSE_HDR)

#OBJ64S = decode64.o decode_table64.o huffman64.o decode_tree64.o main64.o \
#	decode_theiling64.o parse_idef64.o scan_idef.o

#decgen64$(EXEEXT): $(OBJ64S)
#	$(CXXLINK) $(LDFLAGS) $(OBJ64S)

#CFLAGS64 = -DINSTSIZE=8

#%64.o :: $(srcdir)/%.cpp
#	$(CXXCOMPILE) $(CFLAGS64) -c $< -o $@

#decode64.o: decode.cpp $(HDRS)
#	$(CXXCOMPILE) $(CFLAGS64) -c decode.cpp -o $@
#
#decode_table64.o: decode_table.cpp $(HDRS)
#	$(CXXCOMPILE) $(CFLAGS64) -c decode_table.cpp -o $@
#
#huffman64.o: huffman.cpp huffman.h 
#	$(CXXCOMPILE) $(CFLAGS64) -c huffman.cpp -o $@
#
#decode_tree64.o: decode_tree.cpp $(HDRS)
#	$(CXXCOMPILE) $(CFLAGS64) -c decode_tree.cpp -o $@
#
#decode_theiling64.o: decode_theiling.cpp $(HDRS)
#	$(CXXCOMPILE) $(CFLAGS64) -c decode_theiling.cpp -o $@
#
#main64.o: main.cpp $(HDRS)
#	$(CXXCOMPILE) $(CFLAGS64) -c $(srcdir)/main.cpp -o $@
#
#parse_idef64.o: $(DECPARSE_SRC) $(HDRS)
#	$(CXXCOMPILE) $(CFLAGS64) -c $(DECPARSE_SRC) -o $@

EXTRA_DIST = $(DECLEX) $(DECPARSE)

INCLUDES = -I..

CLEANFILES = $(DECPARSE_SRC) $(DECPARSE_HDR) $(DECLEX_SRC)


