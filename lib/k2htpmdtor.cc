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

#include <k2hash/k2hutil.h>
#include <k2hash/k2hdbg.h>

#include "k2htpmdtorman.h"

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
// Version
//---------------------------------------------------------
// This value is set in k2htpbaseversion.cc file which is made
// by make_rev.sh script.
//
extern char k2htpmdtor_commit_hash[];

//---------------------------------------------------------
// Transaction functions
//---------------------------------------------------------
// 
// Transaction function
// 
// This function is called from k2hash library at each one transaction doing.
// You can do anything here for one transaction. If you set transaction file
// path in k2h_trans_cntl function, you can get it's file descriptor by calling
// K2HTransManager::GetArchiveFd() function.
// When you get transaction fd, it is locked automatically by k2hash library.
// Thus it is safe for multi thread.
// 
// This function gets BCOM structure pointer, this structure(union) is specified
// in k2hcommand.h. For example, v1.0.12 k2hash defined following:
// 
//		typedef struct serialize_command{
//			char	szCommand[SCOM_COMSTR_LENGTH];
//			long	type;
//			size_t	key_length;
//			size_t	val_length;
//			size_t	skey_length;
//			size_t	attrs_length;
//			size_t	exdata_length;
//			off_t	key_pos;
//			off_t	val_pos;
//			off_t	skey_pos;
//			off_t	attr_pos;
//			off_t	exdata_pos;
//		}K2HASH_ATTR_PACKED SCOM, *PSCOM;
// 
//		typedef union binary_command{
//			SCOM			scom;
//			unsigned char	byData[sizeof(SCOM)];
//		}K2HASH_ATTR_PACKED BCOM, *PBCOM;
//
bool k2h_trans(k2h_h handle, PBCOM pBinCom)
{
	if(!pBinCom){
		return false;
	}

	// do transaction plugins
	if(!K2HtpMdtorManager::Get()->DoTransaction(handle, pBinCom)){
		ERR_K2HPRN("K2HTPMDTOR failed to do transaction plugin for k2hash handle(0x%016" PRIx64 ").", handle);
		return false;
	}
	return true;
}

// 
// Transaction Version function for this plugin
// 
// This function is called from k2hash library when getting transaction plugin version.
// For example, k2hlinetool can display transaction plugin version by info and trans
// command.
//
const char* k2h_trans_version(void)
{
	// Following codes returns this plugin package version number and Github commit hash.
	//
	static string	trans_version("");
	static bool		is_set = false;

	if(!is_set){
		trans_version	=  "k2htpmdtor V";
		trans_version	+= VERSION;
		trans_version	+= "(";
		trans_version	+= k2htpmdtor_commit_hash;
		trans_version	+= ")";
		is_set = true;
	}
	return trans_version.c_str();
}

// 
// Transaction Control function
// 
// This function is called from k2hash library when transaction is enable/disable.
// You can control disable/enable for transaction by setting TRANSOPT structure.
// And if you set enable, you can specify transaction file path and transaction
// queue prefix.
// 
// This function gets TRANSOPT structure pointer which has below member.
//		pOpt->szFilePath[MAX_TRANSACTION_FILEPATH]
//		pOpt->isEnable
//		pOpt->byTransPrefix[MAX_TRANSACTION_PREFIX]
//		pOpt->PrefixLength
//		pOpt->byTransParam[MAX_TRANSACTION_PARAM];
//		pOpt->ParamLength;
// You must set each member value before this function returns.
// 
// isEnable			This value means transaction going to disable or enable.
// 					This function can know it from this member value.
// 					If you set transaction to disable despite this value is true,
//					you can set false to this value before returning.
// szFilePath		When isEnable is only true, this value means transaction file
//					path which is specified at called K2HShm::Transaction().
//					If you need to modify(or change) this value, you can set it
//					in this function.(over write this value)
//					If this value is not NULL, in the k2h_trans function you can get
//					file pointer to this file path by K2HTransManager::GetArchiveFd()
//					function. Then this archive fd is locked and opened, so you
//					can only push data to it.
//					If isEnable is false, you do not have to care this value.
// byTransPrefix	When isEnable is only true, this value means the queue name
//					prefix for transaction in k2hash. This value is set at calling
//					K2HShm::Transaction(). And the value is set in this buffer at
//					calling this handler. If you want to modify(or change) prefix for
//					transaction queue name, you can set(over write) this value in this
//					function. If you use default queue name, you do not have to set this
//					value. If isEnable is false, you do not have to care this value.
// PrefixLength		If you set byTransPrefix, you must set this value for byTransPrefix
//					length.
// byTransParam		When isEnable is only true, this value means the custom parameter
//					which the transaction control function is called with.
// ParamLength		If you set byTransParam, you must set this value for byTransParam
//					length
//
bool k2h_trans_cntl(k2h_h handle, PTRANSOPT pOpt)
{
	if(!pOpt){
		return false;
	}
	if(pOpt->isEnable){
		if('\0' != pOpt->szFilePath[0]){
			WAN_K2HPRN("K2HTPMDTOR does not use transaction file(%s), so clear this.", pOpt->szFilePath);
			memset(pOpt->szFilePath, 0, MAX_TRANSACTION_FILEPATH);
		}
		if(0 < pOpt->PrefixLength){
			MSG_K2HPRN("K2HTPMDTOR change transaction queue prefix.");
		}
		if(0 == pOpt->ParamLength){
			ERR_K2HPRN("K2HTPMDTOR needs parameter for configuration.");
			return false;
		}

		// set transaction plugins for k2hash
		if(!K2HtpMdtorManager::Get()->Add(handle, reinterpret_cast<const char*>(pOpt->byTransParam))){
			ERR_K2HPRN("K2HTPMDTOR could not set k2hash handle(0x%016" PRIx64 ") with configuration(%s).", handle, reinterpret_cast<const char*>(pOpt->byTransParam));
			return false;
		}

	}else{
		// unset transaction plugins for k2hash
		if(!K2HtpMdtorManager::Get()->Remove(handle)){
			ERR_K2HPRN("K2HTPMDTOR failed to unset k2hash handle(0x%016" PRIx64 ").", handle);
			return false;
		}
	}
	return true;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noexpandtab sw=4 ts=4 fdm=marker
 * vim<600: noexpandtab sw=4 ts=4
 */
