#pragma once
#include "PASettingDef.h"
#include "PiTypeDef.h"
class CPAXmlUserConfig;
/************************************************************************
    每个设置选项的 数据部分 的基类
/************************************************************************/
class CDataSettingBase
{
public:
    CDataSettingBase(CPAXmlUserConfig* pXml, ENUM_DATA_DUTY dt);
    virtual ~CDataSettingBase(void);
public:
    bool Init();
    void SetDataType(ENUM_DATA_DUTY dn);
    bool SetData(tstring strData);
    bool SetXml(CPAXmlUserConfig* pXml);
    bool Write();
    bool Read();
    tstring GetData();
    ENUM_DATA_DUTY GetDataDuty();
    //virtual bool InitDataType() = 0;
    virtual bool CheckDataValid() = 0;
    bool WriteCache();
private:
    ENUM_DATA_DUTY          m_dataDuty; //数据类型， 供保存到xml时定位位置
    tstring                 m_strData;
    CPAXmlUserConfig*              m_xml;
};


class COptDataEnum 
    : public CDataSettingBase
{
public:
    COptDataEnum(CPAXmlUserConfig*  pXml, ENUM_DATA_DUTY dt);
    virtual ~COptDataEnum(void);
public:
    virtual bool CheckDataValid();
private:
};


class COptDataPath 
    : public CDataSettingBase
{
public:
    COptDataPath(CPAXmlUserConfig*  pXml, ENUM_DATA_DUTY dt);
    virtual ~COptDataPath(void);
public:
    virtual bool CheckDataValid();
private:
};

