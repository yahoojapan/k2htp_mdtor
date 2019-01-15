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

SUBDIRS=docs lib tests buildutils
EXTRA_DIST=RELEASE_VERSION

CPPCHECK_CMD=	cppcheck
CPPCHECK_OPT=	--quiet \
				--error-exitcode=1 \
				--inline-suppr \
				-j 4 \
				--std=c++03 \
				--xml \
				--enable=warning,style,information,missingInclude
CPPCHECK_IGN=

cppcheck:
	$(CPPCHECK_CMD) $(CPPCHECK_OPT) $(CPPCHECK_IGN) $(SUBDIRS)

#
# VIM modelines
#
# vim:set ts=4 fenc=utf-8:
#
