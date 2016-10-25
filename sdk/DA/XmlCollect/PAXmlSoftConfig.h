#pragma once
#include "PiTypeDef.h"
#include "PiXmlBase.h"
#include "PAXmlSoftConfigDef.h"

class CPAXmlSoftConfig
    :public CPiXmlBase<XML_PA_SOFT_CONFIG, ENUM_DATA_DUTY_SC>
{
public:
    CPAXmlSoftConfig(void);
    ~CPAXmlSoftConfig(void);
public:
    virtual tstring GetXmlFullPath();
private:
    
    IMP_SC_READ_WRITE(ReadWriteCPUWarnPercent,      strCPUWarnPercent)
    IMP_SC_READ_WRITE(ReadWriteCPUNormalBkColor,    strCPUNormalBkColor)
    IMP_SC_READ_WRITE(ReadWriteCPUWarnBkColor,      strCPUWarnBkColor)
    IMP_SC_READ_WRITE(ReadWriteMemoryWarnPercent,   strMemoryWarnPercent)
    IMP_SC_READ_WRITE(ReadWriteMemoryNormalBkColor, strMemoryNormalBkColor)
    IMP_SC_READ_WRITE(ReadWriteMemoryWarnBkColor,   strMemoryWarnBkColor)
    IMP_SC_READ_WRITE(ReadWriteCPUTimeSpan      ,   strCPUTimeSpan)
    IMP_SC_READ_WRITE(ReadWriteMemTimeSpan      ,   strMemTimeSpan)
    IMP_SC_READ_WRITE(ReadWriteUpdaterPipename      ,   strPipeName)
    IMP_SC_READ_WRITE(ReadWriteUpdaterWndtitle      ,   strWndTitle)

    IMP_SC_READ_WRITE(ReadWriteURL_Lif		          ,   strURL_Lif		    )
    IMP_SC_READ_WRITE(ReadWriteURL_Ad			      ,   strURL_Ad			    )
    IMP_SC_READ_WRITE(ReadWriteURL_HwInfo		      ,   strURL_HwInfo		    )
    IMP_SC_READ_WRITE(ReadWriteURL_SoftwareList	      ,   strURL_SoftwareList	)
    IMP_SC_READ_WRITE(ReadWriteURL_DriverList	      ,   strURL_DriverList	    )
    IMP_SC_READ_WRITE(ReadWriteURL_CloudList	      ,   strURL_CloudList	    )
    IMP_SC_READ_WRITE(ReadWriteURL_CloudItem	      ,   strURL_CloudItem	    )
    IMP_SC_READ_WRITE(ReadWriteURL_Login		      ,   strURL_Login		    )
    IMP_SC_READ_WRITE(ReadWriteURL_Logout		      ,   strURL_Logout		    )
    IMP_SC_READ_WRITE(ReadWriteURL_UserDetail	      ,   strURL_UserDetail	    )
    IMP_SC_READ_WRITE(ReadWriteURL_UserHabit	      ,   strURL_UserHabit	    )
    IMP_SC_READ_WRITE(ReadWriteURL_PaUpdate		      ,   strURL_PaUpdate		)
    IMP_SC_READ_WRITE(ReadWriteURL_ShowPromt   		  ,   strShowPromt   		)
    IMP_SC_READ_WRITE(ReadWriteURL_NerverNotify		  ,   strNerverNotify		)

    virtual bool CreateXmlData();
    virtual bool InitDefaultXmlStruct();
    virtual bool DoMapXmlDataToMem();
    virtual bool ReadWriteData(XML_PA_SOFT_CONFIG& ucData, ENUM_DATA_DUTY_SC da, tstring& strData);
};

