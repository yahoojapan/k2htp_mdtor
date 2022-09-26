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

#ifndef	K2HTPMDTORLIBS_H
#define	K2HTPMDTORLIBS_H

#include <k2hash/k2htransfunc.h>

#include <string>
#include <vector>

//---------------------------------------------------------
// MdtorTpLib Class
//---------------------------------------------------------
class MdtorTpLib
{
	private:
		std::string				LibPath;				// standard or custom k2hash transaction plugin path
		std::string				Config;
		void*					hDynLib;				// loaded library handle

		Tfp_k2h_trans			fp_k2h_trans;
		Tfp_k2h_trans_version	fp_k2h_trans_version;
		Tfp_k2h_trans_cntl 		fp_k2h_trans_cntl;

	protected:
		bool Control(k2h_h handle, bool enable);

	public:
		explicit MdtorTpLib(const char* path = NULL, const char* pconfig = NULL);
		virtual ~MdtorTpLib();

		bool IsLoaded(void) const { return (NULL != hDynLib); }
		bool Unload(void);
		bool Load(const char* path, const char* pconfig);

		const char* Version(void);
		bool Enable(k2h_h handle) { return Control(handle, true); }
		bool Disable(k2h_h handle) { return Control(handle, false); }
		bool DoTransaction(k2h_h handle, PBCOM pBinCom);
};

typedef std::vector<MdtorTpLib*>	mdtortplibs_t;

#endif	// K2HTPMDTORLIBS_H

/*
 * VIM modelines
 *
 * vim:set ts=4 fenc=utf-8:
 */
