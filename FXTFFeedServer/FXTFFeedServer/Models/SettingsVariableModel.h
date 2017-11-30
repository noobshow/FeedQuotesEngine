#if !defined SettingsVariableModel_H
#define SettingsVariableModel_H

#include "../stdafx.h"
#include <string>
#include <stdio.h>

using namespace std;

class SettingsVariableModel
{
private:
	int			mAutoIncrimentID;
	string		mVariableName;
	string		mValue;

public:
	SettingsVariableModel();
	~SettingsVariableModel();

	int getAutoIncrimentID(){return mAutoIncrimentID;}
	void setAutoIncrimentID(int aAutoIncrimentID){mAutoIncrimentID=aAutoIncrimentID;}

	string getVariableName(){return mVariableName;}
	void setVariableName(string aVariableName){mVariableName.assign(aVariableName);}

	string	getValueInString(){return mValue;}
	int		getValueInInteger(){return atoi(mValue.c_str());}
	float	getValueInFloat(){return atof(mValue.c_str());}
	void	setValue(string aValue){mValue.assign(aValue);} 

private:

};

#endif