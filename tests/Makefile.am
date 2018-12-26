#
# k2htpmdtor for K2HASH MULTI TRANSACTION PLUGIN.
#
# Copyright 2016 Yahoo Japan Corporation.
#
# K2HASH TRANSACTION PLUGIN is programable I/F for processing
# transaction data from modifying K2HASH data.
#
# For the full copyright and license information, please view
# the license file that was distributed with this source code.
#
# AUTHOR:   Takeshi Nakatani
# CREATE:   Mon Dec 19 2016
# REVISION:
#

#
# Main test program
#
noinst_PROGRAMS = k2htpmdtortest

k2htpmdtortest_SOURCES = k2htpmdtortest.cc
k2htpmdtortest_LDADD = $(k2hash_LIBS) $(fullock_LIBS)

#
# Dummy transaction plugin shared libraries
#
# [NOTE]
# These library is specified by lib_LTLIBRARIES for creating
# as shared libraries, but these should not be installed.
# Thus (un)install-libLTLIBRARIES is specified here.
#
lib_LTLIBRARIES = libk2htpmdtordmy.la libk2htpmdtordmy2.la
uninstall-libLTLIBRARIES: 
install-libLTLIBRARIES: 

libk2htpmdtordmy_la_SOURCES = k2htpmdtordmy.cc
libk2htpmdtordmy_la_LDFLAGS = -version-info 1:0:0

nodist_libk2htpmdtordmy2_la_SOURCES = k2htpmdtordmy2.cc
libk2htpmdtordmy2_la_LDFLAGS = -version-info 1:0:0

#
# Flags
#
ACLOCAL_AMFLAGS = -I m4
AM_CFLAGS = $(k2hash_CFLAGS) $(fullock_CFLAGS)
AM_CPPFLAGS = $(k2hash_CFLAGS) $(fullock_CFLAGS)

#
# Setting
#
TESTS = test.sh

EXTRA_DIST = test.sh test.ini test.yaml test.json test_json_string.data

#
# copy k2htpmdtordmy.cc to k2htpmdtordmy2.cc
#
.PHONY: k2htpmdtordmy2

k2htpmdtordmy2.cc: k2htpmdtordmy2
	@cp -p k2htpmdtordmy.cc k2htpmdtordmy2.cc

#
# VIM modelines
#
# vim:set ts=4 fenc=utf-8:
#
