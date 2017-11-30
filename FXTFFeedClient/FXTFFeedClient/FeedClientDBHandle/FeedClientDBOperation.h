#if !defined FeedClientDBOperation_H
#define FeedClientDBOperation_H

#include "../stdafx.h"
#include <string>
#include <stdio.h>
#include <iostream>     
#include <fstream>      
#include <vector>
#include <string>
#include <algorithm>    
#include <iterator>     
#include "boost\tokenizer.hpp"

#include "DBHandle\MySQLDBOperation.h"
#include "Models\SymbolSpikeMappingModel.h"
#include "Models\SymbolModel.h"
#include "Models\UsersModel.h"
#include "Models\SymbolOutModel.h"
#include "Models\SymbolSourceMappingModel.h"
#include "Models\SymbolStatusModel.h"

using namespace std;
using namespace boost;

class FeedClientDBOperation
{
private:
	MySQLDBOperation *mMySQLDBOperation;

public:
	FeedClientDBOperation(MySQLDBOperation *aMySQLDBOperation);
	~FeedClientDBOperation();

	list<SymbolOutModel> getSymbolOut(string aQueryString);
	list<SymbolSourceMappingModel> getSymbolSourceMapping(string aQueryString);
	list<SymbolStatusModel> getSymbolStatus(string aQueryString);
	UsersModel* getLoginCredential(string aQueryString);
	list<SymbolModel> getInputSymbolName(string aQueryString);
	list<SymbolSpikeMappingModel> getInputSymbolSpikeMappingInfo(string aQueryString);
	list<SymbolSpikeMappingModel> ReadCSVFile(string CSVFileData);
	int UpdateTableAsCSVFile(string CSVFileData);
	int doOperationToSymbolSourceMapping(int lQueryType, int lQuerySymbolID, int lQueryFeedProviderID);
	int doOperationToAllSymbolSourceMapping(int lTotalSymbolID,int lQueryFeedProviderID);

};

#endif