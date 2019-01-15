#!/bin/sh
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

##############################################################

##############################################################
## library path & programs path
##
CTPTESTSDIR=`dirname $0`
if [ "X${SRCTOP}" = "X" ]; then
	SRCTOP=`cd ${CTPTESTSDIR}/..; pwd`
else
	CTPTESTSDIR=`cd -P ${SRTOP}/tests; pwd`
fi
CTPLIBDIR=`cd -P ${SRCTOP}/lib; pwd`
cd ${CTPTESTSDIR}

if [ "X${OBJDIR}" = "X" ]; then
	LD_LIBRARY_PATH="${CTPLIBDIR}/.libs:${CTPTESTSDIR}/.libs"
else
	LD_LIBRARY_PATH="${CTPLIBDIR}/${OBJDIR}:${CTPTESTSDIR}/${OBJDIR}"
fi
export LD_LIBRARY_PATH

#
# libk2htpmdtor.so.* path
#
if [ -f ${CTPLIBDIR}/${OBJDIR}/libk2htpmdtor.so.0 ]; then
	DTORLIBSO=${CTPLIBDIR}/${OBJDIR}/libk2htpmdtor.so.0
elif [ -f ${CTPLIBDIR}/.libs/libk2htpmdtor.so.0 ]; then
	DTORLIBSO=${CTPLIBDIR}/.libs/libk2htpmdtor.so.0
elif [ -f ${CTPLIBDIR}/libk2htpmdtor.so.0 ]; then
	DTORLIBSO=${CTPLIBDIR}/libk2htpmdtor.so.0
elif [ -f ${CTPLIBDIR}/${OBJDIR}/libk2htpmdtor.so.1 ]; then
	DTORLIBSO=${CTPLIBDIR}/${OBJDIR}/libk2htpmdtor.so.1
elif [ -f ${CTPLIBDIR}/.libs/libk2htpmdtor.so.1 ]; then
	DTORLIBSO=${CTPLIBDIR}/.libs/libk2htpmdtor.so.1
elif [ -f ${CTPLIBDIR}/libk2htpmdtor.so.1 ]; then
	DTORLIBSO=${CTPLIBDIR}/libk2htpmdtor.so.1
else
	echo "ERROR: there is no libk2htpmdtor.so.* binary"
	echo "RESULT --> FAILED"

	exit 1
fi

#
# log files
#
TEST_LOG_FILE1=/tmp/libk2htpmdtordmy.log
TEST_LOG_FILE2=/tmp/libk2htpmdtordmy2.log

#
# test program path
#
CTPTESTBIN=${CTPTESTSDIR}/${OBJDIR}/k2htpmdtortest

##############################################################
## Test by INI file
##
echo ""
echo "====== Start INI file test ================================="

##
## Initialize
##
echo "------ Initialize test environment for INI file ------------"
if [ -f ${TEST_LOG_FILE1} ]; then
	rm -f ${TEST_LOG_FILE1}
fi
if [ -f ${TEST_LOG_FILE2} ]; then
	rm -f ${TEST_LOG_FILE2}
fi
echo "RESULT --> SUCCEED"

##
## Run test
##
echo "------ Start testing for INI file --------------------------"
${CTPTESTBIN} ${DTORLIBSO} ${CTPTESTSDIR}/test.ini

if [ $? -ne 0 ]; then
	echo "ERROR: test program returned false."
	echo "RESULT --> FAILED"

	exit 1
fi
echo "RESULT --> SUCCEED"

##
## Check result
##
echo "------ Check result for INI file ---------------------------"
SED_LOG_FILE1=`sed 's/\[0x[0-9a-f]*\] //' ${TEST_LOG_FILE1} 2> /dev/null`
SED_LOG_FILE2=`sed 's/\[0x[0-9a-f]*\] //' ${TEST_LOG_FILE2} 2> /dev/null`

if [ "X${SED_LOG_FILE1}" = "X" -o "X${SED_LOG_FILE2}" = "X" -o "X${SED_LOG_FILE1}" != "X${SED_LOG_FILE2}" ]; then
	echo "ERROR: test result is false."
	echo "RESULT --> FAILED"

	exit 1
fi
echo "RESULT --> SUCCEED"

##############################################################
## Test by YAML file
##
echo ""
echo "====== Start YAML file test ================================"

##
## Initialize
##
echo "------ Initialize test environment for YAML file -----------"
if [ -f ${TEST_LOG_FILE1} ]; then
	rm -f ${TEST_LOG_FILE1}
fi
if [ -f ${TEST_LOG_FILE2} ]; then
	rm -f ${TEST_LOG_FILE2}
fi
echo "RESULT --> SUCCEED"

##
## Run test
##
echo "------ Start testing for YAML file -------------------------"
${CTPTESTBIN} ${DTORLIBSO} ${CTPTESTSDIR}/test.yaml

if [ $? -ne 0 ]; then
	echo "ERROR: test program returned false."
	echo "RESULT --> FAILED"

	exit 1
fi
echo "RESULT --> SUCCEED"

##
## Check result
##
echo "------ Check result for YAML file --------------------------"
SED_LOG_FILE1=`sed 's/\[0x[0-9a-f]*\] //' ${TEST_LOG_FILE1} 2> /dev/null`
SED_LOG_FILE2=`sed 's/\[0x[0-9a-f]*\] //' ${TEST_LOG_FILE2} 2> /dev/null`

if [ "X${SED_LOG_FILE1}" = "X" -o "X${SED_LOG_FILE2}" = "X" -o "X${SED_LOG_FILE1}" != "X${SED_LOG_FILE2}" ]; then
	echo "ERROR: test result is false."
	echo "RESULT --> FAILED"

	exit 1
fi
echo "RESULT --> SUCCEED"

##############################################################
## Test by JSON file
##
echo ""
echo "====== Start JSON file test ================================"

##
## Initialize
##
echo "------ Initialize test environment for JSON file -----------"
if [ -f ${TEST_LOG_FILE1} ]; then
	rm -f ${TEST_LOG_FILE1}
fi
if [ -f ${TEST_LOG_FILE2} ]; then
	rm -f ${TEST_LOG_FILE2}
fi
echo "RESULT --> SUCCEED"

##
## Run test
##
echo "------ Start testing for JSON file -------------------------"
${CTPTESTBIN} ${DTORLIBSO} ${CTPTESTSDIR}/test.json

if [ $? -ne 0 ]; then
	echo "ERROR: test program returned false."
	echo "RESULT --> FAILED"

	exit 1
fi
echo "RESULT --> SUCCEED"

##
## Check result
##
echo "------ Check result for JSON file --------------------------"
SED_LOG_FILE1=`sed 's/\[0x[0-9a-f]*\] //' ${TEST_LOG_FILE1} 2> /dev/null`
SED_LOG_FILE2=`sed 's/\[0x[0-9a-f]*\] //' ${TEST_LOG_FILE2} 2> /dev/null`

if [ "X${SED_LOG_FILE1}" = "X" -o "X${SED_LOG_FILE2}" = "X" -o "X${SED_LOG_FILE1}" != "X${SED_LOG_FILE2}" ]; then
	echo "ERROR: test result is false."
	echo "RESULT --> FAILED"

	exit 1
fi
echo "RESULT --> SUCCEED"

##############################################################
## Test by JSON string
##
echo ""
echo "====== Start JSON string test =============================="

##
## Initialize
##
echo "------ Initialize test environment for JSON string ---------"
if [ -f ${TEST_LOG_FILE1} ]; then
	rm -f ${TEST_LOG_FILE1}
fi
if [ -f ${TEST_LOG_FILE2} ]; then
	rm -f ${TEST_LOG_FILE2}
fi

JSON_STRING_DATA=`grep 'JSON_STRING=' ${CTPTESTSDIR}/test_json_string.data 2>/dev/null | sed 's/JSON_STRING=//' 2>/dev/null`
if [ $? -ne 0 ]; then
	echo "ERROR: failed initializing for JSON string data."
	echo "RESULT --> FAILED"

	exit 1
fi
echo "RESULT --> SUCCEED"

##
## Run test
##
echo "------ Start testing for JSON string -----------------------"
${CTPTESTBIN} ${DTORLIBSO} "${JSON_STRING_DATA}"

if [ $? -ne 0 ]; then
	echo "ERROR: test program returned false."
	echo "RESULT --> FAILED"

	exit 1
fi
echo "RESULT --> SUCCEED"

##
## Check result
##
echo "------ Check result for JSON string ------------------------"
SED_LOG_FILE1=`sed 's/\[0x[0-9a-f]*\] //' ${TEST_LOG_FILE1} 2> /dev/null`
SED_LOG_FILE2=`sed 's/\[0x[0-9a-f]*\] //' ${TEST_LOG_FILE2} 2> /dev/null`

if [ "X${SED_LOG_FILE1}" = "X" -o "X${SED_LOG_FILE2}" = "X" -o "X${SED_LOG_FILE1}" != "X${SED_LOG_FILE2}" ]; then
	echo "ERROR: test result is false."
	echo "RESULT --> FAILED"

	exit 1
fi
echo "RESULT --> SUCCEED"


##############################################################
## Cleanup
##
echo ""
echo "====== Clear all environment ==============================="

if [ -f ${TEST_LOG_FILE1} ]; then
	rm -f ${TEST_LOG_FILE1}
fi
if [ -f ${TEST_LOG_FILE2} ]; then
	rm -f ${TEST_LOG_FILE2}
fi
echo "RESULT --> SUCCEED"

##############################################################
## Finish
##
echo ""
echo "====== Finish all =========================================="
echo ""
echo "RESULT --> SUCCEED"
echo ""



exit 0

#
# VIM modelines
#
# vim:set ts=4 fenc=utf-8:
#
