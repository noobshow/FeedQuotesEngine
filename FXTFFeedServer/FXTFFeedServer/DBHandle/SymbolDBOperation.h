#if !defined SymbolDBOperation_H
#define SymbolDBOperation_H

#include "../stdafx.h"
#include <string>
#include <stdio.h>
//--
#include "MySQLDBOperation.h"
#include "DBQueryString.h"
#include "../Settings/SymbolValues.h"
using namespace std;

class SymbolValues;

class SymbolDBOperation
{
private:
	MySQLDBOperation *mMySQLDBOperation;
public:
	SymbolDBOperation(MySQLDBOperation *aMySQLDBOperation);
	~SymbolDBOperation();

	list<SymbolValues*> getInputSymbolNames();
	bool getInputSymbolStatus(list<SymbolValues*> aAvailableSymbols);
	bool getInputSymbolSpikeMappingInfo(list<SymbolValues*> aAvailableSymbols);
	bool getSymbolOutTexts(list<SymbolValues*> aAvailableSymbols);
	bool getSymbolSourceMappingModel(list<SymbolValues*> aAvailableSymbols);

private:
	void updateAvailableSymbolListForSymbolStatus(list<SymbolValues*> aAvailableSymbols,SymbolStatusModel* aSymbolStatusModel);
	void updateAvailableSymbolListForSymbolSpikeMapping(list<SymbolValues*> aAvailableSymbols,SymbolSpikeMappingModel* aSymbolSpikeMappingModel);
	void updateAvailableSymbolListForSymbolOutText(list<SymbolValues*> aAvailableSymbols,SymbolOutModel* aSymbolOutModel);
	void updateAvailableSymbolListForSymbolSourceMappingModel(list<SymbolValues*> aAvailableSymbols,SymbolSourceMappingModel* aSymbolOutModel);

};



#endif