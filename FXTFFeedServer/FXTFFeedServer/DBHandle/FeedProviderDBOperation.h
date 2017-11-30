#if !defined FeedProviderDBOperation_H
#define FeedProvicerDBOperation_H

#include "../stdafx.h"
#include <string>
#include <stdio.h>
//--
#include "MySQLDBOperation.h"
#include "DBQueryString.h"
#include "../Models/FeedSourceStatusModel.h"
using namespace std;

class FeedProviderDBOperation
{
private:
	MySQLDBOperation *mMySQLDBOperation;
public:
	FeedProviderDBOperation(MySQLDBOperation *aMySQLDBOperation);
	~FeedProviderDBOperation();

	list<FeedSourceStatusModel*> getAvailableFeedProviders();
	

private:
	
};

#endif