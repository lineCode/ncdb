#pragma once
#include "PiTypeDef.h"

#define IMP_UH_READ_WRITE(InterFaceT, FieldName) \
    IMP_READ_WRITE_COMMON(CPAXmlUserHobby, InterFaceT, XML_USER_HOBBY_DATA, FieldName)


struct XML_USER_HOBBY_DATA
{
    tstring psUseCount;
    tstring saiUseCount;

    tstring daUseCount;
    tstring daUseTime;

    tstring daUFMyAssistantCount;
    tstring daUFWelcom;
    tstring daUFworksprotect;
    tstring daUFConfigInCloud;
    tstring daUFDriverManage;
    tstring daUFSoftManage;
    tstring daUFFontSave;

    tstring daUFVideo;
    tstring daUFInspiration;
};


//用户爱好信息
enum ENUM_DATA_DUTY_UH
{
    dnh_unKnown = 0,
    dnh_ps_use_Count,
    dnh_sai_use_count,
    dnh_da_use_count,
    dnh_da_use_time,
    dnh_da_uf_my_assistant_count,
    dnh_da_uf_welcom,
    dnh_da_uf_worksprotect,
    dnh_da_uf_config_in_cloud,
    dnh_da_uf_driver_manage,
    dnh_da_uf_soft_manage,
    dnh_da_uf_font_save,
    dnh_da_uf_video,
    dnh_da_uf_inspiration,
};

#define STRING_TIME_ZERO _T("00:00:00")

//static const TCHAR*        STRING_VALUE_UH_ELEM_DA_USETIME                    = _T("00:00:00");

//xml文件字段定义

static const TCHAR*        STRING_XML_UH_ELEM_PS                            = _T("PhotoShop");
static const TCHAR*        STRING_XML_UH_ELEM_PS_USECOUNT                   = _T("UseCount");
static const TCHAR*        STRING_XML_UH_ELEM_SAI                           = _T("SAI");
static const TCHAR*        STRING_XML_UH_ELEM_SAI_USECOUNT                  = _T("UseCount");
static const TCHAR*        STRING_XML_UH_ELEM_PA                            = _T("DrawerAssistant");
static const TCHAR*        STRING_XML_UH_ELEM_PA_USECOUNT                   = _T("UseCount");
static const TCHAR*        STRING_XML_UH_ELEM_PA_USETIME                    = _T("UseTime");
//使用功能
static const TCHAR*        STRING_XML_UH_ELEM_PA_USEFUNC                    = _T("UseFunc");
static const TCHAR*        STRING_XML_UH_ELEM_PA_UF_MYASSISTANT             = _T("MyAssistant");
static const TCHAR*        STRING_XML_UH_ELEM_PA_UF_MYASSISTANT_COUNT       = _T("count");

static const TCHAR*        STRING_XML_UH_ELEM_PA_UF_WELCOM                  = _T("Welcom");
static const TCHAR*        STRING_XML_UH_ELEM_PA_UF_WORKSPROTECT            = _T("WorksProtect");
static const TCHAR*        STRING_XML_UH_ELEM_PA_UF_ConfigInCloud           = _T("ConfigInCloud");
static const TCHAR*        STRING_XML_UH_ELEM_PA_UF_DriverManage            = _T("DriverManage");
static const TCHAR*        STRING_XML_UH_ELEM_PA_UF_SoftManage              = _T("SoftManage");
static const TCHAR*        STRING_XML_UH_ELEM_PA_UF_FontSave                = _T("FontSave");

static const TCHAR*        STRING_XML_UH_ELEM_PA_UF_VIDEO                   = _T("Vedio");
static const TCHAR*        STRING_XML_UH_ELEM_PA_UF_Inspiration             = _T("Inspiration");



//xml字段默认值定义
static const TCHAR*        STRING_VALUE_UH_ELEM_PS_USECOUNT                   = _T("0");
static const TCHAR*        STRING_VALUE_UH_ELEM_SAI_USECOUNT                  = _T("0");

static const TCHAR*        STRING_VALUE_UH_ELEM_PA_USECOUNT                   = _T("0");
static const TCHAR*        STRING_VALUE_UH_ELEM_PA_USETIME                    = STRING_TIME_ZERO;
//使用功能
static const TCHAR*        STRING_VALUE_UH_ELEM_PA_UF_MYASSISTANT_COUNT       = _T("0");
static const TCHAR*        STRING_VALUE_UH_ELEM_PA_UF_WELCOM                  = _T("0");
static const TCHAR*        STRING_VALUE_UH_ELEM_PA_UF_WORKSPROTECT            = _T("0");
static const TCHAR*        STRING_VALUE_UH_ELEM_PA_UF_CONFIGINCLOUD           = _T("0");
static const TCHAR*        STRING_VALUE_UH_ELEM_PA_UF_DRIVERMANAGE            = _T("0");
static const TCHAR*        STRING_VALUE_UH_ELEM_PA_UF_SOFTMANAGE              = _T("0");
static const TCHAR*        STRING_VALUE_UH_ELEM_PA_UF_FONTSAVE                = _T("0");

static const TCHAR*        STRING_VALUE_UH_ELEM_PA_UF_VEDIO                   = _T("0");
static const TCHAR*        STRING_VALUE_UH_ELEM_PA_UF_INSPIRATION             = _T("0");
