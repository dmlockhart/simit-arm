#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([SimIt-ARM], [3.0], [weiqin@gmail.com])
AC_CONFIG_SRCDIR([emulator/main.cpp])
AM_INIT_AUTOMAKE

AM_CONFIG_HEADER(config.h:config.h.in)

#if the user sets CFLAGS
usr_CFLAGS=${CFLAGS+set}

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB
AC_PROG_INSTALL
AM_PROG_LEX
AC_PROG_YACC

AC_ARG_ENABLE([devel],
[  --enable-devel    Turn on development features],
[case "${enableval}" in
  yes) devel=true ;;
  no)  devel=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-devel]) ;;
esac],[devel=false])
AM_CONDITIONAL([ENABLE_DEVEL], [test x$devel = xtrue])

AC_ARG_ENABLE([jit],
[  --enable-jit    Turn on JIT generator],
[case "${enableval}" in
  yes) jit=true ;;
  no)  jit=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-jit]) ;;
esac],[jit=false])
AM_CONDITIONAL([ENABLE_JIT], [test x$jit = xtrue])

cflags="-O3 -finline-limit=5000 -fomit-frame-pointer"

# Check for gmp
#AC_CHECK_LIB(gmp, __gmpz_init, , [AC_MSG_ERROR(
#  [GNU MP not found, see http://swox.com/gmp])])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h netinet/in.h stddef.h stdint.h stdlib.h string.h sys/socket.h sys/time.h sys/resource.h termios.h unistd.h])
AC_PREFIX_DEFAULT("`pwd`/build")

# Checks for typedefs, structures, and compiler characteristics.
# AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_UID_T
AC_C_INLINE
AC_C_BIGENDIAN
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_STRUCT_ST_BLOCKS
AC_CHECK_MEMBERS([struct stat.st_rdev])
AC_HEADER_TIME
AC_CHECK_SIZEOF(char)
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_LSTAT
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_STRTOD
AC_CHECK_FUNCS([atexit inet_ntoa memset mkdir munmap rmdir select socket strcasecmp strchr strdup strncasecmp strrchr strstr strtoul strtoull])

AC_CONFIG_FILES([Makefile
				 emulator/Makefile])
AC_OUTPUT
