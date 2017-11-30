#if !defined SymbolModel_H
#define SymbolModel_H

#include "../stdafx.h"
#include <string>
#include <stdio.h>

using namespace std;

class SymbolModel
{
private:
	int			mAutoIncrimentID;
	string		mSymbolName;
	string		mDescription;

public:
	SymbolModel();
	~SymbolModel();

	int getAutoIncrimentID(){return mAutoIncrimentID;}
	void setAutoIncrimentID(int aAutoIncrimentID){mAutoIncrimentID=aAutoIncrimentID;}

	string getSymbolName(){return mSymbolName;}
	void setSymbolName(string aSymbolName){mSymbolName.assign(aSymbolName);}

	string getDescription(){return mDescription;}
	void setDescription(string aDescription){mDescription.assign(aDescription);}

private:

};

#endif