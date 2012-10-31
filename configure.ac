AC_PREREQ([2.68])
AC_INIT([global-trainer-gps],[0.0.1],[agpotter@gmail.com])
AC_CONFIG_SRCDIR([src/])

AC_CONFIG_MACRO_DIR([m4/])
AM_INIT_AUTOMAKE

AC_PROG_CXX
AC_LANG([C++])

AX_CXX_COMPILE_STDCXX_11([noext])

PKG_CHECK_MODULES([LIBUSB], [libusb-1.0 >= 1.0.9])


AC_SUBST([LIBUSB_LIBS])
AC_SUBST([LIBUSB_CFLAGS])

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 src/Makefile])

AC_OUTPUT
