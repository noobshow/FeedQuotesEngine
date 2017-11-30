#if !defined DBQueryString_H
#define DBQueryString_H
#include "../stdafx.h"
#include <string>
#include <stdio.h>
using namespace std;

//--- DB Config File Reading ---

const string INI_DB_CONFIG_FILE_NAME			= "FeedServerDBConfig.ini";
const string INI_DATABASE_CONFIG				= "Database";
const string INI_DB_SERVER_IP_ADDRESS			= "ServerIPAddress";
const string INI_DB_USER_NAME					= "UserName";
const string INI_DB_USER_PASSWORD				= "Password";
const string INI_DATABASE_NAME					= "DBName";



const string QUERY_GET_INPUT_SYMBOL_NAMES				="SELECT pAutoIncrimentID,pSymbolName,Description FROM tbl_symbols";
const string QUERY_GET_INPUT_SYMBOL_STATUS				="SELECT pAutoIncrimentID,fSymbolID,isEnabled,isActiveToSendMT4,isActiveToSendClient FROM tbl_symbol_status";
const string QUERY_GET_INPUT_SYMBOL_SPIKE_MAPPING		="SELECT pAutoIncrimentID,fSymbolID,TickValueChangePercent,TickValueCount,PipsValue FROM tbl_symbol_spike_mapping";
const string QUERY_GET_SYMBOL_OUT_TEXTS					="SELECT pAutoIncrimentID,fSymbolID,SymbolText FROM tbl_symbols_out"; 
const string QUERY_GET_SYMBOL_SOURCE_MAPPING			="SELECT pAutoIncrimentID,fSymbolID,fFeedProviderID FROM tbl_symbol_source_mapping";
const string QUERY_GET_AVAILABLE_FEED_SOURCE			="SELECT tbl_feed_provider.pAutoIncrimentID as baseAutoIncrimentID,tbl_feed_provider.pFeedProviderName,tbl_feed_provider.Description,tbl_feed_source_status.isEnabled,tbl_feed_source_status.isActiveToSendMT4,tbl_feed_source_status.isActiveToSendClient FROM tbl_feed_provider, tbl_feed_source_status where tbl_feed_provider.pAutoIncrimentID=tbl_feed_source_status.fFeedProviderID";

#endif