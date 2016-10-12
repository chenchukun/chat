#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(chat, 0.1, chen_chukun@qq.com)
AM_INIT_AUTOMAKE(chat, 0.1, chen_chukun@qq.com)
AC_CONFIG_SRCDIR([include/reactor/Timer.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h stdint.h sys/socket.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([memset uname])

AC_OUTPUT(Makefile
		include/Makefile
		src/Makefile
		)

AC_OUTPUT
