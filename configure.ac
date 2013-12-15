#
# Copyright 2014 the original author or authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#

AC_PREREQ([2.63])
AC_INIT([libltguard], [1.0.0], [dev@livetribe.org], [ltguard])
AC_CONFIG_SRCDIR([src/guard.cc])
AM_INIT_AUTOMAKE([1.10 -Wall -Werror no-define foreign])


AC_CONFIG_MACRO_DIR([m4])

# This makes the make step quiet, you can override with V=1 w/ make.
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_MACRO_DIR([build])

AC_CONFIG_FILES([Makefile])

ifdef([AM_PROG_AR],
      [AM_PROG_AR])

LT_INIT

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.

AC_CHECK_LIB([rt], [clock_gettime])
AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([zlib], [deflate])


AC_DEFINE([MAC_OS_X_VERSION_MIN_REQUIRED], [MAC_OS_X_VERSION_10_6])

#
# Check for Apache Traffic Server library
#
LTG_CHECK_ATS
if test "x${enable_ats}" != "xyes"; then
  AC_MSG_ERROR([Cannot find Apache Traffic Server library. Configure --with-ats=DIR])
fi

#
# Check for OpenSSL
#
AX_CHECK_OPENSSL([], [AC_MSG_ERROR([OpenSSL header evp.h not found; check OpenSSL installation.])])

#
# Check for Boost.
#
AX_BOOST_BASE([1.33],
  [ AM_CONDITIONAL([BUILD_HAVE_BOOST], [true]) ],
  [AC_MSG_ERROR([Boost C++ libraries not found; check Boost installation.])]
)

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h sys/time.h unistd.h pthread.h stdint.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_INT64_T
AC_TYPE_UINT64_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([clock_gettime gettimeofday memset select socket strerror])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h sys/socket.h sys/time.h sys/select.h sys/types.h unistd.h stdio.h ts/ts.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_OUTPUT
