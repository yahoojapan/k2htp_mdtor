/*
 * k2htpmdtor for K2HASH MULTI TRANSACTION PLUGIN.
 *
 * Copyright 2016 Yahoo Japan Corporation.
 *
 * K2HASH TRANSACTION PLUGIN is programable I/F for processing
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

#include <k2hash/k2hutil.h>
#include <k2hash/k2hdbg.h>

#include "k2htpmdtorman.h"

using namespace std;

//---------------------------------------------------------
// K2HtpMdtorManager Class Methods
//---------------------------------------------------------
// [NOTE]
// To avoid static object initialization order problem(SIOF)
//
K2HtpMdtorManager* K2HtpMdtorManager::Get(void)
{
	static K2HtpMdtorManager	mdtorman;			// singleton
	return &mdtorman;
}

//---------------------------------------------------------
// K2HtpMdtorManager : Methods
//---------------------------------------------------------
K2HtpMdtorManager::K2HtpMdtorManager() : LockVal(FLCK_RWLOCK_UNLOCK)
{
}

K2HtpMdtorManager::~K2HtpMdtorManager()
{
	RemoveAll();
}

bool K2HtpMdtorManager::Add(k2h_h handle, const char* pconfig)
{
	if(K2H_INVALID_HANDLE == handle || ISEMPTYSTR(pconfig)){
		ERR_K2HPRN("Parameter is wrong.");
		return false;
	}

	while(0 != fullock::fl_trywrlock_rwlock(&LockVal));				// LOCK RWLOCK

	// if exists, unload it
	if(!Remove(handle, false)){
		return false;
	}

	// load transaction plugins
	K2HtpMdtorSolo*	pSolo = new K2HtpMdtorSolo();
	if(!pSolo->Load(handle, pconfig)){
		ERR_K2HPRN("Could not build new transaction plugins for k2hash handle(0x%016" PRIx64 ")", handle);
		K2H_Delete(pSolo);
		fullock::fl_unlock_rwlock(&LockVal);						// UNLOCK RWLOCK
		return false;
	}

	// set transaction plugins to map
	LibMap[handle] = pSolo;

	fullock::fl_unlock_rwlock(&LockVal);							// UNLOCK RWLOCK

	return true;
}

bool K2HtpMdtorManager::Remove(k2h_h handle, bool is_need_lock)
{
	if(K2H_INVALID_HANDLE == handle){
		ERR_K2HPRN("Parameter is wrong.");
		return false;
	}

	if(is_need_lock){
		while(0 != fullock::fl_trywrlock_rwlock(&LockVal));			// LOCK RWLOCK
	}

	// if exists, unload it
	mdtorlibmap_t::iterator	iter = LibMap.find(handle);
	if(iter != LibMap.end()){
		// do unload
		K2HtpMdtorSolo*	pSolo	= iter->second;
		iter->second			= NULL;
		if(pSolo){
			if(!pSolo->Unload()){
				ERR_K2HPRN("Failed to unload transaction plugins for k2hash handle(0x%016" PRIx64 ")", handle);
				if(is_need_lock){
					fullock::fl_unlock_rwlock(&LockVal);			// UNLOCK RWLOCK
				}
				return false;
			}
			K2H_Delete(pSolo);
		}
		LibMap.erase(iter);
	}else{
		MSG_K2HPRN("There is no transaction plugins for k2hash handle(0x%016" PRIx64 ")", handle);
	}

	if(is_need_lock){
		fullock::fl_unlock_rwlock(&LockVal);						// UNLOCK RWLOCK
	}
	return true;
}

bool K2HtpMdtorManager::RemoveAll(void)
{
	while(0 != fullock::fl_trywrlock_rwlock(&LockVal));				// LOCK RWLOCK

	for(mdtorlibmap_t::iterator iter = LibMap.begin(); iter != LibMap.end(); LibMap.erase(iter++)){
		k2h_h			handle	= iter->first;
		K2HtpMdtorSolo*	pSolo	= iter->second;
		iter->second			= NULL;
		if(pSolo){
			if(!pSolo->Unload()){
				ERR_K2HPRN("Failed to unload transaction plugins for k2hash handle(0x%016" PRIx64 ")", handle);
				fullock::fl_unlock_rwlock(&LockVal);				// UNLOCK RWLOCK
				return false;
			}
			K2H_Delete(pSolo);
		}
	}
	fullock::fl_unlock_rwlock(&LockVal);							// UNLOCK RWLOCK

	return true;
}

bool K2HtpMdtorManager::DoTransaction(k2h_h handle, PBCOM pBinCom)
{
	if(K2H_INVALID_HANDLE == handle){
		ERR_K2HPRN("Parameter is wrong.");
		return false;
	}

	while(0 != fullock::fl_tryrdlock_rwlock(&LockVal));				// LOCK RWLOCK

	mdtorlibmap_t::iterator	iter = LibMap.find(handle);
	if(iter != LibMap.end()){
		K2HtpMdtorSolo*	pSolo = iter->second;
		if(pSolo){
			if(!pSolo->DoTransaction(pBinCom)){
				ERR_K2HPRN("Failed to do transaction for k2hash handle(0x%016" PRIx64 ")", handle);
				fullock::fl_unlock_rwlock(&LockVal);				// UNLOCK RWLOCK
				return false;
			}
		}
	}else{
		MSG_K2HPRN("There is no transaction plugins for k2hash handle(0x%016" PRIx64 ")", handle);
	}
	fullock::fl_unlock_rwlock(&LockVal);							// UNLOCK RWLOCK

	return true;
}

/*
 * VIM modelines
 *
 * vim:set ts=4 fenc=utf-8:
 */
