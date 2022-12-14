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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include <k2hash/k2htransfunc.h>
#include <k2hash/k2hutil.h>
#include <k2hash/k2hdbg.h>
#include <fullock/flckutil.h>

using namespace std;

//---------------------------------------------------------
// Prototypes
//---------------------------------------------------------
extern "C" {
	bool k2h_trans(k2h_h handle, PBCOM pBinCom);
	const char* k2h_trans_version(void);
	bool k2h_trans_cntl(k2h_h handle, PTRANSOPT pOpt);
}

//---------------------------------------------------------
// Utilities
//---------------------------------------------------------
// [NOTE]
// We use this shared library for testing k2htpmdtor.
// So k2htpmdtor does not allow same file path for shared library,
// we use same libraries by copying this library.
// But we need to see the result and it has uniq key for each library.
// Then we use the function address in main program.
//
static const char* get_uniq_address(void)
{
	static const char*	address = "dummy";
	return address;
}

//---------------------------------------------------------
// Global
//---------------------------------------------------------
// [NOTE]
// To avoid static object initialization order problem(SIOF)
//
static std::string& GetLogFile(void)
{
	static std::string	LogFile("");
	return LogFile;
}

//---------------------------------------------------------
// Transaction functions
//---------------------------------------------------------
// 
// Transaction function
// 
bool k2h_trans(k2h_h handle, PBCOM pBinCom)
{
	if(!pBinCom){
		return false;
	}

	// open log file(default stdout)
	FILE*	fp = stdout;
	if(GetLogFile().empty() || NULL == (fp = fopen(GetLogFile().c_str(), "a+"))){
		// cppcheck-suppress resourceLeak
		fp = stdout;
	}

	// get key and value
	string	key;
	string	val;
	if(0UL < pBinCom->scom.key_length){								// get key
		key.assign(reinterpret_cast<char*>(&(pBinCom->byData[pBinCom->scom.key_pos])), pBinCom->scom.key_length);
	}
	if(0UL < pBinCom->scom.val_length){								// get value
		val.assign(reinterpret_cast<char*>(&(pBinCom->byData[pBinCom->scom.val_pos])), pBinCom->scom.val_length);
	}

	// put
	fprintf(fp, "[%p] %s : %s\n", get_uniq_address(), key.c_str(), val.c_str());
	fflush(fp);
	fclose(fp);

	return true;
}

// 
// Transaction Version function for this plugin
// 
const char* k2h_trans_version(void)
{
	static string	trans_version("");
	static bool		is_set = false;

	if(!is_set){
		//
		// Use uniq function address into version string.
		//
		trans_version	=  "k2htpmdtordmy - 0x";
		trans_version	+= to_hexstring(get_uniq_address());
		is_set = true;
	}
	return trans_version.c_str();
}

// 
// Transaction Control function
// 
bool k2h_trans_cntl(k2h_h handle, PTRANSOPT pOpt)
{
	if(!pOpt){
		return false;
	}
	if(pOpt->isEnable){
		if('\0' != pOpt->szFilePath[0]){
			WAN_K2HPRN("DUMMY TRANSACTION PLUGIN(%p): does not use transaction file(%s), so clear this.", get_uniq_address(), pOpt->szFilePath);
			memset(pOpt->szFilePath, 0, MAX_TRANSACTION_FILEPATH);
		}
		if(0 < pOpt->PrefixLength){
			MSG_K2HPRN("DUMMY TRANSACTION PLUGIN(%p): change transaction queue prefix.", get_uniq_address());
		}
		if(0 == pOpt->ParamLength){
			ERR_K2HPRN("DUMMY TRANSACTION PLUGIN(%p): needs parameter for configuration.", get_uniq_address());
			return false;
		}

		// set log file path
		GetLogFile() = reinterpret_cast<const char*>(pOpt->byTransParam);

	}else{
		// unset transaction plugins for k2hash
		MSG_K2HPRN("DUMMY TRANSACTION PLUGIN(%p): unset k2hash handle(0x%016" PRIx64 ").", get_uniq_address(), handle);
	}
	return true;
}

/*
 * VIM modelines
 *
 * vim:set ts=4 fenc=utf-8:
 */
