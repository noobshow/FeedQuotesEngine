#if !defined FeedClientConstant_H
#define FeedClientConstant_H

#include <string>
#include <stdio.h>

using namespace std;

#define MAX_LOADSTRING 100

#define FEED_MONITOR_SYMBOL_INDEX					0
#define FEED_MONITOR_SYMBOL							0
#define FEED_MONITOR_OCBC_BID						1
#define FEED_MONITOR_OCBC_ASK						2
#define FEED_MONITOR_OCBC_DATE						3
#define FEED_MONITOR_PQE_BID						4
#define FEED_MONITOR_PQE_ASK						5
#define FEED_MONITOR_PQE_DATE						6
#define FEED_MONITOR_FXCMUS_BID						7
#define FEED_MONITOR_FXCMUS_ASK						8
#define FEED_MONITOR_FXCMUS_DATE					9

#define FEED_SWITCHER_INDEX							0
#define FEED_SWITCHER_SYMBOL						1
#define FEED_SWITCHER_OCBC							2
#define FEED_SWITCHER_FXCM							3
#define FEED_SWITCHER_FXCMUS						4

#define SYMBOLSPIKEMAPPING_PAUTOINCRIMENTID			0
#define SYMBOLSPIKEMAPPING_FSYMBOLID				0
#define SYMBOLSPIKEMAPPING_TICKVALUECHANGEPERCENT	1
#define SYMBOLSPIKEMAPPING_TICKVALUECOUNT			2
#define SYMBOLSPIKEMAPPING_PIPSVALUE				3


#define MEDIUM_ARRAY_LEN	64 // Min length for small to medium array lengths
//Feed Monitor Column specification
typedef struct
{
	TCHAR Symbol[MEDIUM_ARRAY_LEN];
	TCHAR OCBC_bid[MEDIUM_ARRAY_LEN];
	TCHAR OCBC_ask[MEDIUM_ARRAY_LEN];
	TCHAR OCBC_update[MEDIUM_ARRAY_LEN];
	TCHAR PQE_bid[MEDIUM_ARRAY_LEN];
	TCHAR PQE_ask[MEDIUM_ARRAY_LEN];
	TCHAR PQE_update[MEDIUM_ARRAY_LEN];

} FEED_MONITOR_STRUCT;

const string INI_CONFIG_FILE_NAME		= "FeedClientConfig.ini";
const string INI_DATABASE_CONFIG_NAME	= "Database";
const string INI_DB_SERVER_IP_ADDRESS	= "DBServerIPAddress";
const string INI_DB_USER_NAME			= "DBUserName";
const string INI_DB_USER_PASSWORD		= "DBPassword";
const string INI_DB_SCHEMA_NAME			= "DBSchemaName";

const string INI_FEEDSERVER_CONFIG_NAME = "FeedServer";
const string INI_FEED_SERVER_IP_ADDRESS = "ServerIPAddress";

#endif