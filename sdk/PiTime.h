#pragma once
#include <Windows.h>
#include "PiTypeDef.h"

#define STR_DATATIME_FORMAT "%04d/%02d/%02d %02d:%02d:%02d"
typedef map<tstring, int> MAP_MONTH_ABBR;
class CPiTime
{

public:
	CPiTime(void);
	~CPiTime(void);
public:
	static tstring GetLocalDateTime(tcpchar szFormat = _T("%04d-%02d-%02d %02d:%02d:%02d"));

	//时间戳转成其他格式字符串的时间
	static string SwitchTimeStampToStr(const char* cTimestamp, const char* cFormat = STR_DATATIME_FORMAT);

	//获取本地日期      
	static tstring GetLocalDate(tcpchar szFormat = _T("%04d-%02d-%02d"));

	//获取本地时间(h,m,s) 
	static tstring GetSysTime();

	//获取1970年来经过毫秒数,UTC时间
	static __time64_t GetTimestampMilliSecUTC();

	//获取系统当前的毫秒数, 0 - 999
	static UNINT GetCurrentMillionSecond();
	static tm ST2TM(SYSTEMTIME* );
	static SYSTEMTIME TimestampToST(const char* cTimestamp);

	/*
	    fun:  时间戳字符串转成1970年来经过的秒数
	    note: 参数格式为 yyyymmddhh(24)mimissMilS
	*/
	static time_t TimestampToSecond1970(const char* cTimestamp);
	//1970年来经过的毫秒数
	static time_t TimestampToMilSecond1970(const char* cTimestamp);

	//将时间结构体转成字符串
	static string TimeToString(SYSTEMTIME* st, const char* format =  STR_DATATIME_FORMAT);
	static string TimeToString(tm* pTM, const char* format = "%04d/%02d/%02d %02d:%02d:%02d");
	//转成1970年经过的毫秒数, 格式说明符, 年月日, 时分秒, 有个字段找不到, 则后面的都不会查找
	static time_t TimeStrToIntSecond1970(tcpchar szTime, tcpchar szFormat = _T("%4d_%2d_%2d %2d:%2d:%2d"));

	static SYSTEMTIME TM2ST(tm* pTM);
	/************************************************************************/
	/*  fun: 1970年经过的毫秒数转成 SYSTEMTIME 结构
	/************************************************************************/
	static SYSTEMTIME MillionSecond1970ToST(const time_t& timeTemp);

	static tstring MilliSecond1970ToString(const time_t& timeTemp, tcpchar format = _T("%04d-%02d-%02d %02d:%02d:%02d:%03d"));
	static tstring Second1970ToString(const time_t& timeTemp, tcpchar format = _T("%04d-%02d-%02d %02d:%02d:%02d"));

	//指定的时分秒字符串转成数值秒, 最高单位为时, 可超过24小时

	static bool TimeStrToSecondHighHour( tcpchar szTime, time_t* pTime, tcpchar szFormat = _T("%d:%2d:%2d") );
	static tstring TimeShortToTimeStdString(const tstring& strTime);
	static tstring SecondToTimeStrHighHour(time_t nSecond, tcpchar szFormat = _T("%02d:%02d:%02d") );
	//月份简写字符串对应的月份数
	static MAP_MONTH_ABBR GetMonthAbbrNameMap();
	static tstring VariantDTToString(double dt);
	static tstring GetTimestamp();

};
