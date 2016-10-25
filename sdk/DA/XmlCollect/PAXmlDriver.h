#pragma once

#include "PiTypeDef.h"
#include "PiXmlBase.h"
#include "PAxmlDriverDef.h"
/*

class CPAXmlDriver :
    public CPiXmlBase<XML_PA_DRIVER, ENUM_DATA_DUTY_DRIVER>
{
public:
    CPAXmlDriver(void);
    ~CPAXmlDriver(void);
public:
    virtual tstring GetXmlFullPath();
private:

    / *IMP_SC_READ_WRITE(ReadWriteCPUWarnPercent,      strCPUWarnPercent)
        IMP_SC_READ_WRITE(ReadWriteCPUNormalBkColor,    strCPUNormalBkColor)
        IMP_SC_READ_WRITE(ReadWriteCPUWarnBkColor,      strCPUWarnBkColor)
        IMP_SC_READ_WRITE(ReadWriteMemoryWarnPercent,   strMemoryWarnPercent)
        IMP_SC_READ_WRITE(ReadWriteMemoryNormalBkColor, strMemoryNormalBkColor)
        IMP_SC_READ_WRITE(ReadWriteMemoryWarnBkColor,   strMemoryWarnBkColor)
        IMP_SC_READ_WRITE(ReadWriteUpdaterUrl           ,   strUrl     )
        IMP_SC_READ_WRITE(ReadWriteUpdaterPipename      ,   strPipeName)
        IMP_SC_READ_WRITE(ReadWriteUpdaterWndtitle      ,   strWndTitle)
* /

    virtual bool CreateXmlData();
    virtual bool InitDefaultXmlStruct();
    virtual bool DoMapXmlDataToMem();
    virtual bool ReadWriteData(XML_PA_SOFT_CONFIG& ucData, ENUM_DATA_DUTY_SC da, tstring& strData);
};*/

