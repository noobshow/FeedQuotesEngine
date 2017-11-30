#include "FeedProviderDBOperation.h"


FeedProviderDBOperation::FeedProviderDBOperation(MySQLDBOperation *aMySQLDBOperation)
{
	mMySQLDBOperation=aMySQLDBOperation;
}

FeedProviderDBOperation::~FeedProviderDBOperation()
{
}

list<FeedSourceStatusModel*> FeedProviderDBOperation::getAvailableFeedProviders()
{
	cout<<" QUERY : >> "<<QUERY_GET_AVAILABLE_FEED_SOURCE<<endl;

	list<FeedSourceStatusModel*> lListofAvailableFeedProviders;

	if (mMySQLDBOperation)
	{
		sql::ResultSet *lQueryReturnData= mMySQLDBOperation->executeSelectQuery(QUERY_GET_AVAILABLE_FEED_SOURCE);
		if (lQueryReturnData)
		{
			while (lQueryReturnData->next())
			{
				FeedSourceStatusModel* lFeedSourceStatusModel=new FeedSourceStatusModel();
				lFeedSourceStatusModel->setAutoIncrimentID(lQueryReturnData->getInt("baseAutoIncrimentID"));
				lFeedSourceStatusModel->setFeedProviderID(lQueryReturnData->getInt("baseAutoIncrimentID"));
				lFeedSourceStatusModel->setFeedProviderName(string(lQueryReturnData->getString("pFeedProviderName").c_str()));
				lFeedSourceStatusModel->setDescription(string(lQueryReturnData->getString("Description").c_str()));
				lFeedSourceStatusModel->setEnable(lQueryReturnData->getInt("isEnabled"));
				lFeedSourceStatusModel->setActiveToSendMT4(lQueryReturnData->getInt("isActiveToSendMT4"));
				lFeedSourceStatusModel->setActiveToSendClient(lQueryReturnData->getInt("isActiveToSendClient"));
			
				lListofAvailableFeedProviders.push_back(lFeedSourceStatusModel);
			}
		}
	}
	return lListofAvailableFeedProviders;
}