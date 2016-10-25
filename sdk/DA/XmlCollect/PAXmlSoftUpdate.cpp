#include "StdAfx.h"
#include "PAXmlSoftUpdate.h"
#include "XmlPathMgr.h"


CPAXmlSoftUpdate::CPAXmlSoftUpdate(void)
{
}


CPAXmlSoftUpdate::~CPAXmlSoftUpdate(void)
{
}

tstring CPAXmlSoftUpdate::GetXmlFullPath()
{
    tstring strPath;
    CXmlPathMgr xmlPathMng;
    strPath = xmlPathMng.GetUpdateXmlPath();

    return strPath;
}

bool CPAXmlSoftUpdate::CreateXmlData()
{
    m_pXmlUtil->SetDoc(NULL);
    m_pXmlUtil->SetDoc(_T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"));

    m_pXmlUtil->AddElem(_T("root"));
    m_pXmlUtil->IntoElem();


    m_pXmlUtil->AddElem(STRING_XML_SU_Update);
    m_pXmlUtil->AddChildElem(STRING_XML_SU_Update_Timer, m_ConfigData.strUpdateTime.c_str());

    m_pXmlUtil->AddElem(STRING_XML_SU_PA);
    m_pXmlUtil->AddChildElem(STRING_XML_SU_PA_Version, m_ConfigData.strPAVersion.c_str());

    m_pXmlUtil->AddElem(STRING_XML_SU_MM); 

    m_pXmlUtil->AddChildElem(STRING_XML_SU_MM_Version, m_ConfigData.strMMVersion.c_str());
    m_pXmlUtil->AddChildElem(STRING_XML_SU_MM_EduURL, m_ConfigData.strMMEduUrl.c_str());
    m_pXmlUtil->AddChildElem(STRING_XML_SU_MM_ExURL, m_ConfigData.strMMExUrl.c_str());

    m_pXmlUtil->AddElem(STRING_XML_SU_IM);
    m_pXmlUtil->AddChildElem(STRING_XML_SU_IM_Version, m_ConfigData.strIMVersion.c_str());
    m_pXmlUtil->AddChildElem(STRING_XML_SU_IM_EduURL, m_ConfigData.strIMEduUrl.c_str());
    m_pXmlUtil->AddChildElem(STRING_XML_SU_IM_ExURL, m_ConfigData.strIMExUrl.c_str());
    
    m_pXmlUtil->AddElem(STRING_XML_SU_CSP);
    m_pXmlUtil->AddChildElem(STRING_XML_SU_CSP_Version, m_ConfigData.strCSPVersion.c_str());

    m_pXmlUtil->AddElem(STRING_XML_SU_CSA);
    m_pXmlUtil->AddChildElem(STRING_XML_SU_CSA_Version, m_ConfigData.strCSAVersion.c_str());

    m_pXmlUtil->AddElem(STRING_XML_SU_CSC);
    m_pXmlUtil->AddChildElem(STRING_XML_SU_CSC_Version, m_ConfigData.strCSCVersion.c_str());

    return true;

}

bool CPAXmlSoftUpdate::InitDefaultXmlStruct()
{
    m_ConfigData.strUpdateTime     = STRING_VALUE_SU_Update_Timer;
    m_ConfigData.strPAVersion      = STRING_VALUE_SU_PA_Version;
    m_ConfigData.strMMVersion      = STRING_VALUE_SU_MM_Version;
    m_ConfigData.strMMEduUrl       = STRING_VALUE_SU_MM_EduURL;
    m_ConfigData.strMMExUrl        = STRING_VALUE_SU_MM_ExURL;
    m_ConfigData.strIMVersion      = STRING_VALUE_SU_IM_Version;
    m_ConfigData.strIMEduUrl       = STRING_VALUE_SU_IM_EduURL;
    m_ConfigData.strIMExUrl        = STRING_VALUE_SU_IM_ExURL;
    m_ConfigData.strCSPVersion     = STRING_VALUE_SU_CSP_Version;
    m_ConfigData.strCSAVersion     = STRING_VALUE_SU_CSA_Version;
    m_ConfigData.strCSCVersion     = STRING_VALUE_SU_CSC_Version;
    return true;

}

bool CPAXmlSoftUpdate::DoMapXmlDataToMem()
{
    m_pXmlUtil->ResetPos();

    DealBoolRet( m_pXmlUtil->FindElem() ); 
    m_pXmlUtil->IntoElem(); //进入root


    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_SU_Update) ); 
    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_SU_Update_Timer) ); 
    m_ConfigData.strUpdateTime = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_SU_PA) ); 
    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_SU_PA_Version) ); 
    m_ConfigData.strPAVersion = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_SU_MM) );  

    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_SU_MM_Version) ); 
    m_ConfigData.strMMVersion = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_SU_MM_EduURL) ); 
    m_ConfigData.strMMEduUrl = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_SU_MM_ExURL) ); 
    m_ConfigData.strMMExUrl = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_SU_IM) ); 
    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_SU_IM_Version) ); 
    m_ConfigData.strIMVersion = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_SU_IM_EduURL) ); 
    m_ConfigData.strIMEduUrl = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_SU_IM_ExURL) ); 
    m_ConfigData.strIMExUrl = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_SU_CSP) ); 
    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_SU_CSP_Version) ); 
    m_ConfigData.strCSPVersion = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_SU_CSA) ); 
    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_SU_CSA_Version) ); 
    m_ConfigData.strCSAVersion = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_SU_CSC) ); 
    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_SU_CSC_Version) ); 
    m_ConfigData.strCSCVersion = m_pXmlUtil->GetChildData();
    return true;

}

bool CPAXmlSoftUpdate::ReadWriteData( XML_PA_SOFT_UPDATE& ucData, ENUM_DATA_DUTY_SU da, tstring& strData )
{
    bool bRet = false;
    switch(da)
    {
        Imp_Duty_Cn_Interface(ddsu_update_time,         ReadWriteUpdateTime) 
        Imp_Duty_Cn_Interface(ddsu_pa_version,          ReadWritePAVersion)  
        Imp_Duty_Cn_Interface(ddsu_mm_version,          ReadWriteMMVersion)  
        Imp_Duty_Cn_Interface(ddsu_mm_edu_url,          ReadWriteMMEduUrl)	
        Imp_Duty_Cn_Interface(ddsu_mm_ex_url,           ReadWriteMMExUrl)		
        Imp_Duty_Cn_Interface(ddsu_im_version,          ReadWriteIMVersion)  
        Imp_Duty_Cn_Interface(ddsu_im_edu_url,          ReadWriteIMEduUrl)	
        Imp_Duty_Cn_Interface(ddsu_im_ex_url,           ReadWriteIMExUrl)		
        Imp_Duty_Cn_Interface(ddsu_csp_version,         ReadWriteCSPVersion) 
        Imp_Duty_Cn_Interface(ddsu_csa_version,         ReadWriteCSAVersion) 
        Imp_Duty_Cn_Interface(ddsu_csc_version,         ReadWriteCSCVersion) 
    }
    //把数据覆盖写入到xml
    return true;
}
