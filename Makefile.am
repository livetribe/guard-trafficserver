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
AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

AM_CFLAGS = $(OPENSSL_INCLUDES) \
			$(BOOST_CPPFLAGS) \
            -Isrc/include
AM_CPPFLAGS = $(OPENSSL_INCLUDES) \
			  $(BOOST_CPPFLAGS) \
			  -Isrc/include \
 			  -D__STDC_LIMIT_MACROS=1 \
			  -D__STDC_FORMAT_MACROS=1


# build the library
lib_LTLIBRARIES = ltguard.la
ltguard_la_LDFLAGS= -module -avoid-version -shared -lboost_program_options-mt
ltguard_la_LIBADD = @OPENSSL_LIBS@

ltguard_la_SOURCES = \
				src/guard.cc \
				src/utils.cc

library_includedir=$(includedir)/ltguard
base_include_folder = src/include/ltguard/

library_include_HEADERS = \
				$(base_include_folder)/guard.h \
				$(base_include_folder)/utils.h

TESTS = test_guard

check_PROGRAMS = test_guard

test_guard_SOURCES = $(ltguard_la_SOURCES) \
					 tests/test_guard.cc
test_guard_CPPFLAGS = $(AM_CPPFLAGS) -DBOOST_TEST_DYN_LINK
test_guard_LDFLAGS = $(BOOST_LDFLAGS) -lboost_unit_test_framework -lboost_program_options-mt -lcrypto -lssl -ltsmock

