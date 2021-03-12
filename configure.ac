AC_INIT([ksh], [0.1.0], [nikita.skobelevs@gmail.com])

AM_INIT_AUTOMAKE([foreign no-dependencies subdir-objects])

AX_CHECK_COMPILE_FLAG([-std=c99], [
  CFLAGS+=" -std=c99"
])

AC_PROG_CC

AC_CONFIG_FILES([Makefile])

AC_OUTPUT