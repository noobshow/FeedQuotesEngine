#if !defined UsersModel_H
#define UsersModel_H

#include "../stdafx.h"
#include <string>
#include <stdio.h>

using namespace std;

class UsersModel
{
private:
	int			mAutoIncrimentID;
	string		mDisplayName;
	string		mLoginID;
	string		mPassword;

public:
	UsersModel();
	~UsersModel();

	int getAutoIncrimentID(){return mAutoIncrimentID;}
	void setAutoIncrimentID(int aAutoIncrimentID){mAutoIncrimentID=aAutoIncrimentID;}

	string getUsersName(){return mLoginID;}
	void setUsersName(string aLoginID){mLoginID.assign(aLoginID);}

	string getPassword(){return mPassword;}
	void setPassword(string aPassword){mPassword.assign(aPassword);}

private:

};

#endif