AC_INIT([readahead-utils], [0.1.1], [trbecker@gmail.com])
AC_PROG_CC
AC_PROG_LEX
AC_PROG_YACC
AC_PROG_RANLIB
AC_CHECK_FUNCS([memset strdup strtol])
AC_CHECK_HEADERS([fcntl.h inttypes.h libintl.h limits.h malloc.h stddef.h stdint.h stdlib.h string.h unistd.h])
AC_CHECK_TYPES([ptrdiff_t])
AC_FUNC_ALLOCA
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_HEADER_MAJOR
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_FILES([
		src/Makefile
		Makefile util/udev/Makefile
		libparser/Makefile
		test/parser/Makefile
		libdevice/Makefile
		])
AC_OUTPUT
