#include "ClientRequestModel.h"
#include <sstream>
#include <boost\algorithm\string.hpp>

ClientRequestModel::ClientRequestModel()
{
	

}

ClientRequestModel::~ClientRequestModel()
{
}

bool ClientRequestModel::setInputMessage(string aInputMessage)
{

	// Message Format [RandomNumber] [UserID] [CommandType] [[Action]=[Value]] [[Action]=[Value]]

	vector<string> lSplittedVector=splitStringInputMessage(aInputMessage);
	
	if (lSplittedVector.size()>3)
	{
		if(!lSplittedVector[1].empty())
		{
			mRequestRandID=atoi(lSplittedVector[1].c_str());
		}else
		{
			return false;
		}

		if (!lSplittedVector[2].empty())
		{
			mUserID=atoi(lSplittedVector[2].c_str());
		}else
		{
			return false;
		}
		
		if (!lSplittedVector[3].empty())
		{
			mCommandType.assign(lSplittedVector[3]);
		}else
		{
			return false;
		}

		if (lSplittedVector[4].empty())
		{
			return false;
		}
		
		int lLength=lSplittedVector.size();
		for (int i = 4; i <lLength ; i++)
		{
			vector<string> lSplittedActionMessageVector=splitStringActionMessage(lSplittedVector[i]);
			if (lSplittedActionMessageVector.size()>1)
			{
				RequestStruct lRequestAction;
				lRequestAction.ActionName.assign(lSplittedActionMessageVector[0]);
				lRequestAction.Value.assign(lSplittedActionMessageVector[1]);
				mClientActionRequests.push_back(lRequestAction);
			}

		}
		

	}else
	{
		return false;
	}
	return true;
}

vector<string> ClientRequestModel::splitStringInputMessage(string aInputMessage)
{
	vector<string> lSplittedVector=split(aInputMessage,' ');
	//boost::split(lSplittedVector, aInputMessage, boost::is_any_of(" "));
	return lSplittedVector;
}

vector<string> ClientRequestModel::splitStringActionMessage(string aActionMessage)
{
	vector<string> lSplittedVector =split(aActionMessage,'=');
	//boost::split(lSplittedVector, aActionMessage, boost::is_any_of("="));
	return lSplittedVector;
}

vector<string> ClientRequestModel::split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}