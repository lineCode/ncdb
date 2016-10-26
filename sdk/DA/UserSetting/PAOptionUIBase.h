#pragma once
#include "DataSettingBase.h"

class CPAUserSetting;

/************************************************************************
    fun:    UI设置选项的基类
    remark: 负责处理跟界面的交互, 管理该组里的多个界面元素
************************************************************************/
class CPAOptUIBase
{
public:
    CPAOptUIBase(CPAUserSetting* pSC);
    virtual ~CPAOptUIBase(void);
public:

    /************************************************************************
        fun: 从界面获取数据, 保存设置并写入文件
        param: 
        remark:  
    ************************************************************************/
    bool SaveSetting();

    /************************************************************************
        fun: 从界面获取数据, 保存设置,
        param: 
        remark:  不写入文件
    ************************************************************************/
    bool SetConfig();

    virtual bool ReadData();
    tstring GetData();

     /************************************************************************
        fun: 显示文件里的数据到设置选项上
        parent: 
        remark: 
    ************************************************************************/
    bool ShowDB();

    bool InitData();
    virtual bool Show() = 0;
     /************************************************************************
        fun:    根据界面选项, 获取相应的字符串
        param: strData, 传出的字符串, 要保存的选项数据
        remark: 
    ************************************************************************/
    virtual bool DoGetSaevData(tstring& strData) = 0;
    

    bool SetSetting(CDataSettingBase* pSetting);
    bool SetSetting(ENUM_DATA_DUTY dd);
    
    CDataSettingBase* GetSetting(ENUM_DATA_DUTY dDuty);
protected:
    
    bool SaveData(const tstring& strData);
    bool SetData(const tstring& strData);
    ENUM_DATA_DUTY          m_dataDuty;
    void SetDataDuty(ENUM_DATA_DUTY dd);
private:
    tstring                 m_strData;
    CDataSettingBase*       m_SetData;
    CPAUserSetting*         m_pSettingCol;  //引用
};


