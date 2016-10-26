#include "StdAfx.h"
#include "PAXmlOther.h"
#include "XmlPathMgr.h"


CPAXmlOther::CPAXmlOther(void)
{
}


CPAXmlOther::~CPAXmlOther(void)
{
}



tstring CPAXmlOther::GetXmlFullPath()
{
    tstring strPath;
    strPath = CXmlPathMgr::GetVideoXmlPath();

    //strPath = _T("D:\\test\\DA\\PAOther.xml");
    return strPath;
}

bool CPAXmlOther::CreateXmlData()
{
    m_pXmlUtil->SetDoc(NULL);
    m_pXmlUtil->SetDoc(_T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"));

    DealBoolRet( m_pXmlUtil->AddElem(_T("root")) );
    m_pXmlUtil->IntoElem();


    DealBoolRet( m_pXmlUtil->AddElem(STRING_XML_OTHER_SCREENCAPTURE) );
    m_pXmlUtil->IntoElem();

    DealBoolRet( m_pXmlUtil->AddElem(STRING_XML_OTHER_SC_SAVENAME, m_ConfigData.strSaveName.c_str()) );
    DealBoolRet( m_pXmlUtil->AddElem(STRING_XML_OTHER_SC_VIDEOFORMAT, m_ConfigData.strVideoFormat.c_str()) );
    DealBoolRet( m_pXmlUtil->AddElem(STRING_XML_OTHER_SC_CAPTUREVOICE, m_ConfigData.strCaptureVoice.c_str()) );
    DealBoolRet( m_pXmlUtil->AddElem(STRING_XML_OTHER_SC_CAPTUREMOUSE, m_ConfigData.strCaptureMouse.c_str()) );
    DealBoolRet( m_pXmlUtil->AddElem(STRING_XML_OTHER_SC_MIN, m_ConfigData.strMin.c_str()) );

    DealBoolRet( m_pXmlUtil->AddElem(STRING_XML_OTHER_SC_SHORTKEY) );
    DealBoolRet( m_pXmlUtil->AddChildElem(STRING_XML_OTHER_SC_START_CAPTURE, m_ConfigData.strStartCapture.c_str()) );
    DealBoolRet( m_pXmlUtil->AddChildElem(STRING_XML_OTHER_SC_PAUSE_CAPTURE, m_ConfigData.strPauseCapture.c_str()) );
    DealBoolRet( m_pXmlUtil->AddChildElem(STRING_XML_OTHER_SC_STOP_CAPTURE, m_ConfigData.strStopCapture.c_str()) );

    m_pXmlUtil->OutOfElem();//ScreenCapture, 第二层
    DealBoolRet( m_pXmlUtil->AddElem(STRING_XML_OTHER_SC_USER_LOGIN) ); 
    DealBoolRet( m_pXmlUtil->AddChildElem(STRING_XML_OTHER_SC_AUTO_LOGIN, m_ConfigData.strAutoLogin.c_str()) );

    DealBoolRet( m_pXmlUtil->AddChildElem(STRING_XML_OTHER_SC_TOKEN, m_ConfigData.strToken) );

    DealBoolRet( m_pXmlUtil->AddElem(STRING_XML_OTHER_SC_MONITOR) );
    DealBoolRet( m_pXmlUtil->AddChildElem(STRING_XML_OTHER_SC_NERVERNOTIFY, m_ConfigData.strNerverNotify) );

    DealBoolRet( m_pXmlUtil->AddElem(STRING_XML_OTHER_SC_APP) );
    DealBoolRet( m_pXmlUtil->AddChildElem(STRING_XML_OTHER_SC_GUID, m_ConfigData.strGUID) );
    return true;

}

bool CPAXmlOther::InitDefaultXmlStruct()
{
    m_ConfigData.strSaveName        = STRING_VALUE_OTHER_SAVENAME;
    m_ConfigData.strVideoFormat     = STRING_VALUE_OTHER_VIDEOFORMAT;
    m_ConfigData.strCaptureRate     = STRING_VALUE_OTHER_CAPTURERATE;
    m_ConfigData.strCaptureVoice    = STRING_VALUE_OTHER_CAPTUREVOICE;
    m_ConfigData.strCaptureMouse    = STRING_VALUE_OTHER_CAPTUREMOUSE;
    m_ConfigData.strMin             = STRING_VALUE_OTHER_MIN;
    m_ConfigData.strStartCapture    = STRING_VALUE_OTHER_STARTCAPTURE;
    m_ConfigData.strPauseCapture    = STRING_VALUE_OTHER_PAUSECAPTURE;
    m_ConfigData.strStopCapture     = STRING_VALUE_OTHER_STOPCAPTURE;
    m_ConfigData.strAutoLogin       = STRING_VALUE_OTHER_AUTOLOGIN;
    m_ConfigData.strToken           = STRING_VALUE_OTHER_TOKEN;
    m_ConfigData.strNerverNotify    = STRING_VALUE_OTHER_NERVERNOTIFY;
    m_ConfigData.strGUID                = STRING_VALUE_OTHER_GUID;
    return true;

}

bool CPAXmlOther::DoMapXmlDataToMem()
{
    m_pXmlUtil->ResetPos();

    DealBoolRet( m_pXmlUtil->FindElem() );
    m_pXmlUtil->IntoElem(); //进入root

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_OTHER_SCREENCAPTURE) );
    m_pXmlUtil->IntoElem(); //进入ScreenCapture


    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_OTHER_SC_SAVENAME) );
    m_ConfigData.strSaveName = m_pXmlUtil->GetData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_OTHER_SC_VIDEOFORMAT) );
    m_ConfigData.strVideoFormat = m_pXmlUtil->GetData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_OTHER_SC_CAPTUREVOICE) );
    m_ConfigData.strCaptureVoice = m_pXmlUtil->GetData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_OTHER_SC_CAPTUREMOUSE) );
    m_ConfigData.strCaptureMouse = m_pXmlUtil->GetData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_OTHER_SC_MIN) );
    m_ConfigData.strMin = m_pXmlUtil->GetData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_OTHER_SC_SHORTKEY) );
    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_OTHER_SC_START_CAPTURE) );
    m_ConfigData.strStartCapture = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_OTHER_SC_PAUSE_CAPTURE) );
    m_ConfigData.strPauseCapture = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_OTHER_SC_STOP_CAPTURE) );
    m_ConfigData.strStopCapture = m_pXmlUtil->GetChildData();


    m_pXmlUtil->OutOfElem();//ScreenCapture, 第二层
    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_OTHER_SC_USER_LOGIN) ); 
    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_OTHER_SC_AUTO_LOGIN) );
    m_ConfigData.strAutoLogin = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_OTHER_SC_TOKEN) );
    m_ConfigData.strToken = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_OTHER_SC_MONITOR) );
    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_OTHER_SC_NERVERNOTIFY) );
    m_ConfigData.strNerverNotify = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_OTHER_SC_APP) );
    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_OTHER_SC_GUID) );
    m_ConfigData.strGUID = m_pXmlUtil->GetChildData();

    return true;

}

bool CPAXmlOther::ReadWriteData( XML_PA_OTHER& ucData, ENUM_DATA_DUTY_OTHER da, tstring& strData )
{
    bool bRet = false;
    switch(da)
    {
        Imp_Duty_Cn_Interface(ddso_save_name,           ReadWriteSaveName       ) 
        Imp_Duty_Cn_Interface(ddso_video_format,        ReadWriteVideoFormat    )    
        Imp_Duty_Cn_Interface(ddso_capture_rate,        ReadWriteCaptureRate    )    
        Imp_Duty_Cn_Interface(ddso_capture_voice,       ReadWriteCaptureVoice   ) 
        Imp_Duty_Cn_Interface(ddso_capture_mouse,       ReadWriteCaptureMouse   ) 	
        Imp_Duty_Cn_Interface(ddso_min,                 ReadWriteMin            )            
        Imp_Duty_Cn_Interface(ddso_start_capture,       ReadWriteStartCapture   ) 
        Imp_Duty_Cn_Interface(ddso_pause_capture,       ReadWritePauseCapture   ) 	
        Imp_Duty_Cn_Interface(ddso_stop_capture,        ReadWriteStopCapture    ) 
        Imp_Duty_Cn_Interface(ddso_auto_login,          ReadWriteAutoLogin      ) 
        Imp_Duty_Cn_Interface(ddso_token,               ReadWriteToken            ) 
        Imp_Duty_Cn_Interface(ddso_nerver_notify,       ReadWriteNerverNotify      ) 
        Imp_Duty_Cn_Interface(ddso_guid,                ReadWriteNerverGUID      ) 
    }
    //把数据覆盖写入到xml
    return true;
}
