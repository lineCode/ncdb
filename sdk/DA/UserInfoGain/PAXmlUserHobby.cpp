#include "StdAfx.h"
#include "PAXmlUserHobby.h"

#include "StdAfx.h"
#include "Markup.h"
#include "ResCommon.h"
#include "XmlPathMgr.h"
//CPAXmlUserHobby* CPAXmlUserHobby::m_pgXml = NULL;
auto_ptr<CPAXmlUserHobby> CPAXmlUserHobby::m_pgXml;

CPAXmlUserHobby::CPAXmlUserHobby(void)
{
    
}


CPAXmlUserHobby::~CPAXmlUserHobby(void)
{
}

bool CPAXmlUserHobby::ReadWriteData( XML_USER_HOBBY_DATA& ucData, ENUM_DATA_DUTY_UH da, tstring& strData )
{
    bool bRet = false;
    switch(da)
    {

            Imp_Duty_Cn_Interface(dnh_ps_use_Count,				ReadWritePsUseCount);
            Imp_Duty_Cn_Interface(dnh_sai_use_count,			ReadWriteSaiUseCount);
            Imp_Duty_Cn_Interface(dnh_da_use_count,				ReadWritePAUseCount);
            Imp_Duty_Cn_Interface(dnh_da_use_time,				ReadWritePAUseTime);
            Imp_Duty_Cn_Interface(dnh_da_uf_my_assistant_count,	ReadWritePAUFMyAssistantCount);
            Imp_Duty_Cn_Interface(dnh_da_uf_welcom,				ReadWritePAUFWelcom);
            Imp_Duty_Cn_Interface(dnh_da_uf_worksprotect,		ReadWritePAUFworksprotect);
            Imp_Duty_Cn_Interface(dnh_da_uf_config_in_cloud,	ReadWritePAUFConfigInCloud);
            Imp_Duty_Cn_Interface(dnh_da_uf_driver_manage,		ReadWritePAUFDriverManage);
            Imp_Duty_Cn_Interface(dnh_da_uf_soft_manage,		ReadWritePAUFSoftManage);
            Imp_Duty_Cn_Interface(dnh_da_uf_font_save,			ReadWritePAUFFontSave);
            Imp_Duty_Cn_Interface(dnh_da_uf_video,				ReadWritePAUFVideo);
            Imp_Duty_Cn_Interface(dnh_da_uf_inspiration,		ReadWritePAUFInspiration);

    }
    //把数据覆盖写入到xml
    return bRet;
}

bool CPAXmlUserHobby::DoMapXmlDataToMem()
{
    m_pXmlUtil->ResetPos();
    DealBoolRet( m_pXmlUtil->FindElem() ); 

    m_pXmlUtil->IntoElem(); //进入root

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_UH_ELEM_PS) ); 
    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_UH_ELEM_PS_USECOUNT) ); 
    m_ConfigData.psUseCount = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_UH_ELEM_SAI) ); 
    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_UH_ELEM_SAI_USECOUNT) ); 
    m_ConfigData.saiUseCount = m_pXmlUtil->GetChildData();

    //绘画助手软件
    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_UH_ELEM_PA) );  
    m_pXmlUtil->IntoElem();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_UH_ELEM_PA_USECOUNT) ); 
    m_ConfigData.daUseCount = m_pXmlUtil->GetData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_UH_ELEM_PA_USETIME) ); 
    m_ConfigData.daUseTime = m_pXmlUtil->GetData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_UH_ELEM_PA_USEFUNC) ); 

    //使用功能
    m_pXmlUtil->IntoElem();
    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_UH_ELEM_PA_UF_MYASSISTANT) ); 
    m_ConfigData.daUFMyAssistantCount = m_pXmlUtil->GetAttrib(STRING_XML_UH_ELEM_PA_UF_MYASSISTANT_COUNT);

    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_UH_ELEM_PA_UF_WELCOM) ); 
    m_ConfigData.daUFWelcom = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_UH_ELEM_PA_UF_WORKSPROTECT) ); 
    m_ConfigData.daUFworksprotect = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_UH_ELEM_PA_UF_ConfigInCloud) ); 
    m_ConfigData.daUFConfigInCloud = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_UH_ELEM_PA_UF_DriverManage) ); 
    m_ConfigData.daUFDriverManage = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_UH_ELEM_PA_UF_SoftManage) ); 
    m_ConfigData.daUFSoftManage = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindChildElem(STRING_XML_UH_ELEM_PA_UF_FontSave) ); 
    m_ConfigData.daUFFontSave = m_pXmlUtil->GetChildData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_UH_ELEM_PA_UF_VIDEO) ); 
    m_ConfigData.daUFVideo = m_pXmlUtil->GetData();

    DealBoolRet( m_pXmlUtil->FindElem(STRING_XML_UH_ELEM_PA_UF_Inspiration) ); 
    m_ConfigData.daUFInspiration = m_pXmlUtil->GetData();
   
    return true;
}

bool CPAXmlUserHobby::CreateXmlData()
{
    m_pXmlUtil->SetDoc(NULL);
    m_pXmlUtil->SetDoc(_T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"));

    //m_pXmlUtil->ResetPos();
    m_pXmlUtil->AddElem(_T("root"));
    m_pXmlUtil->IntoElem();

    m_pXmlUtil->AddElem(STRING_XML_UH_ELEM_PS);
    m_pXmlUtil->AddChildElem(STRING_XML_UH_ELEM_PS_USECOUNT, m_ConfigData.psUseCount);
    
    m_pXmlUtil->AddElem(STRING_XML_UH_ELEM_SAI);
    m_pXmlUtil->AddChildElem(STRING_XML_UH_ELEM_SAI_USECOUNT, m_ConfigData.saiUseCount);

    //绘画助手软件
    m_pXmlUtil->AddElem(STRING_XML_UH_ELEM_PA); 
    m_pXmlUtil->IntoElem();

    m_pXmlUtil->AddElem(STRING_XML_UH_ELEM_PA_USECOUNT, m_ConfigData.daUseCount);
    m_pXmlUtil->AddElem(STRING_XML_UH_ELEM_PA_USETIME, m_ConfigData.daUseTime);
    m_pXmlUtil->AddElem(STRING_XML_UH_ELEM_PA_USEFUNC);

    //使用功能
    m_pXmlUtil->IntoElem();
    m_pXmlUtil->AddElem(STRING_XML_UH_ELEM_PA_UF_MYASSISTANT);
    m_pXmlUtil->SetAttrib(STRING_XML_UH_ELEM_PA_UF_MYASSISTANT_COUNT, m_ConfigData.daUFMyAssistantCount);
    m_pXmlUtil->AddChildElem(STRING_XML_UH_ELEM_PA_UF_WELCOM, m_ConfigData.daUFWelcom);
    m_pXmlUtil->AddChildElem(STRING_XML_UH_ELEM_PA_UF_WORKSPROTECT, m_ConfigData.daUFworksprotect);
    m_pXmlUtil->AddChildElem(STRING_XML_UH_ELEM_PA_UF_ConfigInCloud, m_ConfigData.daUFConfigInCloud);
    m_pXmlUtil->AddChildElem(STRING_XML_UH_ELEM_PA_UF_DriverManage, m_ConfigData.daUFDriverManage);
    m_pXmlUtil->AddChildElem(STRING_XML_UH_ELEM_PA_UF_SoftManage, m_ConfigData.daUFSoftManage);
    m_pXmlUtil->AddChildElem(STRING_XML_UH_ELEM_PA_UF_FontSave, m_ConfigData.daUFFontSave);
    
    m_pXmlUtil->AddElem(STRING_XML_UH_ELEM_PA_UF_VIDEO, m_ConfigData.daUFVideo);
    m_pXmlUtil->AddElem(STRING_XML_UH_ELEM_PA_UF_Inspiration, m_ConfigData.daUFInspiration);


    return true;
}

bool CPAXmlUserHobby::InitDefaultXmlStruct()
{
    m_ConfigData.psUseCount                 = STRING_VALUE_UH_ELEM_PS_USECOUNT;
    m_ConfigData.saiUseCount                = STRING_VALUE_UH_ELEM_SAI_USECOUNT;

    m_ConfigData.daUseCount                 = STRING_VALUE_UH_ELEM_PA_USECOUNT;
    m_ConfigData.daUseTime                  = STRING_VALUE_UH_ELEM_PA_USETIME;

    m_ConfigData.daUFMyAssistantCount		= STRING_VALUE_UH_ELEM_PA_UF_MYASSISTANT_COUNT;
    m_ConfigData.daUFWelcom					= STRING_VALUE_UH_ELEM_PA_UF_WELCOM;            
    m_ConfigData.daUFworksprotect			= STRING_VALUE_UH_ELEM_PA_UF_WORKSPROTECT;      
    m_ConfigData.daUFConfigInCloud			= STRING_VALUE_UH_ELEM_PA_UF_CONFIGINCLOUD;     
    m_ConfigData.daUFDriverManage			= STRING_VALUE_UH_ELEM_PA_UF_DRIVERMANAGE;      
    m_ConfigData.daUFSoftManage				= STRING_VALUE_UH_ELEM_PA_UF_SOFTMANAGE;        
    m_ConfigData.daUFFontSave				= STRING_VALUE_UH_ELEM_PA_UF_FONTSAVE;          

    m_ConfigData.daUFVideo					= STRING_VALUE_UH_ELEM_PA_UF_VEDIO;             
    m_ConfigData.daUFInspiration			= STRING_VALUE_UH_ELEM_PA_UF_INSPIRATION;       
    return true;
}

tstring CPAXmlUserHobby::GetCurrentMonthDBName()
{
    //DA_Collection_201X_XX.clt（月份时间）
    time_t tNow = time(NULL);
    tstring strFileName;
    strFileName.resize(MAX_PATH);
    _tcsftime(&strFileName.at(0), MAX_PATH - 1, _T("%Y_%m"), gmtime(&tNow));
    //strFileName = _T("2013_03");
    return strFileName;
}

tstring CPAXmlUserHobby::GetXmlFullPath()
{//DA_Collection_201X_XX.clt
    tstring strPath;
    tstring strDir;
    CXmlPathMgr xmlPathMng;
    strDir = xmlPathMng.GetUserCollectXmlFolder();

    //tcpchar szPath = _T("D:\\test\\DA\\");
    tcpchar szPath = strDir.c_str();
    //strPath += _T("DA_Collection_");
    strPath.resize(MAX_PATH + 1);
//     strPath += GetCurrentMonthDBName();
//     strPath += _T(".clt");
    _stprintf_s(&strPath.at(0), MAX_PATH, _T("%s\\DA_Collection_%s.clt"), szPath
        , GetCurrentMonthDBName().c_str());
    return strPath;
}

CPAXmlUserHobby* CPAXmlUserHobby::GetInstance()
{
//     if(!m_pgXml)
//     {
//         CPAXmlUserHobby* pRet = new CPAXmlUserHobby;
//         pRet->Init();
//         pRet->OpenDB();
//         m_pgXml = pRet;
//     }
// 
//     return m_pgXml;

      if(!m_pgXml.get())
      {
          CPAXmlUserHobby* pRet = new CPAXmlUserHobby;
          pRet->Init();
          pRet->OpenDB();
          m_pgXml.reset(pRet);
      }
      
      return &*m_pgXml;
     //return NULL;
}

void CPAXmlUserHobby::ReleaseInstance()
{
    m_pgXml.reset();
}

bool CPAXmlUserHobby::DoFirstCreateDone()
{
    CString strData;
    strData = _T("dfdf");
    this->WriteData( dnh_sai_use_count, strData);
    this->ReadData(dnh_da_uf_worksprotect, strData);
    return true;
}
