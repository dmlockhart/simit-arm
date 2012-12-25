
AC_DEFUN([AX_IS_GXX], [
AC_REQUIRE([AC_PROG_CXX])

# see if this is g++
  cat >conftest.c <<EOF
#ifndef __GNUC__ 
  choke me
#endif
EOF

	AC_MSG_CHECKING([whether $CXX is g++]) 
	test_compile="$CXX -c conftest.c >&AC_FD_CC"
	if AC_TRY_EVAL(test_compile); then
		rm -f conftest*
		AC_MSG_RESULT([yes])
	else
    	rm -f conftest*
		AC_MSG_RESULT([no])
	fi
])


AC_DEFUN([AX_IS_GXX3OR4], [
AC_REQUIRE([AC_PROG_CXX])

# see if this is g++
  cat >conftest.c <<EOF
#ifdef __GNUC__ 
#if __GNUC__ >= 3
#else
  choke me
#endif
#else
  choke me
#endif
EOF
	AC_MSG_CHECKING([whether $CXX is g++3 or 4]) 
	test_compile="$CXX -c conftest.c >&AC_FD_CC"
	if AC_TRY_EVAL(test_compile); then
		rm -f conftest*
		AC_MSG_RESULT([yes])
	else
    	rm -f conftest*
		AC_MSG_RESULT([no])
	fi
])


AC_DEFUN([AX_CXX_FLAG_WORKS], [
AC_REQUIRE([AC_PROG_CXX])

# see if this is g++
  cat >conftest.c <<EOF
int main() {return 0;}
EOF

	AC_MSG_CHECKING([whether $CXX accepts $1]) 
	test_compile="$CXX $1 -c conftest.c >&AC_FD_CC"
	if AC_TRY_EVAL(test_compile); then
		rm -f conftest*
		AC_MSG_RESULT([yes])
	else
    	rm -f conftest*
		AC_MSG_RESULT([no])
	fi
])


# NOTE: This macro has been submitted for inclusion into   #
#  GNU Autoconf as AC_PROG_SED.  When it is available in   #
#  a released version of Autoconf we should remove this    #
#  macro and use it instead.                               #
# LT_AC_PROG_SED
# --------------
# Check for a fully-functional sed program, that truncates
# as few characters as possible.  Prefer GNU sed if found.
AC_DEFUN([LT_AC_PROG_SED],
[AC_MSG_CHECKING([for a sed that does not truncate output])
AC_CACHE_VAL(lt_cv_path_SED,
[# Loop through the user's path and test for sed and gsed.
# Then use that list of sed's as ones to test for truncation.
as_save_IFS=$IFS; IFS=$PATH_SEPARATOR
for as_dir in $PATH
do
  IFS=$as_save_IFS
  test -z "$as_dir" && as_dir=.
  for lt_ac_prog in sed gsed; do
    for ac_exec_ext in '' $ac_executable_extensions; do
      if $as_executable_p "$as_dir/$lt_ac_prog$ac_exec_ext"; then
        lt_ac_sed_list="$lt_ac_sed_list $as_dir/$lt_ac_prog$ac_exec_ext"
      fi
    done
  done
done
lt_ac_max=0
lt_ac_count=0
# Add /usr/xpg4/bin/sed as it is typically found on Solaris
# along with /bin/sed that truncates output.
for lt_ac_sed in $lt_ac_sed_list /usr/xpg4/bin/sed; do
  test ! -f $lt_ac_sed && break
  cat /dev/null > conftest.in
  lt_ac_count=0
  echo $ECHO_N "0123456789$ECHO_C" >conftest.in
  # Check for GNU sed and select it if it is found.
  if "$lt_ac_sed" --version 2>&1 < /dev/null | grep 'GNU' > /dev/null; then
    lt_cv_path_SED=$lt_ac_sed
    break
  fi
  while true; do
    cat conftest.in conftest.in >conftest.tmp
    mv conftest.tmp conftest.in
    cp conftest.in conftest.nl
    echo >>conftest.nl
    $lt_ac_sed -e 's/a$//' < conftest.nl >conftest.out || break
    cmp -s conftest.out conftest.nl || break
    # 10000 chars as input seems more than enough
    test $lt_ac_count -gt 10 && break
    lt_ac_count=`expr $lt_ac_count + 1`
    if test $lt_ac_count -gt $lt_ac_max; then
      lt_ac_max=$lt_ac_count
      lt_cv_path_SED=$lt_ac_sed
    fi
  done
done
SED=$lt_cv_path_SED
AC_SUBST([SED])
])
AC_MSG_RESULT([$SED])
])

