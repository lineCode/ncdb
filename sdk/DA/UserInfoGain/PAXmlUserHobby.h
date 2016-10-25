#pragma once

#include "PiTypeDef.h"
#include "PAXmlUserHobbyDef.h"
#include "BackEndCollectDef.h"
#include "PiXmlBase.h"
/************************************************************************
    
/************************************************************************/

class CPAXmlUserHobby
    :public CPiXmlBase<XML_USER_HOBBY_DATA, ENUM_DATA_DUTY_UH>
{
public:
    CPAXmlUserHobby(void);
    ~CPAXmlUserHobby(void);
public:
    virtual tstring GetXmlFullPath();
    static CPAXmlUserHobby* GetInstance();
    static void ReleaseInstance() ;
    virtual bool DoFirstCreateDone();

    //bool AddMath( ENUM_DATA_DUTY_UH m_udDuty, int nAdd );

private: //private
    tstring GetCurrentMonthDBName();
private:

    //    virtual bool DoSaveCache();
    //virtual void CreateDataBackup();
    //virtual bool DoReadWriteData(ENUM_DATA_DUTY_UH da, tstring& strData );
    //virtual bool DoReadWriteCache( ENUM_DATA_DUTY_UH da, tstring& strData );

    virtual bool CreateXmlData();
    virtual bool InitDefaultXmlStruct();
    virtual bool DoMapXmlDataToMem();
    virtual bool ReadWriteData(XML_USER_HOBBY_DATA& ucData, ENUM_DATA_DUTY_UH da, tstring& strData );

    //m_ConfigData.daUFConfigInCloud
    IMP_UH_READ_WRITE(ReadWritePsUseCount,              psUseCount)
    IMP_UH_READ_WRITE(ReadWriteSaiUseCount,             saiUseCount)
    IMP_UH_READ_WRITE(ReadWritePAUseCount,              daUseCount)
    IMP_UH_READ_WRITE(ReadWritePAUseTime,               daUseTime)
    IMP_UH_READ_WRITE(ReadWritePAUFMyAssistantCount,    daUFMyAssistantCount)
    IMP_UH_READ_WRITE(ReadWritePAUFWelcom,              daUFWelcom)
    IMP_UH_READ_WRITE(ReadWritePAUFworksprotect,        daUFworksprotect)
    IMP_UH_READ_WRITE(ReadWritePAUFConfigInCloud,       daUFConfigInCloud)
    IMP_UH_READ_WRITE(ReadWritePAUFDriverManage,        daUFDriverManage)
    IMP_UH_READ_WRITE(ReadWritePAUFSoftManage,          daUFSoftManage)
    IMP_UH_READ_WRITE(ReadWritePAUFFontSave,            daUFFontSave)
    IMP_UH_READ_WRITE(ReadWritePAUFVideo,               daUFVideo)
    IMP_UH_READ_WRITE(ReadWritePAUFInspiration,         daUFInspiration)
    
    /*******************************  Ù–‘ **********************************/
private:

    static auto_ptr<CPAXmlUserHobby>                m_pgXml;
};

