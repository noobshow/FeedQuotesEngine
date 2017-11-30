#include "FeedClientDBOperation.h"

FeedClientDBOperation::FeedClientDBOperation(MySQLDBOperation *aMySQLDBOperation)
{
	mMySQLDBOperation=aMySQLDBOperation;
}

FeedClientDBOperation::~FeedClientDBOperation()
{
}

UsersModel* FeedClientDBOperation::getLoginCredential(string aQueryString)
{
	UsersModel *lUsersModel=NULL;
	if (mMySQLDBOperation)
	{
		sql::ResultSet *lQueryReturnData= mMySQLDBOperation->executeSelectQuery(aQueryString);

		while (lQueryReturnData->next())
		{
			lUsersModel= new UsersModel();
			lUsersModel->setAutoIncrimentID(lQueryReturnData->getInt("pAutoIncrimentID"));
			lUsersModel->setUsersName(string(lQueryReturnData->getString("pLoginID").c_str()));
			lUsersModel->setPassword(string(lQueryReturnData->getString("Password").c_str()));
			//lListOfData.push_back(lUsersModel);
		}
	}
	return lUsersModel;
}

list<SymbolOutModel> FeedClientDBOperation::getSymbolOut(string aQueryString)
{
	list<SymbolOutModel> lListOfData;
	if (mMySQLDBOperation)
	{
		sql::ResultSet *lQueryReturnData= mMySQLDBOperation->executeSelectQuery(aQueryString);

		while (lQueryReturnData->next())
		{
			SymbolOutModel lSymbolOutModel = SymbolOutModel();
			lSymbolOutModel.setAutoIncrimentID(lQueryReturnData->getInt("pAutoIncrimentID"));
			lSymbolOutModel.setSymbolID(lQueryReturnData->getInt("fSymbolID"));
			lSymbolOutModel.setSymbolOutText(string(lQueryReturnData->getString("SymbolText").c_str()));
			lSymbolOutModel.setSymbolDigit(lQueryReturnData->getInt("fSymbolDigit"));
			
			lListOfData.push_back(lSymbolOutModel);
		}
	}
	return lListOfData;
}

list<SymbolModel> FeedClientDBOperation::getInputSymbolName(string aQueryString)
{
	list<SymbolModel> lListofSymbolValues;

	if (mMySQLDBOperation)
	{
		sql::ResultSet *lQueryReturnData= mMySQLDBOperation->executeSelectQuery(aQueryString);
		while (lQueryReturnData->next())
		{
			SymbolModel lSymbolModel=SymbolModel();
			lSymbolModel.setAutoIncrimentID(lQueryReturnData->getInt("pAutoIncrimentID"));
			lSymbolModel.setSymbolName(string(lQueryReturnData->getString("pSymbolName").c_str()));
			lSymbolModel.setDescription(string(lQueryReturnData->getString("Description").c_str()));

			lListofSymbolValues.push_back(lSymbolModel);
		}
	}
	return lListofSymbolValues;
}

list<SymbolSourceMappingModel> FeedClientDBOperation::getSymbolSourceMapping(string aQueryString)
{
	list<SymbolSourceMappingModel> lListOfSymbolSourceMapping;
	if (mMySQLDBOperation)
	{
		sql::ResultSet *lQueryReturnData= mMySQLDBOperation->executeSelectQuery(aQueryString);

		while (lQueryReturnData->next())
		{
			SymbolSourceMappingModel lSymbolSourceMappingModel = SymbolSourceMappingModel();
			lSymbolSourceMappingModel.setAutoIncrimentID(lQueryReturnData->getInt("pAutoIncrimentID"));
			lSymbolSourceMappingModel.setSymbolID(lQueryReturnData->getInt("fSymbolID"));
			lSymbolSourceMappingModel.setFeedProviderID(lQueryReturnData->getInt("fFeedProviderID"));
			
			lListOfSymbolSourceMapping.push_back(lSymbolSourceMappingModel);
		}
	}
	return lListOfSymbolSourceMapping;
}

list<SymbolStatusModel> FeedClientDBOperation::getSymbolStatus(string aQueryString)
{
	list<SymbolStatusModel> lListSymbolStatus;

	return lListSymbolStatus;
}

list<SymbolSpikeMappingModel> FeedClientDBOperation::getInputSymbolSpikeMappingInfo(string aQueryString)
{
	list<SymbolSpikeMappingModel> lListOfData;
	if (mMySQLDBOperation)
	{
		sql::ResultSet *lQueryReturnData= mMySQLDBOperation->executeSelectQuery(aQueryString);

		while (lQueryReturnData->next())
		{
			SymbolSpikeMappingModel lSymbolSpikeMappingModel=SymbolSpikeMappingModel();
			lSymbolSpikeMappingModel.setAutoIncrimentID(lQueryReturnData->getInt("pAutoIncrimentID"));
			lSymbolSpikeMappingModel.setSymbolID(lQueryReturnData->getInt("fSymbolID"));			
			lSymbolSpikeMappingModel.setTickValueChangePercent(lQueryReturnData->getDouble("TickValueChangePercent"));
			lSymbolSpikeMappingModel.setTickValueCount(lQueryReturnData->getInt("TickValueCount"));
			lSymbolSpikeMappingModel.setPipsValue(lQueryReturnData->getInt("PipsValue"));
			//lListOfData.push_front(lSymbolSpikeMappingModel);
			lListOfData.push_back(lSymbolSpikeMappingModel);
		}
	}
	return lListOfData;
}

list<SymbolSpikeMappingModel> FeedClientDBOperation::ReadCSVFile(string CSVFileData)
{
	list<SymbolSpikeMappingModel> lListOfDataCSVFile;
    ifstream in(CSVFileData.c_str());
    if (!in.is_open()) 
		cout<<"Couldn't open the CSV file";

    typedef tokenizer< escaped_list_separator<char> > Tokenizer;
    vector< string > lRowWiseFullString;
    string lRowTokenize;

    while (getline(in,lRowTokenize))
    {
		SymbolSpikeMappingModel lSymbolSpikeMappingModel=SymbolSpikeMappingModel();
        Tokenizer tok(lRowTokenize);
        lRowWiseFullString.assign(tok.begin(),tok.end());
		//if(lRowWiseFullString.size() != 5)
		//	MessageBox(NULL,"Error: CSV file formatting does not following.",NULL,MB_OK|MB_ICONINFORMATION);

		lSymbolSpikeMappingModel.setAutoIncrimentID(atoi(lRowWiseFullString[0].c_str()));
		lSymbolSpikeMappingModel.setSymbolID(atoi(lRowWiseFullString[1].c_str()));
		lSymbolSpikeMappingModel.setTickValueChangePercent(atof(lRowWiseFullString[2].c_str()));
		lSymbolSpikeMappingModel.setTickValueCount(atoi(lRowWiseFullString[3].c_str()));
		lSymbolSpikeMappingModel.setPipsValue(atoi(lRowWiseFullString[4].c_str()));

		lListOfDataCSVFile.push_front(lSymbolSpikeMappingModel);
    }
	return lListOfDataCSVFile;
}

int FeedClientDBOperation::UpdateTableAsCSVFile(string CSVFileData)
{
	int lDBUpdateStatus;
	list<SymbolSpikeMappingModel> lListOfDataCSVFile = FeedClientDBOperation::ReadCSVFile(CSVFileData);
	for(std::list<SymbolSpikeMappingModel>::iterator list_iter = lListOfDataCSVFile.begin(); list_iter != lListOfDataCSVFile.end(); list_iter++)
	{
		SymbolSpikeMappingModel lSymbolSpikeMappingModel=(SymbolSpikeMappingModel)*list_iter;
		string pAutoIncrimentID( "pAutoIncrimentID="+ to_string(lSymbolSpikeMappingModel.getAutoIncrimentID()) );
		string fSymbolID( ", fSymbolID="+ to_string(lSymbolSpikeMappingModel.getSymbolID()) );
		string TickValueChangePercent( ", TickValueChangePercent="+ to_string(lSymbolSpikeMappingModel.getTickValueChangePercent()));
		string TickValueCount( ", TickValueCount="+ to_string(lSymbolSpikeMappingModel.getTickValueCount()) );
		string PipsValue( ", PipsValue="+ to_string(lSymbolSpikeMappingModel.getPipsValue()) );
		string lWhereSymbolID("fSymbolID="+ to_string(lSymbolSpikeMappingModel.getSymbolID()));
		string lQueryStringUpdate="UPDATE tbl_symbol_spike_mapping SET "+pAutoIncrimentID+fSymbolID+TickValueChangePercent+TickValueCount+PipsValue+" WHERE "+lWhereSymbolID;
		
		lDBUpdateStatus = mMySQLDBOperation->executeUpdateQuery(lQueryStringUpdate);
	}

	if( mMySQLDBOperation )
	{

	}

	return lDBUpdateStatus;
}

int FeedClientDBOperation::doOperationToSymbolSourceMapping(int lQueryType,int lQuerySymbolID,int lQueryFeedProviderID)
{
	string lStringQuery = "";
	if( lQueryType == 1 )  //inserQuery
	{
		lStringQuery = "insert into tbl_symbol_source_mapping (fSymbolID,fFeedProviderID) values (" + to_string(lQuerySymbolID) + "," + to_string(lQueryFeedProviderID) + ")";
		return mMySQLDBOperation->executeUpdateQuery(lStringQuery);
	}
	if( lQueryType == 0 )  //deleteQuery
	{
		lStringQuery = "delete from tbl_symbol_source_mapping where fSymbolID=" + to_string(lQuerySymbolID) + " && fFeedProviderID=" + to_string(lQueryFeedProviderID);
		return mMySQLDBOperation->executeUpdateQuery(lStringQuery);
	}
}

int FeedClientDBOperation::doOperationToAllSymbolSourceMapping(int lTotalSymbolID,int lQueryFeedProviderID)
{
	string lStringQuery = "";
	int fFeedProviderID = lQueryFeedProviderID;
	int lStatus;
	for(int fSymbolID = 1; fSymbolID<=lTotalSymbolID; fSymbolID++)
	{
		lStringQuery = "INSERT INTO tbl_symbol_source_mapping (fSymbolID, fFeedProviderID) SELECT '"+to_string(fSymbolID)+"','"+to_string(fFeedProviderID)+"' from tbl_symbol_source_mapping WHERE NOT EXISTS ( SELECT fSymbolID,fFeedProviderID FROM tbl_symbol_source_mapping WHERE fSymbolID= '"+to_string(fSymbolID)+"' AND fFeedProviderID= '"+to_string(fFeedProviderID)+"') LIMIT 1";
		lStatus = mMySQLDBOperation->executeUpdateQuery(lStringQuery);
	}
return lStatus;
}