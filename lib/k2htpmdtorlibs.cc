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

#include <string.h>
#include <dlfcn.h>

#include <k2hash/k2hutil.h>
#include <k2hash/k2hdbg.h>

#include "k2htpmdtorlibs.h"

using namespace std;

//---------------------------------------------------------
// MdtorTpLib : Methods
//---------------------------------------------------------
MdtorTpLib::MdtorTpLib(const char* path, const char* pconfig) : LibPath(""), Config(""), hDynLib(NULL), fp_k2h_trans(NULL), fp_k2h_trans_version(NULL), fp_k2h_trans_cntl(NULL)
{
	if(!ISEMPTYSTR(path)){
		if(!Load(path, pconfig)){
			ERR_K2HPRN("Failed to load library at constructor, but continue and you will get something error after this.");
		}
	}
}

MdtorTpLib::~MdtorTpLib()
{
	if(IsLoaded()){
		MSG_K2HPRN("Called destructor before unload library, thus try to unload it.");
		if(!Unload()){
			ERR_K2HPRN("Failed to unload library at destructor, but continue and you will get something error after this.");
		}
	}
}

bool MdtorTpLib::Unload(void)
{
	if(!IsLoaded()){
		WAN_K2HPRN("Already unloaded library.");
	}else{
		// unload
		MSG_K2HPRN("Unloading library(%s): Transaction function version(%s)", LibPath.c_str(), (fp_k2h_trans_version ? (*fp_k2h_trans_version)() : "unknown"));
		dlclose(hDynLib);
		hDynLib = NULL;
	}

	LibPath.clear();
	Config.clear();
	fp_k2h_trans		= NULL;
	fp_k2h_trans_version= NULL;
	fp_k2h_trans_cntl	= NULL;

	return true;
}

bool MdtorTpLib::Load(const char* path, const char* pconfig)
{
	if(ISEMPTYSTR(path)){
		ERR_K2HPRN("load library path is NULL");
		return false;
	}
	if(pconfig && MAX_TRANSACTION_PARAM < (strlen(pconfig) + 1)){
		ERR_K2HPRN("library configuration string is over %d byte.", MAX_TRANSACTION_PARAM);
		return false;
	}

	// open
	if(NULL == (hDynLib = dlopen(path, RTLD_LAZY))){
		const char*	pError = dlerror();
		ERR_K2HPRN("Failed to load library(%s), error = %s", path, pError ? pError : "unknown");
		return false;
	}

	// check symbol
	const char*	pError = dlerror();		// call because dlerror result is set NULL.
	bool		result = true;
	if(NULL == (fp_k2h_trans = reinterpret_cast<Tfp_k2h_trans>(dlsym(hDynLib, "k2h_trans"))) && NULL != (pError = dlerror())){
		ERR_K2HPRN("Failed to load symbol(k2h_trans), error = %s", pError ? pError : "unknown");
		result = false;
	}else if(NULL == (fp_k2h_trans_version = reinterpret_cast<Tfp_k2h_trans_version>(dlsym(hDynLib, "k2h_trans_version"))) && NULL != (pError = dlerror())){
		ERR_K2HPRN("Failed to load symbol(k2h_trans_version), error = %s", pError ? pError : "unknown");
		result = false;
	}else if(NULL == (fp_k2h_trans_cntl = reinterpret_cast<Tfp_k2h_trans_cntl>(dlsym(hDynLib, "k2h_trans_cntl"))) && NULL != (pError = dlerror())){
		ERR_K2HPRN("Failed to load symbol(k2h_trans_cntl), error = %s", pError ? pError : "unknown");
		result = false;
	}

	if(result){
		MSG_K2HPRN("Success loading library(%s): Transaction function version(%s)", path, (fp_k2h_trans_version ? (*fp_k2h_trans_version)() : "unknown"));
		LibPath	= path;
		Config	= pconfig ? pconfig : "";		// [NOTE] allows NULL
	}else{
		dlclose(hDynLib);
		hDynLib				= NULL;
		fp_k2h_trans		= NULL;
		fp_k2h_trans_version= NULL;
		fp_k2h_trans_cntl	= NULL;
	}
	return result;
}

const char* MdtorTpLib::Version(void)
{
	static const char*	stcDummy = "Unknown k2hash transaction plugin";
	if(!IsLoaded()){
		ERR_K2HPRN("There is no loaded library.");
		return stcDummy;
	}
	if(!fp_k2h_trans_version){
		return stcDummy;
	}

	// call version function for transaction plugin
	return (*fp_k2h_trans_version)();
}

// [NOTE]
// After calling this method, you must check loading library status.
// Because this method unload library when disable it and no more reference it.
//
bool MdtorTpLib::Control(k2h_h handle, bool enable)
{
	if(!IsLoaded()){
		ERR_K2HPRN("There is no loaded library.");
		return false;
	}
	if(fp_k2h_trans_cntl){
		// transaction option data
		TRANSOPT	TrOpt;
		memset(&TrOpt, 0, sizeof(TRANSOPT));
		strncpy(reinterpret_cast<char*>(&(TrOpt.byTransParam[0])), Config.c_str(), MAX_TRANSACTION_PARAM - 1);
		TrOpt.ParamLength	= (Config.length() < MAX_TRANSACTION_PARAM) ? (Config.length() + 1) : MAX_TRANSACTION_PARAM;
		TrOpt.isEnable		= enable;

		// call control function for transaction plugin
		if(!(*fp_k2h_trans_cntl)(handle, &TrOpt)){
			ERR_K2HPRN("Get failure result from %s transaction function for %s", (enable ? "enable" : "disable"), LibPath.c_str());
			return false;
		}
	}
	return true;
}

bool MdtorTpLib::DoTransaction(k2h_h handle, PBCOM pBinCom)
{
	if(!IsLoaded()){
		ERR_K2HPRN("There is no loaded library.");
		return false;
	}

	// call trans function for transaction plugin
	if(fp_k2h_trans && !(*fp_k2h_trans)(handle, pBinCom)){
		ERR_K2HPRN("Get failure result from do transaction function for %s", LibPath.c_str());
		return false;
	}
	return true;
}

/*
 * VIM modelines
 *
 * vim:set ts=4 fenc=utf-8:
 */
