/*
 * k2htpmdtor for K2HASH MULTI TRANSACTION PLUGIN.
 *
 * Copyright 2016 Yahoo Japan Corporation.
 *
 * K2HASH TRANSACTION PLUGIN is programmable I/F for processing
 * transaction data from modifying K2HASH data.
 *
 * For the full copyright and license information, please view
 * the license file that was distributed with this source code.
 *
 * AUTHOR:   Takeshi Nakatani
 * CREATE:   Mon Dec 19 2016
 * REVISION:
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <k2hash/k2hshm.h>
#include <k2hash/k2htransfunc.h>

#include <iostream>

using namespace std;

//---------------------------------------------------------
// Utilities
//---------------------------------------------------------
static inline void PRN(const char* format, ...)
{
	if(format){
		va_list ap;
		va_start(ap, format);
		vfprintf(stdout, format, ap); 
		va_end(ap);
	}
	fprintf(stdout, "\n");
}

static inline void ERR(const char* format, ...)
{
	fprintf(stderr, "[ERR] ");
	if(format){
		va_list ap;
		va_start(ap, format);
		vfprintf(stderr, format, ap); 
		va_end(ap);
	}
	fprintf(stderr, "\n");
}

static inline string programname(const char* prgname)
{
	string	strname("program");
	if(!prgname){
		return strname;
	}
	char*	pFree = strdup(prgname);
	char*	pName = pFree;
	pName = basename(pName);
	if(0 == strncmp(pName, "lt-", strlen("lt-"))){
		pName = &pName[strlen("lt-")];
	}
	strname = pName;
	K2H_Free(pFree);
	return strname;
}

//
// Usage: k2htpmdtortest <path for libk2htpmdtor.so.*> <configuration>
//
static void print_usage(const char* prgname)
{
	cout << endl;
	cout << "Usage: " << programname(prgname) << " <path for libk2htpmdtor.so.*> <configuration>" << endl;
	cout << endl;
}

//---------------------------------------------------------
// Main
//---------------------------------------------------------
int main(int argc, char** argv)
{
	if(argc < 3){
		ERR("No specified options.");
		print_usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	K2HShm  k2hash;
	if(!k2hash.Attach(NULL, false, true, false, true)){
		ERR("failed to initialize k2hash(memory type).");
		exit(EXIT_FAILURE);
	}

	// load libk2htpmdtor.so.*
	if(!K2HTransDynLib::get()->Load(argv[1])){
		ERR("failed to load transaction library.");
		k2hash.Detach();
		exit(EXIT_FAILURE);
	}
	PRN("--- Loaded multi transaction plugin(%s)",			argv[1]);
	PRN("    Loaded function addr(k2h_trans):        %p",	K2HTransDynLib::get()->get_k2h_trans());
	PRN("    Loaded function addr(k2h_trans_version):%p",	K2HTransDynLib::get()->get_k2h_trans_version());
	PRN("    Loaded function addr(k2h_trans_cntl):   %p",	K2HTransDynLib::get()->get_k2h_trans_cntl());
	PRN("");

	// enable multi transaction plugin
	if(!k2hash.EnableTransaction(NULL, NULL, 0L, reinterpret_cast<const unsigned char*>(argv[2]), strlen(argv[2]) + 1)){
		ERR("Could not enable multi transaction plugin with config(%s).", argv[2]);

		K2HTransDynLib::get()->Unload();
		k2hash.Detach();
		exit(EXIT_FAILURE);
	}

	// transaction test by writing data
	if(!k2hash.Set("TESTKEY", "TESTVALUE")){
		ERR("failed to set key to k2hash.");

		K2HTransDynLib::get()->Unload();
		k2hash.Detach();
		exit(EXIT_FAILURE);
	}
	K2HTransDynLib::get()->Unload();
	k2hash.Detach();

	exit(EXIT_SUCCESS);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noexpandtab sw=4 ts=4 fdm=marker
 * vim<600: noexpandtab sw=4 ts=4
 */
