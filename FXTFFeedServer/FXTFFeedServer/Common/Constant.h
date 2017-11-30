#if !defined Constant_H
#define Constant_H
#include <string>
#include <stdio.h>

using namespace std;

enum hostType {NAME, ADDRESS};
const int HOST_NAME_LENGTH = 64;

const int MAX_NUM_CLIENTS = 10;
const int MAX_NUM_MT4_SERVER = 10;


const int MAX_RECV_LEN = 8096;
const int MAX_MSG_LEN = 1024;
const int PORTNUM_CLIENT = 4010;
const int PORTNUM_MT4_SERVER = 4005;
const int MSG_HEADER_LEN = 6;

const string CLIENT_COMMAND_TYPE_SPIKE="SPIKE_OPERATION";
const string CLIENT_ACTION_TYPE_SPIKE_NFP="NFP";
const string CLIENT_ACTION_TYPE_SPIKE_HIGH="HIGH";
const string CLIENT_ACTION_TYPE_SPIKE_NORMAL="NORMAL";
const string CLIENT_COMMAND_TYPE_FEED_SWITCHER="FEED_SWITCHER";

#endif