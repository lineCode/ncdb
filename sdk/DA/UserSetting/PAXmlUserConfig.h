#pragma once
#include "PASettingDef.h"
#include "PiTypeDef.h"

#include "PiXmlBase.h"
/************************************************************************
    整个xml对象存在期间， 不会占用xml的读写
/************************************************************************/
class CMarkup;

class CPAXmlUserConfig
    :public CPiXmlBase<UserConfigData, ENUM_DATA_DUTY>
{
public:
    CPAXmlUserConfig(void);
    ~CPAXmlUserConfig(void);
public:

    
private:
    virtual tstring GetXmlFullPath();

    virtual bool CreateXmlData();
    virtual bool InitDefaultXmlStruct();
    virtual bool DoReadWriteData(ENUM_DATA_DUTY da, tstring& strData );
    virtual bool DoReadWriteCache( ENUM_DATA_DUTY da, tstring& strData );
    virtual bool DoMapXmlDataToMem();

    bool ReadWriteData(UserConfigData& ucData, ENUM_DATA_DUTY da, tstring& strData );

    IMP_UC_READ_WRITE(ReadWriteLaunchAsBoot, appAutoLaunch)
    IMP_UC_READ_WRITE(ReadWriteCloseApp, exitType)
    IMP_UC_READ_WRITE(ReadWriteTask, task)
    IMP_UC_READ_WRITE(ReadWriteSoftSaveSetting, softPackageSave)
    IMP_UC_READ_WRITE(ReadWriteSoftSavePath, softPackagePath)
    IMP_UC_READ_WRITE(ReadWriteDriverSaveSetting, dPackageSave)
    IMP_UC_READ_WRITE(ReadWriteDriverSavePath, dPackagePath)
    IMP_UC_READ_WRITE(ReadWriteDesktopRecordPath, videoPATH)
    IMP_UC_READ_WRITE(ReadWriteWorksSavePath, worksPath)
    IMP_UC_READ_WRITE(ReadWriteFontLibSavePath, fontPath)
    IMP_UC_READ_WRITE(ReadWriteUpdateType, updateType)
    IMP_UC_READ_WRITE(ReadWriteFlowAutoOpen, flowAutoOpen)
    IMP_UC_READ_WRITE(ReadWriteFlowTopWnd, flowTopMost)
    
    /******************************* 属性 **********************************/
private:

};

