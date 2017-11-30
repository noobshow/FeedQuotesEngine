#include "MySQLDBOperation.h"



MySQLDBOperation::MySQLDBOperation(string aServerIPAddress,string aDBUserName,string aDBUserPassword,string aTargetDatabase)
{
	try
	{
		m_Driver=get_driver_instance();
		mServerIPAddress.assign(aServerIPAddress);
		mDBUserName.assign(aDBUserName);
		mDBUserPassword.assign(aDBUserPassword);
		mTargetDatabase.assign(aTargetDatabase);
		ConnectToDB();
	}catch(sql::SQLException aEx)
	{
		cout<< "SQL Exception Occured "<<aEx.what()<<endl;
	}
}

int MySQLDBOperation::ConnectToDB()
{
	try
	{
		if (m_Driver)
		{
			m_Connection=m_Driver->connect(mServerIPAddress.c_str(),mDBUserName.c_str(),mDBUserPassword.c_str());
			m_Connection->setSchema(mTargetDatabase.c_str());
		}
		return 1;
	}catch(sql::SQLException aEx)
	{
		cout<< "SQL Exception Occured "<<aEx.what()<<endl;
		delete m_Connection;
		m_Connection=NULL;
		return 0;
	}	
}

sql::ResultSet  *MySQLDBOperation::executeSelectQuery(string aQueryString)
{
	sql::Statement *lStatement=0;
	sql::ResultSet *lResultSet=0;
	try
	{
		if(!m_Connection)
		{
			ConnectToDB();
		}else
		{
			lStatement=m_Connection->createStatement();
			lResultSet=lStatement->executeQuery(aQueryString.c_str());
		}
	}catch(sql::SQLException aEx)
	{
		cout<< "SQL Exception Occured "<<aEx.what()<<endl;
		delete m_Connection;
		m_Connection=NULL;
		int k=0;
	}
	delete lStatement;
	//delete m_Connection;
	return lResultSet;
}


int MySQLDBOperation::executeUpdateQuery(string aQueryString)
{
	sql::Statement *lStatement=NULL;
	sql::ResultSet *lResultSet=0;
	try
	{
		if(!m_Connection)
		{
			ConnectToDB();
		}else
		{
			lStatement=m_Connection->createStatement();
			lStatement->executeUpdate(aQueryString.c_str());
		}
	}catch(sql::SQLException aEx)
	{
		cout<< "SQL Exception Occured "<<aEx.what()<<endl;
		delete m_Connection;
		m_Connection=NULL;
		int k=0;
		return false;
	}
	delete lStatement;
	//delete m_Connection;
	return true;
}

MySQLDBOperation::~MySQLDBOperation(void)
{
	delete m_Connection;
}
