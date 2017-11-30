#include "SymbolDBOperation.h"


SymbolDBOperation::SymbolDBOperation(MySQLDBOperation *aMySQLDBOperation)
{
	mMySQLDBOperation=aMySQLDBOperation;
}

SymbolDBOperation::~SymbolDBOperation()
{
}

list<SymbolValues*> SymbolDBOperation::getInputSymbolNames()
{
	cout<<" QUERY : >> "<<QUERY_GET_INPUT_SYMBOL_NAMES<<endl;

	list<SymbolValues*> lListofSymbolValues;

	if (mMySQLDBOperation)
	{
		sql::ResultSet *lQueryReturnData= mMySQLDBOperation->executeSelectQuery(QUERY_GET_INPUT_SYMBOL_NAMES);
		if (lQueryReturnData)
		{
			while (lQueryReturnData->next())
			{
				SymbolValues* lSymbolValues=new SymbolValues();
				lSymbolValues->setAutoIncrimentID(lQueryReturnData->getInt("pAutoIncrimentID"));
				lSymbolValues->setSymbolName(string(lQueryReturnData->getString("pSymbolName").c_str()));
				lSymbolValues->setDescription(string(lQueryReturnData->getString("Description").c_str()));

				lListofSymbolValues.push_back(lSymbolValues);
			}
		}
	}
	return lListofSymbolValues;
}

bool SymbolDBOperation::getInputSymbolStatus(list<SymbolValues*> aAvailableSymbols)
{
	cout<<" QUERY : >> "<<QUERY_GET_INPUT_SYMBOL_STATUS<<endl;

	if (mMySQLDBOperation)
	{
		sql::ResultSet *lQueryReturnData= mMySQLDBOperation->executeSelectQuery(QUERY_GET_INPUT_SYMBOL_STATUS);
		if (lQueryReturnData)
		{
			while (lQueryReturnData->next())
			{
				SymbolStatusModel* lSymbolStatusModel=new SymbolStatusModel();
				lSymbolStatusModel->setAutoIncrimentID(lQueryReturnData->getInt("pAutoIncrimentID"));
				lSymbolStatusModel->setSymbolID(lQueryReturnData->getInt("fSymbolID"));			
				lSymbolStatusModel->setEnable(lQueryReturnData->getInt("isEnabled"));
				lSymbolStatusModel->setActiveToSendMT4(lQueryReturnData->getInt("isActiveToSendMT4"));
				lSymbolStatusModel->setActiveToSendClient(lQueryReturnData->getInt("isActiveToSendClient"));
			
				updateAvailableSymbolListForSymbolStatus(aAvailableSymbols,lSymbolStatusModel);
			}
		}
	}
	return true;
}

void SymbolDBOperation::updateAvailableSymbolListForSymbolStatus(list<SymbolValues*> aAvailableSymbols,SymbolStatusModel* aSymbolStatusModel)
{
	std::list<SymbolValues*>::iterator list_iter;
	for( list_iter= aAvailableSymbols.begin(); list_iter != aAvailableSymbols.end(); ++list_iter)
	{
		SymbolValues *lSymbolValues=(SymbolValues*)*list_iter;
		if (lSymbolValues->getAutoIncrimentID()==aSymbolStatusModel->getSymbolID())
		{
			lSymbolValues->setSymbolStatusModel(aSymbolStatusModel);
			*list_iter=lSymbolValues;
			break;
		}
	}
}

bool SymbolDBOperation::getInputSymbolSpikeMappingInfo(list<SymbolValues*> aAvailableSymbols)
{
	cout<<" QUERY : >> "<<QUERY_GET_INPUT_SYMBOL_SPIKE_MAPPING<<endl;

	if (mMySQLDBOperation)
	{
		sql::ResultSet *lQueryReturnData= mMySQLDBOperation->executeSelectQuery(QUERY_GET_INPUT_SYMBOL_SPIKE_MAPPING);
		if (lQueryReturnData)
		{
			while (lQueryReturnData->next())
			{
				SymbolSpikeMappingModel* lSymbolSpikeMappingModel=new SymbolSpikeMappingModel();
				lSymbolSpikeMappingModel->setAutoIncrimentID(lQueryReturnData->getInt("pAutoIncrimentID"));
				lSymbolSpikeMappingModel->setSymbolID(lQueryReturnData->getInt("fSymbolID"));			
				lSymbolSpikeMappingModel->setTickValueChangePercent(lQueryReturnData->getDouble("TickValueChangePercent"));
				lSymbolSpikeMappingModel->setTickValueCount(lQueryReturnData->getInt("TickValueCount"));
				lSymbolSpikeMappingModel->setPipsValue(lQueryReturnData->getInt("PipsValue"));
			
				updateAvailableSymbolListForSymbolSpikeMapping(aAvailableSymbols,lSymbolSpikeMappingModel);
			}
			return true;
		}else
		{
			return false;
		}
	}
}

void SymbolDBOperation::updateAvailableSymbolListForSymbolSpikeMapping(list<SymbolValues*> aAvailableSymbols,SymbolSpikeMappingModel* aSymbolSpikeMappingModel)
{
	std::list<SymbolValues*>::iterator list_iter;
	for( list_iter= aAvailableSymbols.begin(); list_iter != aAvailableSymbols.end(); ++list_iter)
	{
		SymbolValues *lSymbolValues=(SymbolValues*)*list_iter;
		if (lSymbolValues->getAutoIncrimentID()==aSymbolSpikeMappingModel->getSymbolID())
		{
			lSymbolValues->setSymbolSpikeMapping(aSymbolSpikeMappingModel);
			*list_iter=lSymbolValues;
			break;
		}
	}
}

bool SymbolDBOperation::getSymbolOutTexts(list<SymbolValues*> aAvailableSymbols)
{
	cout<<" QUERY : >> "<<QUERY_GET_SYMBOL_OUT_TEXTS<<endl;

	if (mMySQLDBOperation)
	{
		sql::ResultSet *lQueryReturnData= mMySQLDBOperation->executeSelectQuery(QUERY_GET_SYMBOL_OUT_TEXTS);
		if( lQueryReturnData )
		{
			while (lQueryReturnData->next())
			{
				SymbolOutModel* lSymbolOutModel=new SymbolOutModel();
				lSymbolOutModel->setAutoIncrimentID(lQueryReturnData->getInt("pAutoIncrimentID"));
				lSymbolOutModel->setSymbolID(lQueryReturnData->getInt("fSymbolID"));			
				lSymbolOutModel->setSymbolOutText(string(lQueryReturnData->getString("SymbolText").c_str()));

			
				updateAvailableSymbolListForSymbolOutText(aAvailableSymbols,lSymbolOutModel);
			}
		}
	}
	return true;
}

void SymbolDBOperation::updateAvailableSymbolListForSymbolOutText(list<SymbolValues*> aAvailableSymbols,SymbolOutModel* aSymbolOutModel)
{
	std::list<SymbolValues*>::iterator list_iter;
	for( list_iter= aAvailableSymbols.begin(); list_iter != aAvailableSymbols.end(); ++list_iter)
	{
		SymbolValues *lSymbolValues=(SymbolValues*)*list_iter;
		if (lSymbolValues->getAutoIncrimentID()==aSymbolOutModel->getSymbolID())
		{
			lSymbolValues->insertItemToSymbolOutTexts(aSymbolOutModel);
			*list_iter=lSymbolValues;
			break;
		}
	}
}

bool SymbolDBOperation::getSymbolSourceMappingModel(list<SymbolValues*> aAvailableSymbols)
{
	cout<<" QUERY : >> "<<QUERY_GET_SYMBOL_SOURCE_MAPPING<<endl;

	if (mMySQLDBOperation)
	{
		sql::ResultSet *lQueryReturnData= mMySQLDBOperation->executeSelectQuery(QUERY_GET_SYMBOL_SOURCE_MAPPING);
		
		while (lQueryReturnData->next())
		{
			SymbolSourceMappingModel* lSymbolSourceMappingModel=new SymbolSourceMappingModel();
			lSymbolSourceMappingModel->setAutoIncrimentID(lQueryReturnData->getInt("pAutoIncrimentID"));
			lSymbolSourceMappingModel->setSymbolID(lQueryReturnData->getInt("fSymbolID"));			
			lSymbolSourceMappingModel->setFeedProviderID(lQueryReturnData->getInt("fFeedProviderID"));

			
			updateAvailableSymbolListForSymbolSourceMappingModel(aAvailableSymbols,lSymbolSourceMappingModel);
		}
	}
	return true;
}

void SymbolDBOperation::updateAvailableSymbolListForSymbolSourceMappingModel(list<SymbolValues*> aAvailableSymbols,SymbolSourceMappingModel* aSymbolSourceMappingModel)
{
	std::list<SymbolValues*>::iterator list_iter;
	for( list_iter= aAvailableSymbols.begin(); list_iter != aAvailableSymbols.end(); ++list_iter)
	{
		SymbolValues *lSymbolValues=(SymbolValues*)*list_iter;
		if (lSymbolValues->getAutoIncrimentID()==aSymbolSourceMappingModel->getSymbolID())
		{
			lSymbolValues->insertItemToSymbolSourceModels(aSymbolSourceMappingModel);
			*list_iter=lSymbolValues;
			break;
		}
	}
}