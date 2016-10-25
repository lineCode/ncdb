#pragma once
#include "PiTypeDef.h"
#include "functional.h"

#define IMP_OTHER_READ_WRITE(InterFaceT, FieldName) \
    IMP_READ_WRITE_COMMON(CPAXmlOther, InterFaceT, XML_PA_OTHER, FieldName)

struct XML_PA_OTHER
{
    tstring strSaveName;
    tstring strVideoFormat;
    tstring strCaptureRate;
    tstring strCaptureVoice;
    tstring strCaptureMouse;    
    tstring strMin;
    tstring strStartCapture;
    tstring strPauseCapture;
    tstring strStopCapture;
    tstring strAutoLogin;
    tstring strToken;
    tstring strNerverNotify;
    tstring strGUID;
};

enum ENUM_DATA_DUTY_OTHER 
{
    ddso_save_name = 0,
    ddso_video_format,
    ddso_capture_rate,
    ddso_capture_voice,
    ddso_capture_mouse, 
    ddso_min,
    ddso_start_capture,
    ddso_pause_capture,
    ddso_stop_capture,
    ddso_auto_login,
    ddso_token,      
    ddso_nerver_notify,
    ddso_guid,
};


//xml文件字段定义

static const TCHAR*        STRING_XML_OTHER_SCREENCAPTURE                       = _T("ScreenCapture");
static const TCHAR*        STRING_XML_OTHER_SC_SAVENAME                         = _T("SaveName");
static const TCHAR*        STRING_XML_OTHER_SC_VIDEOFORMAT                      = _T("VideoFormat");
static const TCHAR*        STRING_XML_OTHER_SC_CAPTURERATE                      = _T("CaptureRate");
static const TCHAR*        STRING_XML_OTHER_SC_CAPTUREVOICE                     = _T("CaptureVoice");
static const TCHAR*        STRING_XML_OTHER_SC_CAPTUREMOUSE                     = _T("CaptureMouse");
static const TCHAR*        STRING_XML_OTHER_SC_MIN                              = _T("Min");
static const TCHAR*        STRING_XML_OTHER_SC_SHORTKEY                         = _T("ShortKey");
static const TCHAR*        STRING_XML_OTHER_SC_START_CAPTURE                    = _T("StartCapture");
static const TCHAR*        STRING_XML_OTHER_SC_PAUSE_CAPTURE                    = _T("PauseCapture");
static const TCHAR*        STRING_XML_OTHER_SC_STOP_CAPTURE                     = _T("StopCapture");
static const TCHAR*        STRING_XML_OTHER_SC_USER_LOGIN                       = _T("UserLogin");
static const TCHAR*        STRING_XML_OTHER_SC_AUTO_LOGIN                       = _T("AutoLogin");
static const TCHAR*        STRING_XML_OTHER_SC_TOKEN                            = _T("Token");
static const TCHAR*        STRING_XML_OTHER_SC_MONITOR                          = _T("Monitor");
static const TCHAR*        STRING_XML_OTHER_SC_NERVERNOTIFY                     = _T("NerverNotify");
static const TCHAR*        STRING_XML_OTHER_SC_APP                              = _T("App");
static const TCHAR*        STRING_XML_OTHER_SC_GUID                             = _T("GUID");



//xml字段默认值定义
static const TCHAR*        STRING_VALUE_OTHER_SAVENAME               = _T("录像1");
static const TCHAR*        STRING_VALUE_OTHER_VIDEOFORMAT            = _T("1");
static const TCHAR*        STRING_VALUE_OTHER_CAPTURERATE            = _T("12");
static const TCHAR*        STRING_VALUE_OTHER_CAPTUREVOICE           = _T("1");
static const TCHAR*        STRING_VALUE_OTHER_CAPTUREMOUSE           = _T("0");
static const TCHAR*        STRING_VALUE_OTHER_MIN                    = _T("1");
static const TCHAR*        STRING_VALUE_OTHER_STARTCAPTURE           = _T("K");
static const TCHAR*        STRING_VALUE_OTHER_PAUSECAPTURE           = _T("P");
static const TCHAR*        STRING_VALUE_OTHER_STOPCAPTURE            = _T("Q");
static const TCHAR*        STRING_VALUE_OTHER_AUTOLOGIN              = _T("1");
static const TCHAR*        STRING_VALUE_OTHER_TOKEN                     = _T("");
static const TCHAR*        STRING_VALUE_OTHER_NERVERNOTIFY              = _T("0");
static const tstring        STRING_VALUE_OTHER_GUID                      = MakeGUID();
                                                   




