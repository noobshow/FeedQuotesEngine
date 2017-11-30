#ifndef ClientRequestModel_H
#define ClientRequestModel_H

#include "../stdafx.h"
#include <string>
#include <stdio.h>
#include <list>
#include <vector>

using namespace std;

struct RequestStruct
{
	string ActionName;
	string Value;
};

class ClientRequestModel
{
public:
	ClientRequestModel();
	~ClientRequestModel();
	bool setInputMessage(string aInputMessage);

private:
	int						mRequestRandID;
	int						mUserID;
	string					mCommandType;
	list<RequestStruct>		mClientActionRequests;
private:
	vector<string> splitStringInputMessage(string aInputMessage);
	vector<string> splitStringActionMessage(string aInputMessage);
	vector<string> split(const string &s, char delim);

public:
	int getRequestRandID(){return mRequestRandID;}
	void setRequestRandID(int aRequestRandID){mRequestRandID=aRequestRandID;}

	int getUserID(){return mUserID;}
	void setUserID(int aUserID){mUserID=aUserID;}

	string getCommandType(){return mCommandType;}
	void setCommandType(string aCommandType){mCommandType.assign(aCommandType);}

	/*

	list<RequestStruct> getClientActionRequests(){return mClientActionRequests;}
	void addClientActionRequest(RequestStruct aRequestStruct)
	{
		mClientActionRequests.push_back(aRequestStruct);
	}
	void removeClientActionRequest(RequestStruct aRequestStruct)
	{
		mClientActionRequests.remove(aRequestStruct);
	}

	void clearClientActionRequest()
	{
		mClientActionRequests.clear();
	}
	*/
};

#endif
