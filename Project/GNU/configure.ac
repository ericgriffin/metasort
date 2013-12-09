#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([Metasort], [1.x])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_HEADERS([config.h])

dnl -------------------------------------------------------------------------
dnl sets build, host, target variables and the same with _alias
dnl
AC_CANONICAL_BUILD
AM_INIT_AUTOMAKE([foreign -Wall])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_DISABLE_SHARED
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MKTIME

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
