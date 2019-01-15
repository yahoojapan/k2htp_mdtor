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

#include <k2hash/k2hutil.h>
#include <k2hash/k2hdbg.h>
#include <chmpx/chmconfutil.h>
#include <fullock/flckutil.h>

#include <fstream>

#include "k2htpmdtorsolo.h"

using namespace std;

//---------------------------------------------------------
// Symbols
//---------------------------------------------------------
#define	INICFG_INCLUDE_STR					"INCLUDE"
#define	INICFG_KV_SEP						"="
#define	INICFG_COMMENT_CHAR					'#'
#define	INICFG_SEC_START_CHAR				'['
#define	INICFG_SEC_END_CHAR					']'
#define	INICFG_VAL_SEPARATOR_CHAR			','

#define	CFG_K2HTPMDTOR_SEC_STR				"K2HTPMDTOR"
#define	CFG_K2HTPMDTOR_TP_SEC_STR			"K2HTPMDTOR_TP"

#define	INICFG_K2HTPMDTOR_SEC_STR			"[" CFG_K2HTPMDTOR_SEC_STR "]"
#define	INICFG_K2HTPMDTOR_TP_SEC_STR		"[" CFG_K2HTPMDTOR_TP_SEC_STR "]"

#define	INICFG_K2HTPMDTOR_MODE_STR			"RUN_MODE"
#define	INICFG_K2HTPMDTOR_TPPATH_STR		"TPPATH"
#define	INICFG_K2HTPMDTOR_TPCONF_STR		"TPCONF"

#define	INICFG_K2HTPMDTOR_VAL_ALL			"ALL"
#define	INICFG_K2HTPMDTOR_VAL_RANDOM		"RANDOM"
#define	INICFG_K2HTPMDTOR_VAL_UNTIL_SUCC	"UNTIL_SUCCESS"
#define	INICFG_K2HTPMDTOR_VAL_UNTIL_FAIL	"UNTIL_FAILURE"

#define	K2HTPMDTOR_K2HTPDTOR_LIB			"libk2htpdtor.so.1"

//---------------------------------------------------------
// Utilities
//---------------------------------------------------------
inline void Free_MdtorInfoList(mdtorinfolist_t& infolist)
{
	for(mdtorinfolist_t::iterator iter = infolist.begin(); iter != infolist.end(); iter = infolist.erase(iter)){
		PMDTORINFO	pLibInfo = *iter;
		if(pLibInfo){
			K2H_Delete(pLibInfo);
		}
	}
}

inline bool Check_MdtorInfoList(const mdtorinfolist_t& infolist)
{
	mdtorstrmap_t	tmpmap;
	for(mdtorinfolist_t::const_iterator iter = infolist.begin(); iter != infolist.end(); ++iter){
		const MDTORINFO*	pLibInfo = *iter;
		if(!pLibInfo){
			ERR_K2HPRN("The library path is NULL, there is something wrong codes.");
			return false;
		}
		if(tmpmap.end() != tmpmap.find(pLibInfo->libpath)){
			ERR_K2HPRN("Same library path in configuration.");
			return false;
		}
		tmpmap[pLibInfo->libpath] = true;
	}
	return true;
}

//---------------------------------------------------------
// K2HtpMdtorSolo Class
//---------------------------------------------------------
unsigned int	K2HtpMdtorSolo::seed		= 0;
bool			K2HtpMdtorSolo::is_init_seed= false;

//---------------------------------------------------------
// K2HtpMdtorSolo : Class Methods
//---------------------------------------------------------
bool K2HtpMdtorSolo::LoadConfigration(const char* config, MDTORMODE& mode, mdtorinfolist_t& infolist)
{
	// get configuration type without environment
	CHMCONFTYPE	conftype = check_chmconf_type(config);
	if(CHMCONF_TYPE_UNKNOWN == conftype || CHMCONF_TYPE_NULL == conftype){
		ERR_K2HPRN("Parameter configuration file or json string is wrong.");
		return false;
	}

	bool	result;
	if(CHMCONF_TYPE_INI_FILE == conftype){
		result = K2HtpMdtorSolo::LoadConfigrationIni(config, mode, infolist);
	}else{
		result = K2HtpMdtorSolo::LoadConfigrationYaml(config, mode, infolist, (CHMCONF_TYPE_JSON_STRING == conftype));
	}
	if(!result){
		Free_MdtorInfoList(infolist);
	}
	return result;
}

bool K2HtpMdtorSolo::SetMdtorInfoUtil(PMDTORINFO& pLibinfo, mdtorinfolist_t& infolist)
{
	if(!pLibinfo){
		// nothing to push
		return true;
	}
	if(pLibinfo->config.empty()){
		ERR_K2HPRN("%s section does not have configuration key and value.", INICFG_K2HTPMDTOR_TP_SEC_STR);
		K2H_Delete(pLibinfo);
		return false;
	}
	if(pLibinfo->libpath.empty()){
		pLibinfo->libpath = K2HTPMDTOR_K2HTPDTOR_LIB;
	}
	infolist.push_back(pLibinfo);
	pLibinfo = NULL;

	return true;
}

bool K2HtpMdtorSolo::ReadIniFileContents(const char* filepath, mdtorstrlst_t& linelst, mdtorstrlst_t& allfiles)
{
	if(!filepath){
		ERR_K2HPRN("Parameter is wrong.");
		return false;
	}

	ifstream	cfgstream(filepath, ios::in);
	if(!cfgstream.good()){
		ERR_K2HPRN("Could not open(read only) file(%s)", filepath);
		return false;
	}

	string		line;
	int			lineno;
	for(lineno = 1; cfgstream.good() && getline(cfgstream, line); lineno++){
		// cut after comment word
		string::size_type	pos;
		if(string::npos != (pos = line.find(INICFG_COMMENT_CHAR))){
			line = line.substr(0, pos);
		}

		// trim
		line = trim(line);
		if(0 == line.length()){
			continue;
		}

		// check only include
		if(string::npos != (pos = line.find(INICFG_INCLUDE_STR))){
			string	value	= trim(line.substr(pos + 1));
			string	key		= trim(line.substr(0, pos));
			if(key == INICFG_INCLUDE_STR){
				// found include.
				bool	found_same_file = false;
				for(mdtorstrlst_t::const_iterator iter = allfiles.begin(); iter != allfiles.end(); ++iter){
					if(value == (*iter)){
						found_same_file = true;
						break;
					}
				}
				if(found_same_file){
					WAN_K2HPRN("%s keyword in %s(%d) is filepath(%s) which already read!", INICFG_INCLUDE_STR, filepath, lineno, value.c_str());
				}else{
					// reentrant
					allfiles.push_back(value);
					if(!K2HtpMdtorSolo::ReadIniFileContents(value.c_str(), linelst, allfiles)){
						ERR_K2HPRN("Failed to load include file(%s)", value.c_str());
						cfgstream.close();
						return false;
					}
				}
				continue;
			}
		}
		// add
		linelst.push_back(line);
	}
	cfgstream.close();

	return true;
}

bool K2HtpMdtorSolo::LoadConfigrationIni(const char* filepath, MDTORMODE& mode, mdtorinfolist_t& infolist)
{
	// Load all file contents(with include file)
	mdtorstrlst_t	linelst;
	mdtorstrlst_t	allfiles;
	allfiles.push_back(filepath);
	if(!K2HtpMdtorSolo::ReadIniFileContents(filepath, linelst, allfiles)){
		ERR_K2HPRN("Could not load configuration file(%s) contents.", filepath);
		return false;
	}

	// parsing
	string		line;
	bool		is_main_sec = false;
	bool		is_libs_sec = false;
	PMDTORINFO	pLibinfo	= NULL;
	for(mdtorstrlst_t::const_iterator iter = linelst.begin(); iter != linelst.end(); ++iter){
		line = trim((*iter));
		if(0 == line.length()){
			continue;
		}

		// check section keywords
		if(INICFG_COMMENT_CHAR == line[0]){
			continue;

		}else if(line == INICFG_K2HTPMDTOR_SEC_STR){
			if(!K2HtpMdtorSolo::SetMdtorInfoUtil(pLibinfo, infolist)){		// utility
				return false;
			}
			is_main_sec = true;
			is_libs_sec = false;
			continue;

		}else if(line == INICFG_K2HTPMDTOR_TP_SEC_STR){
			if(!K2HtpMdtorSolo::SetMdtorInfoUtil(pLibinfo, infolist)){		// utility
				return false;
			}
			is_main_sec = false;
			is_libs_sec = true;
			pLibinfo	= new MDTORINFO;
			continue;

		}else if(INICFG_SEC_START_CHAR == line[0] && INICFG_SEC_END_CHAR == line[line.length() - 1]){
			// other section start
			if(!K2HtpMdtorSolo::SetMdtorInfoUtil(pLibinfo, infolist)){		// utility
				return false;
			}
			is_main_sec = false;
			is_libs_sec = false;
			continue;
		}
		if(!is_main_sec && !is_libs_sec){
			continue;
		}

		// in section -> parse key and value
		string				value("");
		string::size_type	pos;
		if(string::npos != (pos = line.find(INICFG_KV_SEP))){
			value	= trim(line.substr(pos + 1));
			line	= upper(trim(line.substr(0, pos)));
		}

		// check and set
		if(0 != value.length()){
			if(is_main_sec){
				// Main section
				if(line == INICFG_K2HTPMDTOR_MODE_STR){
					value = upper(value);
					if(value == INICFG_K2HTPMDTOR_VAL_ALL){
						mode = MDTOR_MODE_ALL;
					}else if(value == INICFG_K2HTPMDTOR_VAL_RANDOM){
						mode = MDTOR_MODE_RANDOM;
					}else if(value == INICFG_K2HTPMDTOR_VAL_UNTIL_SUCC){
						mode = MDTOR_MODE_UNTIL_SUCCESS;
					}else if(value == INICFG_K2HTPMDTOR_VAL_UNTIL_FAIL){
						mode = MDTOR_MODE_UNTIL_FAILURE;
					}else{
						ERR_K2HPRN("Unknown value(%s) for key(%s).", value.c_str(), line.c_str());
						return false;
					}
				}else{
					// unknown key name.
					MSG_K2HPRN("Unknown key(%s), so skip this line.", line.c_str());
				}

			}else{	// is_libs_sec
				// Plugins section
				if(line == INICFG_K2HTPMDTOR_TPPATH_STR){
					pLibinfo->libpath = value;

				}else if(line == INICFG_K2HTPMDTOR_TPCONF_STR){
					pLibinfo->config = value;

				}else{
					// unknown key name.
					MSG_K2HPRN("Unknown key(%s), so skip this line.", line.c_str());
				}
			}
		}else{
			WAN_K2HPRN("The value is empty for %s key, so skip this line.", line.c_str());
		}
	}

	// need to check(last)
	if(!K2HtpMdtorSolo::SetMdtorInfoUtil(pLibinfo, infolist)){		// utility
		return false;
	}
	return true;
}

bool K2HtpMdtorSolo::LoadConfigrationYaml(const char* config, MDTORMODE& mode, mdtorinfolist_t& infolist, bool is_json_string)
{
	// initialize yaml parser
	yaml_parser_t	yparser;
	if(!yaml_parser_initialize(&yparser)){
		ERR_K2HPRN("Failed to initialize yaml parser");
		return false;
	}

	FILE*	fp = NULL;
	if(!is_json_string){
		// open configuration file
		if(NULL == (fp = fopen(config, "r"))){
			ERR_K2HPRN("Could not open configuration file(%s). errno = %d", config, errno);
			return false;
		}
		// set file to parser
		yaml_parser_set_input_file(&yparser, fp);

	}else{	// JSON_STR
		// set string to parser
		yaml_parser_set_input_string(&yparser, reinterpret_cast<const unsigned char*>(config), strlen(config));
	}

	// Do parsing
	bool	result = K2HtpMdtorSolo::LoadConfigrationYamlTopLevel(yparser, mode, infolist);

	yaml_parser_delete(&yparser);
	if(fp){
		fclose(fp);
	}
	return result;
}

bool K2HtpMdtorSolo::LoadConfigrationYamlTopLevel(yaml_parser_t& yparser, MDTORMODE& mode, mdtorinfolist_t& infolist)
{
	CHMYamlDataStack	other_stack;


	bool				is_set_main	= false;
	bool				is_set_libs	= false;
	bool				result		= true;
	for(bool is_loop = true, in_stream = false, in_document = false, in_toplevel = false; is_loop && result; ){
		// get event
		yaml_event_t	yevent;
		if(!yaml_parser_parse(&yparser, &yevent)){
			ERR_K2HPRN("Could not parse event. errno = %d", errno);
			result = false;
			continue;
		}

		// check event
		switch(yevent.type){
			case YAML_NO_EVENT:
				MSG_K2HPRN("There is no yaml event in loop");
				break;

			case YAML_STREAM_START_EVENT:
				if(!other_stack.empty()){
					MSG_K2HPRN("Found start yaml stream event in skipping event loop");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else if(in_stream){
					MSG_K2HPRN("Already start yaml stream event in loop, Thus stacks this event.");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else{
					MSG_K2HPRN("Start yaml stream event in loop");
					in_stream = true;
				}
				break;

			case YAML_STREAM_END_EVENT:
				if(!other_stack.empty()){
					MSG_K2HPRN("Found stop yaml stream event in skipping event loop");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else if(!in_stream){
					MSG_K2HPRN("Already stop yaml stream event in loop, Thus stacks this event.");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else{
					MSG_K2HPRN("Stop yaml stream event in loop");
					in_stream = false;
				}
				break;

			case YAML_DOCUMENT_START_EVENT:
				if(!other_stack.empty()){
					MSG_K2HPRN("Found start yaml document event in skipping event loop");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else if(!in_stream){
					MSG_K2HPRN("Found start yaml document event before yaml stream event in loop, Thus stacks this event.");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else if(in_document){
					MSG_K2HPRN("Already start yaml document event in loop, Thus stacks this event.");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else{
					MSG_K2HPRN("Start yaml document event in loop");
					in_document = true;
				}
				break;

			case YAML_DOCUMENT_END_EVENT:
				if(!other_stack.empty()){
					MSG_K2HPRN("Found stop yaml document event in skipping event loop");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else if(!in_document){
					MSG_K2HPRN("Already stop yaml document event in loop, Thus stacks this event.");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else{
					MSG_K2HPRN("Stop yaml document event in loop");
					in_document = false;
				}
				break;

			case YAML_MAPPING_START_EVENT:
				if(!other_stack.empty()){
					MSG_K2HPRN("Found start yaml mapping event in skipping event loop");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else if(!in_stream){
					MSG_K2HPRN("Found start yaml mapping event before yaml stream event in loop, Thus stacks this event.");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else if(!in_document){
					MSG_K2HPRN("Found start yaml mapping event before yaml document event in loop, Thus stacks this event.");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else if(in_toplevel){
					MSG_K2HPRN("Already start yaml mapping event in loop, Thus stacks this event.");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else{
					MSG_K2HPRN("Start yaml mapping event in loop");
					in_toplevel = true;
				}
				break;

			case YAML_MAPPING_END_EVENT:
				if(!other_stack.empty()){
					MSG_K2HPRN("Found stop yaml mapping event in skipping event loop");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else if(!in_toplevel){
					MSG_K2HPRN("Already stop yaml mapping event in loop, Thus stacks this event.");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else{
					MSG_K2HPRN("Stop yaml mapping event in loop");
					in_toplevel = false;
				}
				break;

			case YAML_SEQUENCE_START_EVENT:
				// always stacking
				//
				if(!other_stack.empty()){
					MSG_K2HPRN("Found start yaml sequence event in skipping event loop");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else{
					MSG_K2HPRN("Found start yaml sequence event before top level event in loop, Thus stacks this event.");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}
				break;

			case YAML_SEQUENCE_END_EVENT:
				// always stacking
				//
				if(!other_stack.empty()){
					MSG_K2HPRN("Found stop yaml sequence event in skipping event loop");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else{
					MSG_K2HPRN("Found stop yaml sequence event before top level event in loop, Thus stacks this event.");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}
				break;

			case YAML_SCALAR_EVENT:
				if(!other_stack.empty()){
					MSG_K2HPRN("Got yaml scalar event in skipping event loop");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else if(!in_stream){
					MSG_K2HPRN("Got yaml scalar event before yaml stream event in loop, Thus stacks this event.");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else if(!in_document){
					MSG_K2HPRN("Got yaml scalar event before yaml document event in loop, Thus stacks this event.");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else if(!in_toplevel){
					MSG_K2HPRN("Got yaml scalar event before yaml mapping event in loop, Thus stacks this event.");
					if(!other_stack.add(yevent.type)){
						result = false;
					}
				}else{
					// Found Top Level Keywords, start to loading
					if(0 == strcasecmp(CFG_K2HTPMDTOR_SEC_STR, reinterpret_cast<const char*>(yevent.data.scalar.value))){
						if(is_set_main){
							MSG_K2HPRN("Got yaml scalar event in loop, but already loading %s. Thus stacks this event.", CFG_K2HTPMDTOR_SEC_STR);
							if(!other_stack.add(yevent.type)){
								result = false;
							}
						}else{
							// Load K2HTPMDTOR section
							if(!K2HtpMdtorSolo::LoadConfigrationYamlMain(yparser, mode)){
								ERR_K2HPRN("Something error occured in loading %s section.", CFG_K2HTPMDTOR_SEC_STR);
								result = false;
							}
						}

					}else if(0 == strcasecmp(CFG_K2HTPMDTOR_TP_SEC_STR, reinterpret_cast<const char*>(yevent.data.scalar.value))){
						if(is_set_libs){
							MSG_K2HPRN("Got yaml scalar event in loop, but already loading %s. Thus stacks this event.", CFG_K2HTPMDTOR_TP_SEC_STR);
							if(!other_stack.add(yevent.type)){
								result = false;
							}
						}else{
							// Load K2HTPMDTOR_TP section
							if(!K2HtpMdtorSolo::LoadConfigrationYamlLibs(yparser, infolist)){
								ERR_K2HPRN("Something error occured in loading %s section.", CFG_K2HTPMDTOR_TP_SEC_STR);
								result = false;
							}
						}

					}else{
						MSG_K2HPRN("Got yaml scalar event in loop, but unknown keyword(%s) for me. Thus stacks this event.", reinterpret_cast<const char*>(yevent.data.scalar.value));
						if(!other_stack.add(yevent.type)){
							result = false;
						}
					}
				}
				break;

			case YAML_ALIAS_EVENT:
				// [TODO]
				// Now we do not supports alias(anchor) event.
				//
				MSG_K2HPRN("Got yaml alias(anchor) event in loop, but we does not support this event. Thus skip this event.");
				break;
		}
		// delete event
		is_loop = yevent.type != YAML_STREAM_END_EVENT;
		yaml_event_delete(&yevent);
	}
	return result;
}

bool K2HtpMdtorSolo::LoadConfigrationYamlMain(yaml_parser_t& yparser, MDTORMODE& mode)
{
	// Must start yaml mapping event.
	yaml_event_t	yevent;
	if(!yaml_parser_parse(&yparser, &yevent)){
		ERR_K2HPRN("Could not parse event. errno = %d", errno);
		return false;
	}
	if(YAML_MAPPING_START_EVENT != yevent.type){
		ERR_K2HPRN("Parsed event type is not start mapping(%d)", yevent.type);
		yaml_event_delete(&yevent);
		return false;
	}
	yaml_event_delete(&yevent);

	// Loading
	string	key("");
	bool	result = true;
	for(bool is_loop = true; is_loop && result; ){
		// get event
		if(!yaml_parser_parse(&yparser, &yevent)){
			ERR_K2HPRN("Could not parse event. errno = %d", errno);
			result = false;
			continue;
		}

		// check event
		if(YAML_MAPPING_END_EVENT == yevent.type){
			// End of mapping event
			is_loop = false;

		}else if(YAML_SCALAR_EVENT == yevent.type){
			// Load key & value
			if(key.empty()){
				key = reinterpret_cast<const char*>(yevent.data.scalar.value);

			}else{
				//
				// Compare key and set value
				//
				if(0 == strcasecmp(INICFG_K2HTPMDTOR_MODE_STR, key.c_str())){
					string value	= upper(string(reinterpret_cast<const char*>(yevent.data.scalar.value)));
					if(value == INICFG_K2HTPMDTOR_VAL_ALL){
						mode = MDTOR_MODE_ALL;
					}else if(value == INICFG_K2HTPMDTOR_VAL_RANDOM){
						mode = MDTOR_MODE_RANDOM;
					}else if(value == INICFG_K2HTPMDTOR_VAL_UNTIL_SUCC){
						mode = MDTOR_MODE_UNTIL_SUCCESS;
					}else if(value == INICFG_K2HTPMDTOR_VAL_UNTIL_FAIL){
						mode = MDTOR_MODE_UNTIL_FAILURE;
					}else{
						ERR_K2HPRN("Unknown value(%s) for %s key.", value.c_str(), key.c_str());
						result = false;
					}
				}else{
					// unknown key name.
					MSG_K2HPRN("Unknown key(%s), so skip this line.", key.c_str());
				}
				key.clear();
			}
		}else{
			// [TODO] Now not support alias(anchor) event
			//
			ERR_K2HPRN("Found unexpected yaml event(%d) in %s section.", yevent.type, CFG_K2HTPMDTOR_SEC_STR);
			result = false;
		}

		// delete event
		if(is_loop){
			is_loop = yevent.type != YAML_STREAM_END_EVENT;
		}
		yaml_event_delete(&yevent);
	}
	return result;
}

bool K2HtpMdtorSolo::LoadConfigrationYamlLibs(yaml_parser_t& yparser, mdtorinfolist_t& infolist)
{
	// Must start yaml sequence(for mapping array) -> mapping event.
	yaml_event_t	yevent;
	{
		// sequence
		if(!yaml_parser_parse(&yparser, &yevent)){
			ERR_K2HPRN("Could not parse event. errno = %d", errno);
			return false;
		}
		if(YAML_SEQUENCE_START_EVENT != yevent.type){
			ERR_K2HPRN("Parsed event type is not start sequence(%d)", yevent.type);
			yaml_event_delete(&yevent);
			return false;
		}
		yaml_event_delete(&yevent);
	}

	// Loading
	string		key("");
	PMDTORINFO	pLibinfo= NULL;		// temporary buffer pointer
	bool		result	= true;
	for(bool is_loop = true; is_loop && result; ){
		// get event
		if(!yaml_parser_parse(&yparser, &yevent)){
			ERR_K2HPRN("Could not parse event. errno = %d", errno);
			result = false;
			continue;				// break loop assap.
		}

		// check event
		if(YAML_MAPPING_START_EVENT == yevent.type){
			// Start mapping event
			if(pLibinfo){
				ERR_K2HPRN("Already start yaml mapping event in %s section loop.", INICFG_K2HTPMDTOR_TP_SEC_STR);
				result = false;
			}else{
				pLibinfo = new MDTORINFO;
			}

		}else if(YAML_MAPPING_END_EVENT == yevent.type){
			// End mapping event
			if(!pLibinfo){
				ERR_K2HPRN("Already stop yaml mapping event in %s section loop.", INICFG_K2HTPMDTOR_TP_SEC_STR);
				result = false;
			}else{
				// Finish one lib configuration
				//
				if(!K2HtpMdtorSolo::SetMdtorInfoUtil(pLibinfo, infolist)){		// utility
					ERR_K2HPRN("Something wrong in %s section.", INICFG_K2HTPMDTOR_TP_SEC_STR);
					result = false;
				}
			}

		}else if(YAML_SEQUENCE_END_EVENT == yevent.type){
			// End sequence(for mapping) event
			if(pLibinfo){
				ERR_K2HPRN("Found yaml sequence event, but not stop yaml mapping event in %s section loop.", INICFG_K2HTPMDTOR_TP_SEC_STR);
				result = false;
			}else{
				// Finish loop without error.
				//
				is_loop = false;
			}

		}else if(YAML_SCALAR_EVENT == yevent.type){
			// Load key & value
			if(key.empty()){
				key = reinterpret_cast<const char*>(yevent.data.scalar.value);

			}else if(pLibinfo){
				//
				// Compare key and set value
				//
				if(0 == strcasecmp(INICFG_K2HTPMDTOR_TPPATH_STR, key.c_str())){
					pLibinfo->libpath = reinterpret_cast<const char*>(yevent.data.scalar.value);
				}else if(0 == strcasecmp(INICFG_K2HTPMDTOR_TPCONF_STR, key.c_str())){
					pLibinfo->config = reinterpret_cast<const char*>(yevent.data.scalar.value);
				}else{
					// unknown key name.
					MSG_K2HPRN("Unknown key(%s), so skip this line.", key.c_str());
				}
				key.clear();
			}else{
				ERR_K2HPRN("Found unexpected key(%s) direct in %s section.", key.c_str(), INICFG_K2HTPMDTOR_TP_SEC_STR);
				result = false;
			}

		}else{
			// [TODO] Now not support alias(anchor) event
			//
			ERR_K2HPRN("Found unexpected yaml event(%d) in %s section.", yevent.type, INICFG_K2HTPMDTOR_TP_SEC_STR);
			result = false;
		}

		// delete event
		if(is_loop){
			is_loop = yevent.type != YAML_STREAM_END_EVENT;
		}
		yaml_event_delete(&yevent);
	}

	if(result){
		if(!K2HtpMdtorSolo::SetMdtorInfoUtil(pLibinfo, infolist)){		// utility
			ERR_K2HPRN("Something wrong in %s section.", INICFG_K2HTPMDTOR_TP_SEC_STR);
			result = false;
		}
	}
	if(pLibinfo){
		K2H_Delete(pLibinfo);
	}

	return result;
}

//---------------------------------------------------------
// K2HtpMdtorSolo : Methods
//---------------------------------------------------------
K2HtpMdtorSolo::K2HtpMdtorSolo() : handle(K2H_INVALID_HANDLE), config(""), mode(MDTOR_MODE_ALL)
{
	if(!K2HtpMdtorSolo::is_init_seed){
		// [NOTE]
		// For random mode, initialize seed simply.
		//
		K2HtpMdtorSolo::is_init_seed= true;
		K2HtpMdtorSolo::seed		= static_cast<unsigned int>(time(NULL));
	}
}

K2HtpMdtorSolo::~K2HtpMdtorSolo()
{
	Unload();
}

bool K2HtpMdtorSolo::Load(k2h_h hk2h, const char* pconfig)
{
	if(K2H_INVALID_HANDLE == hk2h || ISEMPTYSTR(pconfig)){
		ERR_K2HPRN("Parameter is wrong.");
		return false;
	}
	if(IsLoad()){
		ERR_K2HPRN("This object already built from configuration.");
		return false;
	}

	// load configuration
	mdtorinfolist_t	infolist;
	if(!K2HtpMdtorSolo::LoadConfigration(pconfig, mode, infolist)){
		ERR_K2HPRN("Failed to load configuration.");
		return false;
	}

	// check transaction plugin libraries(conflict)
	if(!Check_MdtorInfoList(infolist)){
		ERR_K2HPRN("Failed to load configuration, the reason is about setting transaction plugin libraries.");
		Free_MdtorInfoList(infolist);
		return false;
	}

	// set transaction plugin object list.
	for(mdtorinfolist_t::iterator iter = infolist.begin(); iter != infolist.end(); ++iter){
		PMDTORINFO	pLibInfo = *iter;

		// open transaction plugin
		MdtorTpLib*	pLibObj = new MdtorTpLib();
		if(!pLibObj->Load(pLibInfo->libpath.c_str(), pLibInfo->config.c_str())){
			ERR_K2HPRN("Failed to open transaction plugin(%s).", pLibInfo->libpath.c_str());

			K2H_Delete(pLibObj);
			Free_MdtorInfoList(infolist);
			Unload();						// Unload all
			return false;
		}

		// enable transaction plugin
		if(!pLibObj->Enable(handle)){
			ERR_K2HPRN("Failed to enable transaction plugin(%s).", pLibInfo->libpath.c_str());

			K2H_Delete(pLibObj);
			Free_MdtorInfoList(infolist);
			Unload();						// Unload all
			return false;
		}

		// set it into list
		tplibs.push_back(pLibObj);
		pLibObj = NULL;
	}
	Free_MdtorInfoList(infolist);

	// set others
	handle	= hk2h;
	config	= pconfig;

	return true;
}

bool K2HtpMdtorSolo::Unload(void)
{
	if(!IsLoad()){
		MSG_K2HPRN("This object already unloaded.");
		return true;
	}

	// unload all transaction plugin libs
	for(mdtortplibs_t::iterator iter = tplibs.begin(); iter != tplibs.end(); iter = tplibs.erase(iter)){
		MdtorTpLib*	plib = *iter;
		if(plib){
			// do disable
			if(!plib->Disable(handle)){
				WAN_K2HPRN("Failed to disable transaction plugin, but continue...");
			}
			// do unload
			if(!plib->Unload()){
				WAN_K2HPRN("Failed to unload transaction plugin, but continue...");
			}
			// destruct
			K2H_Delete(plib);
		}
	}
	return true;
}

bool K2HtpMdtorSolo::DoTransaction(PBCOM pBinCom)
{
	if(!IsLoad()){
		ERR_K2HPRN("This object is not initialized.");
		return false;
	}
	if(tplibs.size() < 1){
		MSG_K2HPRN("There is no transaction plugin in list.");
		return true;
	}

	bool	result = false;
	if(MDTOR_MODE_RANDOM == mode){
		// one of plugin as random
		MdtorTpLib*	plib = tplibs[K2HtpMdtorSolo::seed % tplibs.size()];
		if(false == (result = plib->DoTransaction(handle, pBinCom))){
			ERR_K2HPRN("Something error occurred in transaction plugin.");
		}
		K2HtpMdtorSolo::seed = static_cast<unsigned int>(rand_r(&K2HtpMdtorSolo::seed));	// update seed(do not care for conflicting)

	}else{
		// loop
		int	success_count = 0;
		for(mdtortplibs_t::iterator iter = tplibs.begin(); iter != tplibs.end(); iter = tplibs.erase(iter)){
			MdtorTpLib*	plib = *iter;
			if(plib){
				if(false == (result = plib->DoTransaction(handle, pBinCom))){
					//MSG_K2HPRN("Something error occurred in transaction plugin.");
				}else{
					++success_count;
				}

				if(MDTOR_MODE_UNTIL_SUCCESS == mode){
					if(result){
						break;
					}
				}else if(MDTOR_MODE_UNTIL_FAILURE == mode){
					if(!result){
						break;
					}
				}else{	// MDTOR_MODE_ALL
					// nothing to do(not care for error)
				}
			}
		}
		if(0 < success_count){
			result = true;
		}
	}
	return result;
}

/*
 * VIM modelines
 *
 * vim:set ts=4 fenc=utf-8:
 */
