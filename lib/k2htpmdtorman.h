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

#ifndef	K2HTPMDTORMAN_H
#define	K2HTPMDTORMAN_H

#include <fullock/fullock.h>
#include <fullock/flckstructure.h>
#include <fullock/flckbaselist.tcc>

#include "k2htpmdtorsolo.h"

//---------------------------------------------------------
// K2HtpMdtorManager Class
//---------------------------------------------------------
// [TODO]
// Now, K2HtpMdtorManager class does not support reset configuration if it is changed.
// Because this reset logic needs to unload transaction plugin libraries, then we do
// not check all yet.
// This function will be supported in the future.(It will be implemented in this class)
//
class K2HtpMdtorManager
{
	protected:
		flck_rwlock_t	LockVal;					// the value to LibMap(no shared fullock_rwlock)
		mdtorlibmap_t	LibMap;						// All transaction plugin mapping by each k2hash

	protected:
		K2HtpMdtorManager();
		virtual ~K2HtpMdtorManager();

		bool Remove(k2h_h handle, bool is_need_lock);
		bool RemoveAll(void);

	public:
		static K2HtpMdtorManager* Get(void);		// singleton object

		bool Add(k2h_h handle, const char* pconfig);
		bool Remove(k2h_h handle) { return Remove(handle, true); }
		bool DoTransaction(k2h_h handle, PBCOM pBinCom);
};

#endif	// K2HTPMDTORMAN_H

/*
 * VIM modelines
 *
 * vim:set ts=4 fenc=utf-8:
 */
