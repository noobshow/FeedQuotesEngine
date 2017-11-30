//========================
//Log.cpp
#include "../StdAfx.h"
#include "FXTFLog.h"

FXTFLog::FXTFLog()
    :m_bEnabled(true)
{
}

FXTFLog::~FXTFLog()
{
}

bool FXTFLog::Open(string sFileName)
{
    m_tOLogFile.open(sFileName.c_str(), ios_base::out | ios_base::app);

    if( !m_tOLogFile )
    {
        return false;
    }

    return true;
}

void FXTFLog::Close()
{
    if(m_tOLogFile.is_open())
    {
        m_tOLogFile.close();
    }
}

bool FXTFLog::CommonLogInit()
{
    time_t tNowTime;
    time(&tNowTime);
	Close();
    tm* tLocalTime = localtime(&tNowTime);
	m_PreviousTime=tLocalTime;
    //
    string sDateStr = ValueToStr(tLocalTime->tm_year+1900) + "-" +
        ValueToStr(tLocalTime->tm_mon+1) + "-" +
        ValueToStr(tLocalTime->tm_mday);

    return Open("Logs\\FXTFLog_" + sDateStr + ".log");
}

bool FXTFLog::CheckIsDateChanged()
{
	time_t tNowTime;
    time(&tNowTime);

    tm* tLocalTime = localtime(&tNowTime);
	if (tLocalTime->tm_year!=m_PreviousTime->tm_year || tLocalTime->tm_mon!=m_PreviousTime->tm_mon || tLocalTime->tm_mday!=m_PreviousTime->tm_mday)
	{
		return true;
	}
	return false;
}

void FXTFLog::Enable()
{
    m_bEnabled = true;
}

void FXTFLog::Disable()
{
    m_bEnabled = false;
}

//
string FXTFLog::GetTimeStr()
{
    time_t tNowTime;
    time(&tNowTime);

    tm* tLocalTime = localtime(&tNowTime);

    //"2011-07-18 23:03:01 ";
    string strFormat = "%Y-%m-%d %H:%M:%S ";

    char strDateTime[30] = {'\0'};
    strftime(strDateTime, 30, strFormat.c_str(), tLocalTime);

    string strRes = strDateTime;

    return strRes;
}
