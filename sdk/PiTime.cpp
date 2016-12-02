
#include "stdafx.h"
#include "PiTime.h"
#include <time.h>
#include <sys/timeb.h>
#include "AutoType.h"
#include "Math/mathUnit.h"
#include "StrCoding.h"

Pi_NameSpace_Using
CPiTime::CPiTime(void)
{
}

CPiTime::~CPiTime(void)
{
}

tstring CPiTime::GetLocalDateTime( tcpchar szFormat /*= _T("%04d-%02d-%02d %02d:%02d:%02d")*/ )
{
	//time();

	SYSTEMTIME st = {0};
	::GetLocalTime(&st);
	tchar cBuf[50] = {0};
	_stprintf_s(cBuf, _countof(cBuf), szFormat,
		st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute, st.wSecond);
	return cBuf;
}

/************************************************************************/
/* 功能:获取本地日期                                                        */
/************************************************************************/
tstring CPiTime::GetLocalDate( tcpchar szFormat /*= _T("%04d-%02d-%02d")*/ )
{
	//time();
	SYSTEMTIME st = {0};
	::GetLocalTime(&st);
	tchar cBuf[40] = {0};
	_stprintf_s(cBuf,  _countof(cBuf), szFormat,
		st.wYear,st.wMonth,st.wDay);
	return cBuf;
}

string CPiTime::SwitchTimeStampToStr(const char* cTimestamp, const char* cFormat)
{	
	string strTime(cTimestamp);
	return TimeToString(&TimestampToST(cTimestamp), cFormat);

}


/************************************************************************
功能:获取本地时间(hh:mm:s)                                                        
***********************************************************************/
tstring CPiTime::GetSysTime()
{
	//time();
	SYSTEMTIME st = {0};
	::GetLocalTime(&st);
	tchar cBuf[40] = {0};
	_stprintf_s(cBuf, _countof(cBuf), _T("%02d:%02d:%02d"),
		st.wHour,st.wMinute,st.wSecond);
	return cBuf;
}

tm CPiTime::ST2TM(SYSTEMTIME* pST)
{
	/* y-m-d h:m:s */

	tm tmTemp = {0};
	tmTemp.tm_year = pST->wYear - 1900;
	tmTemp.tm_mon = pST->wMonth - 1;
	tmTemp.tm_mday = pST->wDay;
	tmTemp.tm_hour = pST->wHour;
	tmTemp.tm_min = pST->wMinute;
	tmTemp.tm_sec = pST->wSecond;
	return tmTemp;
}

string CPiTime::TimeToString(SYSTEMTIME* st, const char* format /*=  %d/%d/%d /%d:%d */)
{
	char cBuf[40] = {0};
	sprintf_s(cBuf, sizeof(cBuf), format, st->wYear, st->wMonth, st->wDay,
		st->wHour, st->wMinute, st->wSecond);

	return cBuf;
}

//string TimeMillSecondToString(SYSTEMTIME* st, const char* format /*=  %d/%d/%d /%d:%d */)
//{
//    char cBuf[40] = {0};
//    sprintf_s(cBuf, sizeof(cBuf), format, st->wYear, st->wMonth, st->wDay,
//        st->wHour, st->wMinute, st->wSecond, st->wMilliseconds);
//    
//    return cBuf;
//}

string CPiTime::TimeToString(tm* pTM, const char* format /*=  %d/%d/%d /%d:%d */)
{
	return TimeToString(&TM2ST(pTM), format);
}




SYSTEMTIME CPiTime::TimestampToST(const char* cTimestamp)
{
	string strTime(cTimestamp);
	SYSTEMTIME st;
	st.wYear	= atoi(strTime.substr(0, 4).c_str());
	st.wMonth	= atoi(strTime.substr(4, 2).c_str());
	st.wDay		= atoi(strTime.substr(6, 2).c_str());

	st.wHour	= atoi(strTime.substr(8, 2).c_str());
	st.wMinute	= atoi(strTime.substr(10, 2).c_str());
	st.wSecond	= atoi(strTime.substr(12, 2).c_str());
	if (strTime.size() > 14)
	{
		//毫秒的处理..................
		st.wMilliseconds = atoi(strTime.substr(14, 3).c_str());
	}

	return st;
}

time_t CPiTime::TimestampToSecond1970(const char* cTimestamp)
{
	if (!cTimestamp || !strlen(cTimestamp))
	{
		return 0;
	}
	return mktime(&ST2TM(&TimestampToST(cTimestamp)));
}

time_t CPiTime::TimestampToMilSecond1970(const char* cTimestamp)
{

	return TimestampToSecond1970(cTimestamp) * 1000 + atoi(cTimestamp + 14);
}

__time64_t CPiTime::GetTimestampMilliSecUTC()
{
	__timeb64 t;
	_ftime64_s(&t);
	return t.time * 1000 + t.millitm;
}

UNINT CPiTime::GetCurrentMillionSecond()
{
	__timeb64 t = {0};
	_ftime64_s(&t);
	return t.millitm;
}

time_t CPiTime::TimeStrToIntSecond1970(tcpchar szTime, tcpchar szFormat)
{
	time_t t = 0;
	tm tmSrut = {0};

	int nRet = _stscanf_s(szTime, _T("%4d_%2d_%2d %2d:%2d:%2d"), &tmSrut.tm_year, &tmSrut.tm_mon, &tmSrut.tm_mday
		, &tmSrut.tm_hour, &tmSrut.tm_min, &tmSrut.tm_sec);

	//调整下数据表示和结构体的数据解释
	tmSrut.tm_year -= 1900;
	tmSrut.tm_mon -= 1;

	t = mktime(&tmSrut);
	return t;
}


SYSTEMTIME CPiTime::TM2ST(tm* pTM)
{
	SYSTEMTIME st = {0};
	st.wYear	= pTM->tm_year + 1900;
	st.wMonth	= pTM->tm_mon + 1;
	st.wDay		= pTM->tm_mday;
	st.wHour	= pTM->tm_hour;
	st.wMinute	= pTM->tm_min;
	st.wSecond	= pTM->tm_sec;
	return st;
}

SYSTEMTIME CPiTime::MillionSecond1970ToST( const time_t& timeTemp )
{
	CAutoType at;
	at.SetData(timeTemp);


	time_t timeSecond = timeTemp / 1000;
	struct tm ptimeStruct;

	localtime_s(&ptimeStruct,&timeSecond);
	SYSTEMTIME st = TM2ST(&ptimeStruct);
	string strTime = at;
	st.wMilliseconds = atoi(&(strTime.at(strTime.length() - 3)));
	return st;
}

tstring CPiTime::MilliSecond1970ToString( const time_t& timeTemp, tcpchar format /*= _T("%04d-%02d-%02d %02d:%02d:%02d:%03d")*/ )
{


	SYSTEMTIME st = MillionSecond1970ToST(timeTemp);

	tchar cBuf[40] = {0};
	_stprintf_s(cBuf, _countof(cBuf), format, st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	return cBuf;
}



tstring CPiTime::Second1970ToString( const time_t& timeTemp, tcpchar format /*= _T("%04d-%02d-%02d %02d:%02d:%02d")*/ )
{
	return MilliSecond1970ToString(SECOND_TO_MILLISECOND(timeTemp), format);
}


bool CPiTime::TimeStrToSecondHighHour( tcpchar szTime, time_t* pTime, tcpchar szFormat /*= _T("%d:%2d:%2d") */ )
{
	const int nSection = 3;
	time_t tTotalSecond = 0;
	tm tmSrut = {0};

	int nRet = _stscanf_s(szTime, szFormat, &tmSrut.tm_hour, &tmSrut.tm_min, &tmSrut.tm_sec);
	if(nRet != nSection)
	{
		return false;
	}
	tTotalSecond = HOUR_TO_SECOND(tmSrut.tm_hour)
		+ MINUTE_TO_SECOND(tmSrut.tm_min)
		+ tmSrut.tm_sec;

	//调整下数据表示和结构体的数据解释
	//     tmSrut.tm_year -= 1900;
	//     tmSrut.tm_mon -= 1;
	//tTotalSecond = mktime(&tmSrut);
	*pTime = tTotalSecond;
	return true;
}

tstring CPiTime::SecondToTimeStrHighHour( time_t nSecond, tcpchar szFormat /*= _T("%02d:%02d:%02d")*/ )
{
	const int nTimeStrLength = 50;
	int nSecondInciseHour = 0;
	int nSecondInciseHourMin = 0;
	tstring strTime;
	strTime.resize(nTimeStrLength);
	//int nSize = strTime.capacity();
	//strTime.reserve(30);
	//nSize = strTime.capacity();

	tm tmSrut = {0};
	tmSrut.tm_hour = static_cast<int>(SECOND_TO_HOUR(nSecond));
	nSecondInciseHour = static_cast<int>(nSecond - HOUR_TO_SECOND(tmSrut.tm_hour));

	tmSrut.tm_min   = SECOND_TO_MINUTE(nSecondInciseHour);
	nSecondInciseHourMin = nSecondInciseHour - MINUTE_TO_SECOND(tmSrut.tm_min);

	tmSrut.tm_sec   = nSecondInciseHourMin;
	int nRet = _stprintf_s(&strTime.at(0), nTimeStrLength, szFormat, tmSrut.tm_hour, tmSrut.tm_min, tmSrut.tm_sec);

	tstring strRet(strTime.c_str());
	strTime = strRet;
	return strTime;
}



tstring CPiTime::TimeShortToTimeStdString( const tstring& strTime )
{
	tstring strNew;
	if (strTime.empty())
	{
		return strNew;
	}
	MAP_MONTH_ABBR mapMonth = GetMonthAbbrNameMap();
	tchar cBuf[100] = {0};
	tchar strTimeSec[3] = {0};
	tchar cMon[4] = {0};
	SYSTEMTIME st = {0};
	int nYear = 0 ,nMon = 0, nDay = 0, nHour = 0, nMin = 0, nSec = 0;
	_stscanf_s(strTime.c_str(), _T("%3s%d, %d %d:%d:%d %02s")
		, &cMon, _countof(cMon), &st.wDay, &st.wYear, &st.wHour, &st.wMinute, &st.wSecond
		, strTimeSec, _countof(strTimeSec));
	st.wMonth = mapMonth[cMon];
	if (tstring(_T("PM")) == strTimeSec && st.wHour < 12)	//数据 Nov 7, 2014 12:42:20 PM = 12点
	{
		st.wHour += 12;
	}
	strNew = s2t(TimeToString(&st));
	return strNew;
}


MAP_MONTH_ABBR CPiTime::GetMonthAbbrNameMap()
{
	MAP_MONTH_ABBR	mapMonth;
	mapMonth[_T("Jan")] = 1;
	mapMonth[_T("Feb")] = 2;
	mapMonth[_T("Mar")] = 3;
	mapMonth[_T("Apr")] = 4;
	mapMonth[_T("May")] = 5;
	mapMonth[_T("Jun")] = 6;
	mapMonth[_T("Jul")] = 7;
	mapMonth[_T("Aug")] = 8;
	mapMonth[_T("Sep")] = 9;
	mapMonth[_T("Oct")] = 10;
	mapMonth[_T("Nov")] = 11;
	mapMonth[_T("Dec")] = 12;
	return mapMonth;
}


tstring CPiTime::VariantDTToString(double dt)
{
	tstring strTime;
	/*UNSHORT uData = 0;
	UNSHORT uTime = 0;
	if( TRUE != VariantTimeToDosDateTime(dt, &uData, &uTime))
	{
		return strTime;
	}

	FILETIME ft = {0};
	if( !DosDateTimeToFileTime(uData, uTime, &ft) )
	{
		return strTime;
	}*/

	SYSTEMTIME st = {0};

	VariantTimeToSystemTime(dt, &st);
	//FileTimeToSystemTime(&ft, &st);
	return s2t(TimeToString(&st));
}

tstring CPiTime::GetTimestamp()
{
	return CAutoType(time(NULL)).operator tstring();
}

