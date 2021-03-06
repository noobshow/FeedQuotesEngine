//-*-C++-*-///////////////////////////////////////////////////////////////////
//
//   Class: myLog
//   
//   Description: used as a system log file for the simDelta project 
//
//   Name of Designer:  Liyang Yu 
//   Name of Coder:     Liyang Yu 
//   Date:              01/27/04
//
//////////////////////////////////////////////////////////////////////////////

#ifndef SocketLog_H
#define SocketLog_H 1

#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

/* 
   the log file has to be accessed from any code
   which includes this header (similiar to cout, cerr, clog, etc..)
*/

class SocketLog;
extern SocketLog winLog;

// const string SD_DEFAULT_LOGFILE = "simDelta.log";

const int LOG_WRITE  = ios::out;
const int LOG_APPEND = ios::app;

const int EXIT_MSG_SIZE = 512;
const int MAX_EXIT_CODES = 3;

class SocketLog : public ofstream
{

public:

   enum logLevels 
   {
      LEVEL_0,       // buffer all log messages
      LEVEL_1,       // buffer Level one, two and three log messages
      LEVEL_2,       // buffer Level two and three log messages
      LEVEL_3,       // buffer Level three log messages
      QUIET_MODE     // do not print out any messages
   };
 
   SocketLog();
   SocketLog(const string&);
   SocketLog(const string&,enum logLevels);
   virtual ~SocketLog();

private:

   void initVars();
   void init(const string&);
   void init(const string&,int);
   void init();

   char* getExecTime();
   void  getExecTime(int*,int*);
   void  openLog(const string&,int);
   void  printHeader(int);

private:

   string logName;
   enum logLevels logLevel;
   time_t startTime;

};

const enum SocketLog::logLevels L0 = SocketLog::LEVEL_0;
const enum SocketLog::logLevels L1 = SocketLog::LEVEL_1;
const enum SocketLog::logLevels L2 = SocketLog::LEVEL_2;
const enum SocketLog::logLevels L3 = SocketLog::LEVEL_3;
const enum SocketLog::logLevels LQUIET = SocketLog::QUIET_MODE;

#endif

