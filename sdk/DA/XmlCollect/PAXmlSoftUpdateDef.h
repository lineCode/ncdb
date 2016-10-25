#pragma once
#include "PiTypeDef.h"

#define IMP_SU_READ_WRITE(InterFaceT, FieldName) \
    IMP_READ_WRITE_COMMON(CPAXmlSoftUpdate, InterFaceT, XML_PA_SOFT_UPDATE, FieldName)

struct XML_PA_SOFT_UPDATE
{
    tstring strUpdateTime;
    tstring strPAVersion;
    tstring strMMVersion;
    tstring strMMEduUrl;
    tstring strMMExUrl;
    tstring strIMVersion;
    tstring strIMEduUrl;
    tstring strIMExUrl;
    tstring strCSPVersion;
    tstring strCSAVersion;
    tstring strCSCVersion;
};

enum ENUM_DATA_DUTY_SU 
{
    ddsu_update_time = 0,
    ddsu_pa_version,
    ddsu_mm_version,
    ddsu_mm_edu_url,
    ddsu_mm_ex_url,
    ddsu_im_version,
    ddsu_im_edu_url,
    ddsu_im_ex_url,
    ddsu_csp_version,
    ddsu_csa_version,
    ddsu_csc_version,
};




//xml文件字段定义

static const TCHAR*        STRING_XML_SU_Update                            = _T("Update");
static const TCHAR*        STRING_XML_SU_Update_Timer                            = _T("Timer");
static const TCHAR*        STRING_XML_SU_PA                                 = _T("DrawerAssistant");
static const TCHAR*        STRING_XML_SU_PA_Version                         = _T("Version");
static const TCHAR*        STRING_XML_SU_ELEM_PA                            = _T("DrawerAssistant");
static const TCHAR*        STRING_XML_SU_MM                        = _T("MangaMaster");
static const TCHAR*        STRING_XML_SU_MM_Version                = _T("Version");
static const TCHAR*        STRING_XML_SU_MM_EduURL                    = _T("EduURL");
static const TCHAR*        STRING_XML_SU_MM_ExURL             = _T("ExURL");
static const TCHAR*        STRING_XML_SU_IM                     = _T("IllustMaster");
static const TCHAR*        STRING_XML_SU_IM_Version             = _T("Version");
static const TCHAR*        STRING_XML_SU_IM_EduURL             = _T("EduURL");
static const TCHAR*        STRING_XML_SU_IM_ExURL             = _T("ExURL");
static const TCHAR*        STRING_XML_SU_CSP                        = _T("CSP");
static const TCHAR*        STRING_XML_SU_CSP_Version                = _T("Version");
static const TCHAR*        STRING_XML_SU_CSA                        = _T("CSA");
static const TCHAR*        STRING_XML_SU_CSA_Version                = _T("Version");
static const TCHAR*        STRING_XML_SU_CSC                        = _T("CSC");
static const TCHAR*        STRING_XML_SU_CSC_Version                = _T("Version");



//xml字段默认值定义
static const TCHAR*        STRING_VALUE_SU_Update_Timer             = _T("60");
static const TCHAR*        STRING_VALUE_SU_PA_Version               = _T("1.0.0.1");
static const TCHAR*        STRING_VALUE_SU_MM_Version               = _T("1.0.0.1");
static const TCHAR*        STRING_VALUE_SU_MM_EduURL                = _T("http://www.baidu.com");
static const TCHAR*        STRING_VALUE_SU_MM_ExURL                 = _T("http://www.baidu.com");
static const TCHAR*        STRING_VALUE_SU_IM_Version               = _T("1.0.0.1");
static const TCHAR*        STRING_VALUE_SU_IM_EduURL                = _T("http://www.baidu.com");
static const TCHAR*        STRING_VALUE_SU_IM_ExURL                 = _T("http://www.baidu.com");
static const TCHAR*        STRING_VALUE_SU_CSP_Version              = _T("1.0.0.1");
static const TCHAR*        STRING_VALUE_SU_CSA_Version              = _T("1.0.0.1");
static const TCHAR*        STRING_VALUE_SU_CSC_Version              = _T("1.0.0.1");










