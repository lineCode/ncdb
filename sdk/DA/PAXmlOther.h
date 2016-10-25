#pragma once
#include "PiTypeDef.h"
#include "PiXmlBase.h"
#include "PAXmlOtherDef.h"
class CPAXmlOther
    :public CPiXmlBase<XML_PA_OTHER, ENUM_DATA_DUTY_OTHER>    
{
public:
    CPAXmlOther(void);
    ~CPAXmlOther(void);
public:
    virtual tstring GetXmlFullPath();
private:

    virtual bool CreateXmlData();
    virtual bool InitDefaultXmlStruct();
    virtual bool DoMapXmlDataToMem();
    virtual bool ReadWriteData(XML_PA_OTHER& ucData, ENUM_DATA_DUTY_OTHER da, tstring& strData);
    //virtual bool ReadWriteData(XML_USER_HOBBY_DATA& ucData, ENUM_DATA_DUTY_UH da, tstring& strData );
private:
    IMP_OTHER_READ_WRITE(ReadWriteSaveName,                 strSaveName		)
    IMP_OTHER_READ_WRITE(ReadWriteVideoFormat,              strVideoFormat		)
    IMP_OTHER_READ_WRITE(ReadWriteCaptureRate,              strCaptureRate		)
    IMP_OTHER_READ_WRITE(ReadWriteCaptureVoice,             strCaptureVoice       )
    IMP_OTHER_READ_WRITE(ReadWriteCaptureMouse,             strCaptureMouse       )
    IMP_OTHER_READ_WRITE(ReadWriteMin,                      strMin		)
    IMP_OTHER_READ_WRITE(ReadWriteStartCapture,             strStartCapture       )
    IMP_OTHER_READ_WRITE(ReadWritePauseCapture,             strPauseCapture       )
    IMP_OTHER_READ_WRITE(ReadWriteStopCapture,              strStopCapture		)
    IMP_OTHER_READ_WRITE(ReadWriteAutoLogin,                strAutoLogin		)
    IMP_OTHER_READ_WRITE(ReadWriteToken,                    strToken		)
    IMP_OTHER_READ_WRITE(ReadWriteNerverNotify,             strNerverNotify		)
    IMP_OTHER_READ_WRITE(ReadWriteNerverGUID,                   strGUID		)
};

