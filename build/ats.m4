dnl -------------------------------------------------------- -*- autoconf -*-
dnl Licensed to the Apache Software Foundation (ASF) under one or more
dnl contributor license agreements.  See the NOTICE file distributed with
dnl this work for additional information regarding copyright ownership.
dnl The ASF licenses this file to You under the Apache License, Version 2.0
dnl (the "License"); you may not use this file except in compliance with
dnl the License.  You may obtain a copy of the License at
dnl
dnl     http://www.apache.org/licenses/LICENSE-2.0
dnl
dnl Unless required by applicable law or agreed to in writing, software
dnl distributed under the License is distributed on an "AS IS" BASIS,
dnl WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
dnl See the License for the specific language governing permissions and
dnl limitations under the License.

dnl
dnl ats.m4: LT Guard's Apache Traffic Server autoconf macros
dnl

dnl
dnl LTG_CHECK_ATS: look for Apache Traffic Server libraries and headers
dnl
AC_DEFUN([LTG_CHECK_ATS], [
enable_ats=no
AC_ARG_WITH(ats, [AC_HELP_STRING([--with-ats=DIR],[use a specific Apache Traffic Server library])],
[
  if test "x$withval" != "xyes" && test "x$withval" != "x"; then
    ats_base_dir="$withval"
    if test "$withval" != "no"; then
      enable_ats=yes
      case "$withval" in
      *":"*)
        ats_include="`echo $withval |sed -e 's/:.*$//'`"
        ats_ldflags="`echo $withval |sed -e 's/^.*://'`"
        AC_MSG_CHECKING(checking for Apache Traffic Server includes in $ats_include libs in $ats_ldflags )
        ;;
      *)
        ats_include="$withval/include"
        ats_ldflags="$withval/lib"
        AC_MSG_CHECKING(checking for Apache Traffic Server includes in $withval)
        ;;
      esac
    fi
  fi
])

if test "x$ats_base_dir" = "x"; then
  AC_MSG_CHECKING([for Apache Traffic Server location])
  AC_CACHE_VAL(ats_cv_ats_dir,[
  for dir in /usr/local /usr ; do
    if test -d $dir && test -f $dir/include/ts/ts.h; then
      ats_cv_ats_dir=$dir
      break
    fi
  done
  ])
  ats_base_dir=$ats_cv_ats_dir
  if test "x$ats_base_dir" = "x"; then
    enable_ats=no
    AC_MSG_RESULT([not found])
  else
    enable_ats=yes
    ats_include="$ats_base_dir/include"
    ats_ldflags="$ats_base_dir/lib"
    AC_MSG_RESULT([$ats_base_dir])
  fi
else
  if test -d $ats_include && test -d $ats_ldflags && test -f $ats_include/ts/ts.h; then
    AC_MSG_RESULT([ok])
  else
    AC_MSG_RESULT([not found])
  fi
fi

atsh=0
if test "$enable_ats" != "no"; then
  saved_ldflags=$LDFLAGS
  saved_cppflags=$CPPFLAGS
  ats_have_headers=0
  ats_have_libs=0
  if test "$ats_base_dir" != "/usr"; then
    LTG_ADDTO(CPPFLAGS, [-I${ats_include}])
    LTG_ADDTO(LDFLAGS, [-L${ats_ldflags}])
    LTG_ADDTO(LIBTOOL_LINK_FLAGS, [-rpath ${ats_ldflags}])
  fi
  AC_SEARCH_LIBS([compressBound], [z], [ats_have_libs=1])
  if test "$ats_have_libs" != "0"; then
    LTG_FLAG_HEADERS(ts/ts.h, [ats_have_headers=1])
  fi
  if test "$ats_have_headers" != "0"; then
    AC_SUBST(LIBZ, [-lz])
  else
    enable_ats=no
    CPPFLAGS=$saved_cppflags
    LDFLAGS=$saved_ldflags
  fi
fi
AC_SUBST(atsh)
])
