#if !defined SymbolOutModel_H
#define SymbolOutModel_H

#include "../stdafx.h"
#include <string>
#include <stdio.h>

using namespace std;

class SymbolOutModel
{
private:
	int			mAutoIncrimentID;
	int			mSymbolID;
	string		mSymbolOutText;
	int			mSymbolDigit;

public:
	SymbolOutModel();
	~SymbolOutModel();

	int getAutoIncrimentID(){return mAutoIncrimentID;}
	void setAutoIncrimentID(int aAutoIncrimentID){mAutoIncrimentID=aAutoIncrimentID;}

	int getSymbolID(){return mSymbolID;}
	void setSymbolID(int aSymbolID){mSymbolID=aSymbolID;}

	string getSymbolOutText(){return mSymbolOutText;}
	void setSymbolOutText(string aSymbolOutText){mSymbolOutText.assign(aSymbolOutText);}

	int getSymbolDigit(){return mSymbolDigit;}
	void setSymbolDigit(int aSymbolDigit){mSymbolDigit=aSymbolDigit;}

private:

};

#endif