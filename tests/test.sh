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

#--------------------------------------------------------------
# Common Variables
#--------------------------------------------------------------
#PRGNAME=$(basename "${0}")
SCRIPTDIR=$(dirname "${0}")
SCRIPTDIR=$(cd "${SCRIPTDIR}" || exit 1; pwd)
SRCTOP=$(cd "${SCRIPTDIR}/.." || exit 1; pwd)

#
# Directories / Files
#
#SRCDIR="${SRCTOP}/src"
LIBDIR="${SRCTOP}/lib"
TESTDIR="${SRCTOP}/tests"
LIBOBJDIR="${LIBDIR}/.libs"
TESTOBJDIR="${TESTDIR}/.libs"

CTPTESTBIN="${TESTDIR}/k2htpmdtortest"
DTORLIBSO="${LIBOBJDIR}/libk2htpmdtor.so.1"

TEST_LOG_FILE1="/tmp/libk2htpmdtordmy.log"
TEST_LOG_FILE2="/tmp/libk2htpmdtordmy2.log"

CFG_TEST_INI_FILE="${TESTDIR}/test.ini"
CFG_TEST_YAML_FILE="${TESTDIR}/test.yaml"
CFG_TEST_JSON_FILE="${TESTDIR}/test.json"
CFG_TEST_JSON_STRING_DATA="${TESTDIR}/test_json_string.data"

#
# LD_LIBRARY_PATH
#
LD_LIBRARY_PATH="${LIBOBJDIR}:${TESTOBJDIR}"
export LD_LIBRARY_PATH

#--------------------------------------------------------------
# Variables and Utility functions
#--------------------------------------------------------------
#
# Escape sequence
#
if [ -t 1 ]; then
	CBLD=$(printf '\033[1m')
	CREV=$(printf '\033[7m')
	CRED=$(printf '\033[31m')
	CYEL=$(printf '\033[33m')
	CGRN=$(printf '\033[32m')
	CDEF=$(printf '\033[0m')
else
	CBLD=""
	CREV=""
	CRED=""
	CYEL=""
	CGRN=""
	CDEF=""
fi

#--------------------------------------------------------------
# Message functions
#--------------------------------------------------------------
PRNTITLE()
{
	echo ""
	echo "${CGRN}---------------------------------------------------------------------${CDEF}"
	echo "${CGRN}${CREV}[TITLE]${CDEF} ${CGRN}$*${CDEF}"
	echo "${CGRN}---------------------------------------------------------------------${CDEF}"
}

PRNERR()
{
	echo "${CBLD}${CRED}[ERROR]${CDEF} ${CRED}$*${CDEF}"
}

# shellcheck disable=SC2317,SC2329
PRNWARN()
{
	echo "${CYEL}${CREV}[WARNING]${CDEF} $*"
}

PRNMSG()
{
	echo "${CYEL}${CREV}[MSG]${CDEF} ${CYEL}$*${CDEF}"
}

PRNINFO()
{
	echo "${CREV}[INFO]${CDEF} $*"
}

PRNSUCCEED()
{
	echo "${CREV}[SUCCEED]${CDEF} $*"
}

#==============================================================
# Main
#==============================================================
#
# Change current
#
cd "${TESTDIR}" || exit 1

#--------------------------------------------------------------
# Test by INI file
#--------------------------------------------------------------
PRNTITLE "TEST INI FILE"

PRNMSG "Initialize test environment for INI file"

if [ -f "${TEST_LOG_FILE1}" ]; then
	rm -f "${TEST_LOG_FILE1}"
fi
if [ -f "${TEST_LOG_FILE2}" ]; then
	rm -f "${TEST_LOG_FILE2}"
fi
PRNINFO "SUCCEED : Initialize test environment for INI file"

#
# Run
#
PRNMSG "Run test for INI file"

if ! "${CTPTESTBIN}" "${DTORLIBSO}" "${CFG_TEST_INI_FILE}"; then
	PRNERR "FAILED : test for INI file"
	exit 1
fi
PRNSUCCEED "SUCCEED : Test for INI file"

#
# Check result
#
PRNMSG "Check result for INI file"

SED_LOG_FILE1=$(sed -e 's/\[0x[0-9a-f]*\] //' "${TEST_LOG_FILE1}" 2> /dev/null)
SED_LOG_FILE2=$(sed -e 's/\[0x[0-9a-f]*\] //' "${TEST_LOG_FILE2}" 2> /dev/null)

if [ -z "${SED_LOG_FILE1}" ] || [ -z "${SED_LOG_FILE2}" ] || [ "${SED_LOG_FILE1}" != "${SED_LOG_FILE2}" ]; then
	PRNERR "FAILED : Check result for INI file"
	exit 1
fi
PRNINFO "SUCCEED : Check result for INI file"

PRNSUCCEED "TEST INI FILE"

#--------------------------------------------------------------
# Test by YAML file
#--------------------------------------------------------------
PRNTITLE "TEST YAML FILE"

PRNMSG "Initialize test environment for YAML file"

if [ -f "${TEST_LOG_FILE1}" ]; then
	rm -f "${TEST_LOG_FILE1}"
fi
if [ -f "${TEST_LOG_FILE2}" ]; then
	rm -f "${TEST_LOG_FILE2}"
fi
PRNINFO "SUCCEED : Initialize test environment for YAML file"

#
# Run
#
PRNMSG "Run test for YAML file"

if ! "${CTPTESTBIN}" "${DTORLIBSO}" "${CFG_TEST_YAML_FILE}"; then
	PRNERR "FAILED : test for YAML file"
	exit 1
fi
PRNSUCCEED "SUCCEED : Test for YAML file"

#
# Check result
#
PRNMSG "Check result for YAML file"

SED_LOG_FILE1=$(sed -e 's/\[0x[0-9a-f]*\] //' "${TEST_LOG_FILE1}" 2> /dev/null)
SED_LOG_FILE2=$(sed -e 's/\[0x[0-9a-f]*\] //' "${TEST_LOG_FILE2}" 2> /dev/null)

if [ -z "${SED_LOG_FILE1}" ] || [ -z "${SED_LOG_FILE2}" ] || [ "${SED_LOG_FILE1}" != "${SED_LOG_FILE2}" ]; then
	PRNERR "FAILED : Check result for YAML file"
	exit 1
fi
PRNINFO "SUCCEED : Check result for YAML file"

PRNSUCCEED "TEST YAML FILE"

#--------------------------------------------------------------
# Test by JSON file
#--------------------------------------------------------------
PRNTITLE "TEST JSON FILE"

PRNMSG "Initialize test environment for JSON file"

if [ -f "${TEST_LOG_FILE1}" ]; then
	rm -f "${TEST_LOG_FILE1}"
fi
if [ -f "${TEST_LOG_FILE2}" ]; then
	rm -f "${TEST_LOG_FILE2}"
fi
PRNINFO "SUCCEED : Initialize test environment for JSON file"

#
# Run
#
PRNMSG "Run test for JSON file"

if ! "${CTPTESTBIN}" "${DTORLIBSO}" "${CFG_TEST_JSON_FILE}"; then
	PRNERR "FAILED : test for JSON file"
	exit 1
fi
PRNSUCCEED "SUCCEED : Test for JSON file"

#
# Check result
#
PRNMSG "Check result for JSON file"

SED_LOG_FILE1=$(sed -e 's/\[0x[0-9a-f]*\] //' "${TEST_LOG_FILE1}" 2> /dev/null)
SED_LOG_FILE2=$(sed -e 's/\[0x[0-9a-f]*\] //' "${TEST_LOG_FILE2}" 2> /dev/null)

if [ -z "${SED_LOG_FILE1}" ] || [ -z "${SED_LOG_FILE2}" ] || [ "${SED_LOG_FILE1}" != "${SED_LOG_FILE2}" ]; then
	PRNERR "FAILED : Check result for JSON file"
	exit 1
fi
PRNINFO "SUCCEED : Check result for JSON file"

PRNSUCCEED "TEST JSON FILE"

#--------------------------------------------------------------
# Test by JSON string
#--------------------------------------------------------------
PRNTITLE "TEST JSON STRING"

PRNMSG "Initialize test environment for JSON string"

if [ -f "${TEST_LOG_FILE1}" ]; then
	rm -f "${TEST_LOG_FILE1}"
fi
if [ -f "${TEST_LOG_FILE2}" ]; then
	rm -f "${TEST_LOG_FILE2}"
fi
if ! JSON_STRING_DATA=$(grep 'JSON_STRING=' "${CFG_TEST_JSON_STRING_DATA}" | sed -e 's/JSON_STRING=//'); then
	PRNERR "Failed initializing for JSON string data."
	exit 1
fi

PRNINFO "SUCCEED : Initialize test environment for JSON string"

#
# Run
#
PRNMSG "Run test for JSON string"

if ! "${CTPTESTBIN}" "${DTORLIBSO}" "${JSON_STRING_DATA}"; then
	PRNERR "FAILED : test for JSON string"
	exit 1
fi
PRNSUCCEED "SUCCEED : Test for JSON string"

#
# Check result
#
PRNMSG "Check result for JSON string"

SED_LOG_FILE1=$(sed -e 's/\[0x[0-9a-f]*\] //' "${TEST_LOG_FILE1}" 2> /dev/null)
SED_LOG_FILE2=$(sed -e 's/\[0x[0-9a-f]*\] //' "${TEST_LOG_FILE2}" 2> /dev/null)

if [ -z "${SED_LOG_FILE1}" ] || [ -z "${SED_LOG_FILE2}" ] || [ "${SED_LOG_FILE1}" != "${SED_LOG_FILE2}" ]; then
	PRNERR "FAILED : Check result for JSON string"
	exit 1
fi
PRNINFO "SUCCEED : Check result for JSON string"

PRNSUCCEED "TEST JSON STRING"

#==============================================================
# Cleanup
#==============================================================
PRNTITLE "CLEANUP"

if [ -f "${TEST_LOG_FILE1}" ]; then
	rm -f "${TEST_LOG_FILE1}"
fi
if [ -f "${TEST_LOG_FILE2}" ]; then
	rm -f "${TEST_LOG_FILE2}"
fi
PRNSUCCEED "CLEANUP"

exit 0

#
# Local variables:
# tab-width: 4
# c-basic-offset: 4
# End:
# vim600: noexpandtab sw=4 ts=4 fdm=marker
# vim<600: noexpandtab sw=4 ts=4
#
