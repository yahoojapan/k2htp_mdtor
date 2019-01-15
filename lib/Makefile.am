#
# k2htpmdtor for K2HASH MULTI TRANSACTION PLUGIN.
#
# Copyright 2016 Yahoo Japan Corporation.
#
# K2HASH TRANSACTION PLUGIN is programmable I/F for processing
# transaction data from modifying K2HASH data.
#
# For the full copyright and license information, please view
# the license file that was distributed with this source code.
#
# AUTHOR:   Takeshi Nakatani
# CREATE:   Mon Dec 19 2016
# REVISION:
#

EXTRA_DIST =

pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = libk2htpmdtor.la

libk2htpmdtor_la_SOURCES = k2htpmdtor.cc k2htpmdtorlibs.cc k2htpmdtorman.cc k2htpmdtorsolo.cc k2htpmdtorversion.cc k2htpmdtorlibs.h k2htpmdtorman.h k2htpmdtorsolo.h
libk2htpmdtor_la_LDFLAGS = -version-info $(LIB_VERSION_INFO)
libk2htpmdtor_la_LIBADD = $(k2hash_LIBS) $(fullock_LIBS) $(chmpx_LIBS) -lyaml

ACLOCAL_AMFLAGS = -I m4
AM_CFLAGS = $(k2hash_CFLAGS) $(fullock_CFLAGS) $(chmpx_CFLAGS)
AM_CPPFLAGS = $(k2hash_CFLAGS) $(fullock_CFLAGS) $(chmpx_CFLAGS)

### version(commit hash)
.PHONY: k2htpmdtorversion

k2htpmdtorversion.cc: k2htpmdtorversion
	@../buildutils/make_commit_hash_source.sh k2htpmdtorversion.cc k2htpmdtor_commit_hash

#
# VIM modelines
#
# vim:set ts=4 fenc=utf-8:
#
