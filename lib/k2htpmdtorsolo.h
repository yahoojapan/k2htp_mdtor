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

#ifndef	K2HTPMDTORSOLO_H
#define	K2HTPMDTORSOLO_H

#include <yaml.h>
#include <k2hash/k2htransfunc.h>

#include <string>
#include <list>
#include <map>

#include "k2htpmdtorlibs.h"

//---------------------------------------------------------
// Enums
//---------------------------------------------------------
typedef enum mdtor_mode{
	MDTOR_MODE_ALL,
	MDTOR_MODE_RANDOM,
	MDTOR_MODE_UNTIL_SUCCESS,
	MDTOR_MODE_UNTIL_FAILURE
}MDTORMODE;

//---------------------------------------------------------
// Structure and Typedefs
//---------------------------------------------------------
//
// MDTORINFO
//
typedef struct mdtor_info{
	std::string		libpath;					// transaction plugin shared library path
	std::string		config;						// configuration

	mdtor_info() : libpath(""), config("") {}
}MDTORINFO, *PMDTORINFO;

typedef std::vector<PMDTORINFO>					mdtorinfolist_t;
typedef std::list<std::string>					mdtorstrlst_t;
typedef std::map<std::string, bool>				mdtorstrmap_t;

//---------------------------------------------------------
// K2HtpMdtorSolo Class
//---------------------------------------------------------
class K2HtpMdtorSolo
{
	private:
		static unsigned int	seed;				// for random mode
		static bool			is_init_seed;

		k2h_h				handle;
		std::string			config;
		MDTORMODE			mode;
		mdtortplibs_t		tplibs;

	protected:
		static bool SetMdtorInfoUtil(PMDTORINFO& pLibinfo, mdtorinfolist_t& infolist);
		static bool ReadIniFileContents(const char* filepath, mdtorstrlst_t& linelst, mdtorstrlst_t& allfiles);
		static bool LoadConfiguration(const char* config, MDTORMODE& mode, mdtorinfolist_t& infolist);
		static bool LoadConfigurationIni(const char* filepath, MDTORMODE& mode, mdtorinfolist_t& infolist);
		static bool LoadConfigurationYaml(const char* config, MDTORMODE& mode, mdtorinfolist_t& infolist, bool is_json_string);
		static bool LoadConfigurationYamlTopLevel(yaml_parser_t& yparser, MDTORMODE& mode, mdtorinfolist_t& infolist);
		static bool LoadConfigurationYamlMain(yaml_parser_t& yparser, MDTORMODE& mode);
		static bool LoadConfigurationYamlLibs(yaml_parser_t& yparser, mdtorinfolist_t& infolist);

	public:
		K2HtpMdtorSolo();
		virtual ~K2HtpMdtorSolo();

		bool IsLoad(void) const { return (K2H_INVALID_HANDLE != handle); }

		bool Load(k2h_h hk2h, const char* pconfig);
		bool Unload(void);

		bool DoTransaction(PBCOM pBinCom);
};

typedef std::map<k2h_h, K2HtpMdtorSolo*>	mdtorlibmap_t;

#endif	// K2HTPMDTORSOLO_H

/*
 * VIM modelines
 *
 * vim:set ts=4 fenc=utf-8:
 */
