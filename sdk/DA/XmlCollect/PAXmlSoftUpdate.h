#pragma once
#include "PiTypeDef.h"
#include "PiXmlBase.h"
#include "PAXmlSoftUpdateDef.h"
class CPAXmlSoftUpdate :
    public CPiXmlBase<XML_PA_SOFT_UPDATE, ENUM_DATA_DUTY_SU>
{
public:
    CPAXmlSoftUpdate(void);
    ~CPAXmlSoftUpdate(void);
public:
    virtual tstring GetXmlFullPath();
private:

    virtual bool CreateXmlData();
    virtual bool InitDefaultXmlStruct();
    virtual bool DoMapXmlDataToMem();
    virtual bool ReadWriteData(XML_PA_SOFT_UPDATE& ucData, ENUM_DATA_DUTY_SU da, tstring& strData );
    //virtual bool ReadWriteData(XML_USER_HOBBY_DATA& ucData, ENUM_DATA_DUTY_UH da, tstring& strData );
private:
        IMP_SU_READ_WRITE(ReadWriteUpdateTime,      strUpdateTime		)
        IMP_SU_READ_WRITE(ReadWritePAVersion,       strPAVersion		)
        IMP_SU_READ_WRITE(ReadWriteMMVersion,       strMMVersion		)
        IMP_SU_READ_WRITE(ReadWriteMMEduUrl,	    strMMEduUrl	        )
        IMP_SU_READ_WRITE(ReadWriteMMExUrl,		    strMMExUrl          )
        IMP_SU_READ_WRITE(ReadWriteIMVersion,       strIMVersion		)
        IMP_SU_READ_WRITE(ReadWriteIMEduUrl,	    strIMEduUrl	        )
        IMP_SU_READ_WRITE(ReadWriteIMExUrl,		    strIMExUrl          )
        IMP_SU_READ_WRITE(ReadWriteCSPVersion,      strCSPVersion		)
        IMP_SU_READ_WRITE(ReadWriteCSAVersion,      strCSAVersion		)
        IMP_SU_READ_WRITE(ReadWriteCSCVersion,      strCSCVersion		)
};

