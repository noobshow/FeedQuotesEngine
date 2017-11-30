// Log.h
#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include "../StdAfx.h"


using namespace std;


/**
 * 
 */
class FXTFLog
{
private:
	tm *m_PreviousTime;
public:
    FXTFLog();
    ~FXTFLog();

    bool Open(string strFileName);
    void Close();

    bool CommonLogInit(); //
	bool CheckIsDateChanged();

    void Enable();
    void Disable();

    string GetTimeStr();

    template <typename T> void LogOut(const T& value)
    {
        if (m_bEnabled)
        {
            m_tOLogFile << value;
        }
    }

    template <typename T> void LogOutLn(const T& value)
    {
        if (m_bEnabled && !CheckIsDateChanged())
        {
            m_tOLogFile <<GetTimeStr()<<" : "<< value << endl;
		}else
		{
			CommonLogInit();
			Enable();
			GetTimeStr();
			LogOut("");
			m_tOLogFile <<GetTimeStr()<<" : "<< value << endl;
		}
    }

    void LogOutLn()
    {
        if (m_bEnabled)
        {
            m_tOLogFile << endl;
        }
    }

    template <typename T> FXTFLog& operator<<(const T& value)
    {
        if (m_bEnabled)
        {
            m_tOLogFile << value;
        }
        return (*this);
    }

    FXTFLog& operator<<(ostream& (*_Pfn)(ostream&))
    {
        if (m_bEnabled)
        {
            (*_Pfn)(m_tOLogFile);
        }
        return (*this);
    }

private:
    template<typename T> string ValueToStr(T value)
    {
        ostringstream ost;
        ost << value;
        return ost.str();
    }
private:
    ofstream m_tOLogFile;

    bool m_bEnabled;
};


#endif

