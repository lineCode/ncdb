#pragma once
#include "PiTypeDef.h"

#define IMP_DRIVER_READ_WRITE(InterFaceT, FieldName) \
    IMP_READ_WRITE_COMMON(CPAXmlDriver, InterFaceT, XML_PA_SOFT_CONFIG, FieldName)

struct XML_PA_DRIVER
{
    tstring strCPUWarnPercent;
    tstring strCPUNormalBkColor;
    tstring strCPUWarnBkColor;
    tstring strMemoryWarnPercent;
    tstring strMemoryNormalBkColor;
    tstring strMemoryWarnBkColor;
    tstring strUrl      ;
    tstring strPipeName ;
    tstring strWndTitle ;

};

enum ENUM_DATA_DUTY_DRIVER
{
    ddsc_cpu_warn_percent = 0,
    ddsc_cpu_normal_bkcolor,
    ddsc_cpu_warn_bkcolor,
    ddsc_memory_warn_percent,
    ddsc_memory_normal_bkcolor,
    ddsc_memory_warn_bkcolor,
    ddsc_updater_url     ,
    ddsc_updater_pipename,
    ddsc_updater_wndtitle,
};




//xml文件字段定义

static const TCHAR*        STRING_XML_SC_MONITOR                        = _T("Monitor");
static const TCHAR*        STRING_XML_SC_CPUWARNPERCENT                 = _T("CPUWarnPercent");
static const TCHAR*        STRING_XML_SC_CPUNORMALBKCOLOR               = _T("CPUNormalBkColor");
static const TCHAR*        STRING_XML_SC_CPUWARNBKCOLOR                 = _T("CPUWarnBkColor");
static const TCHAR*        STRING_XML_SC_MEMORYWARNPERCENT              = _T("MemoryWarnPercent");
static const TCHAR*        STRING_XML_SC_MEMORYNORMALBKCOLOR            = _T("MemoryNormalBkColor");
static const TCHAR*        STRING_XML_SC_MEMORYWARNBKCOLOR              = _T("MemoryWarnBkColor");

static const TCHAR*        STRING_XML_SC_UPDATEINTERFACE                = _T("UpdateInterface");
static const TCHAR*        STRING_XML_SC_UPDATE_URL                     = _T("Url");
static const TCHAR*        STRING_XML_SC_UPDATE_PIPENAME                = _T("PipeName");
static const TCHAR*        STRING_XML_SC_UPDATE_WNDTITLE                = _T("WndTitle");


//xml字段默认值定义
static const TCHAR*        STRING_VALUE_SC_CPUWARNPERCENT             = _T("80");
static const TCHAR*        STRING_VALUE_SC_CPUNORMALBKCOLOR           = _T("0,255,0");
static const TCHAR*        STRING_VALUE_SC_CPUWARNBKCOLOR             = _T("255,0,0");
static const TCHAR*        STRING_VALUE_SC_MEMORYWARNPERCENT          = _T("80");
static const TCHAR*        STRING_VALUE_SC_MEMORYNORMALBKCOLOR        = _T("0,255,0");
static const TCHAR*        STRING_VALUE_SC_MEMORYWARNBKCOLOR          = _T("255,0,0");

//static const TCHAR*        STRING_VALUE_SC_UPDATE_URL                     = _T("http://www.baidu.com");
static const TCHAR*        STRING_VALUE_SC_UPDATE_PIPENAME                = _T("PAPipeName");
static const TCHAR*        STRING_VALUE_SC_UPDATE_WNDTITLE                = _T("_PA_Updater");




