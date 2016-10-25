#pragma once
//#include "DASettingDef.h"
#include "PiTypeDef.h"
#include "Markup.h"
#include "PiXmlDef.h"
#include <time.h>
#include "Lock.h"
#include "define_gnl.h"
#include "ResCommon.h"
#include "functional.h"
/************************************************************************
    整个xml存在期间， 都保持文件打开操作
/************************************************************************/
// class CMarkup;
// class CLock;

using namespace std;
template<class DataStruct, class DataDuty>
class CPiXmlBase
{
public:
    CPiXmlBase(void);
    ~CPiXmlBase(void);
public:
    virtual bool Init();
    /************************************************************************
        fun:    直接写入文件
        param: 
        remark: 
    ************************************************************************/
    bool WriteData(DataDuty da, tstring& strData );
    bool WriteData(DataDuty da, const int& nData );
    bool WriteData(DataDuty da, CString& strData );
    /************************************************************************
        fun:    写到缓存
        param: 
        remark: 
    ************************************************************************/
    bool WriteCache(DataDuty da, tstring& strData );
    bool ReadData(DataDuty da, tstring& strData );
    bool ReadData(DataDuty da, int& nData);
    bool ReadData(DataDuty da, CString& strData );

    bool ReadFromFile(DataDuty da, tstring& strData );
    bool SetXmlPath(tcpchar strPath);

    bool OpenDB();
    bool CloseDB();
    bool CreateXml();
    /************************************************************************
        fun:    把缓存中的数据写入文件, 通常和WriteCache配合使用
        param: 
        remark: 
    ************************************************************************/
    bool SaveCache();
    
    /************************************************************************
        fun: 对某个字段进行数值增加操作, 并写入文件
        param: 
        remark: 
    ************************************************************************/
    bool AddMath( DataDuty dd, int nAdd );

    int OpenMode() const;
    void OpenMode(int val);
    
    bool ReturnDefaultConfigMem();
private:
    bool IsValidConfigXml();
    bool WriteNewDB();
    bool MapXmlDataToMem();
    void SetDataMode( ENUM_DATA_MODE modeR );
    bool _IsCanCreate();
    virtual bool CreateDataBackup();

    //virtual bool DoSaveCache() = 0;
    virtual tstring GetXmlFullPath();
    virtual bool CreateXmlData() = 0;
    virtual bool InitDefaultXmlStruct() = 0;
    virtual bool DoMapXmlDataToMem() = 0;
    virtual bool ReadWriteData(DataStruct& ucData, DataDuty da, tstring& strData ) = 0;
    /************************************************************************
        fun:    文件第一次创建时被调用, 此时内部状态跟OpenDB成功打开后的状态相同
        param: 
        remark: 
    ************************************************************************/
    bool FirstCreateDone();
    virtual bool DoFirstCreateDone();
    //virtual bool DoReadWriteData(DataDuty da, tstring& strData ) = 0;
//    virtual bool DoReadWriteCache( DataDuty da, tstring& strData ) = 0;

    /******************************* 属性 **********************************/
protected:
    auto_ptr<CMarkup>       m_pXmlUtil;
    //CMarkup*                m_pXmlUtil;
    ENUM_DATA_MODE          m_dataMode;

     DataStruct          m_ConfigData;
     DataStruct          m_ConfigDataCache;
private:
    tstring                     m_strXmlPath;
    //CLock*                      m_pLockXmlRW;
    auto_ptr<CLock>             m_pLockXmlRW;
    int m_nOpenMode;
    bool m_bNewCreate;
};

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::ReturnDefaultConfigMem()
{
    InitDefaultXmlStruct();
    return CreateDataBackup();
}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::_IsCanCreate()
{
    return (m_nOpenMode & mode_open_create) > 0;
}


template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::ReadData( DataDuty da, CString& strData )
{
    tstring strTemp;
    bool bRet = ReadData(da, strTemp);
    strData = strTemp.c_str();
    return bRet;

}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::WriteData( DataDuty da, CString& strData )
{
    return WriteData(da, tstring(strData));
}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::WriteData( DataDuty da,const int& nData )
{
    tstring strBuf;
    const int nSize = 50;
    strBuf.resize(nSize);
    _itot_s(nData, &strBuf.at(0), nSize, 10);

    return WriteData(da, strBuf);

}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::ReadData( DataDuty da, int& nData )
{
    tstring strTemp;

    ReadData(da, strTemp);
    nData = _tstol(strTemp.c_str());
    return true;

}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::ReadFromFile( DataDuty da, tstring& strData )
{
    return OpenDB() && ReadData(da, strData);
    
}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::AddMath( DataDuty dd, int nAdd )
{
    tstring strData;
    ReadData(dd, strData);
    //判断是否为数字, 如果不是 则返回false

    //TODO:判断是否为纯数字
    assert(false == IsAllNum(_T("01556e420")));
    assert(true == IsAllNum(_T("2")));
    assert(false == IsAllNum(_T("-2")));
    assert(false == IsAllNum(_T("2+")));
    assert(true == IsAllNum(_T("01556420")));

    if(!IsAllNum(strData.c_str()))
    {
        strData = _T("0");
    }


    int nInt = _ttoi(strData.c_str());
    nInt += nAdd;
    const int nNumLeng = 15;
    strData.resize(nNumLeng);
    _itot_s(nInt, &strData.at(0), nNumLeng, 10);

    return WriteData(dd, strData);
}

template<class DataStruct, class DataDuty>
tstring CPiXmlBase<DataStruct, DataDuty>::GetXmlFullPath()
{
    return m_strXmlPath;
}

template<class DataStruct, class DataDuty> bool CPiXmlBase<DataStruct, DataDuty>::CreateDataBackup()
{
    m_ConfigDataCache = m_ConfigData;
    return true;
}



/********************************** 模板实现 *************************************/

template<class DataStruct, class DataDuty>
CPiXmlBase<DataStruct, DataDuty>::CPiXmlBase(void)
{
    //m_pXmlUtil      = NULL;
    m_dataMode      = mode_read;
    m_nOpenMode     = mode_open_create;
    m_bNewCreate        = false;
//      ::memset(&m_ConfigData, 0, sizeof(DataStruct));
//      ::memset(&m_ConfigDataCache, 0, sizeof(DataStruct));
    
}

template<class DataStruct, class DataDuty>
CPiXmlBase<DataStruct, DataDuty>::~CPiXmlBase(void)
{
    CloseDB();
}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::Init()
{
    Create_Auto_Ptr(m_pXmlUtil, CMarkup);
    
    Create_Auto_Ptr(m_pLockXmlRW, CLock);
    

    InitDefaultXmlStruct();
    CreateDataBackup();

    return true;
}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::WriteData( DataDuty da, tstring& strData )
{
    bool bRet = false;
    m_pLockXmlRW->Lock();

    //定位到xml中具体的节点位置
    SetDataMode(mode_write);
    if(!ReadWriteData(m_ConfigData, da, strData))
    {
        return false;
    }
    bRet = WriteNewDB();

    m_pLockXmlRW->UnLock();
        
    return bRet;

}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::WriteCache( DataDuty da, tstring& strData )
{
    m_pLockXmlRW->Lock();

    //保存到缓存里
    SetDataMode(mode_write);
    if(!ReadWriteData(m_ConfigDataCache, da, strData))
    {
        return false;
    }
    m_pLockXmlRW->UnLock();

    return true;
}


template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::ReadData( DataDuty da, tstring& strData )
{

    m_pLockXmlRW->Lock();

    SetDataMode(mode_read);
    bool bRet = ReadWriteData(m_ConfigData, da, strData);

    m_pLockXmlRW->UnLock();
    OutInfo(strData.c_str());
    /*TRACE(strData.c_str());
    TRACE(_T("\n"));*/
    return bRet;
}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::OpenDB()
{
    tstring strPath = GetXmlFullPath();
    this->SetXmlPath(strPath.c_str()); 
    bool bLoadRet = (IsValidConfigXml() 
        && m_pXmlUtil->Load(strPath.c_str()) 
        && MapXmlDataToMem() 
        );//加载结果

    if( !bLoadRet
        && _IsCanCreate())
    {
        tstring strMsg = m_pXmlUtil->GetResult();
        TRACE(strMsg.c_str());
        return CreateXml() && MapXmlDataToMem() && FirstCreateDone();
    }

    return bLoadRet;
}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::CloseDB()
{
    //m_pXmlUtil->Close();
    return true;
}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::CreateXml()
{
    if(!CreateXmlData())
    {
        return false;
    }
    m_bNewCreate = true;
    return m_pXmlUtil->Save(m_strXmlPath.c_str());
}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::IsValidConfigXml()
{
    //文件是否存在

    //xml节点是否都存在
    return true;
}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::WriteNewDB()
{
    return CreateXml();
}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::MapXmlDataToMem()
{
    return DoMapXmlDataToMem()
        && CreateDataBackup();
}

template<class DataStruct, class DataDuty>
void CPiXmlBase<DataStruct, DataDuty>::SetDataMode( ENUM_DATA_MODE modeR )
{
    m_dataMode = modeR;
}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::SetXmlPath( tcpchar strPath )
{
    m_strXmlPath = strPath;
    return true;
}


template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::SaveCache()
{
    m_ConfigData = m_ConfigDataCache;
    //DoSaveCache();
    return WriteNewDB();
}

/*
bool CXmlBase::InitChild()
{
    throw std::exception("The method or operation is not implemented.");
}*/



template<class DataStruct, class DataDuty>
int CPiXmlBase<DataStruct, DataDuty>::OpenMode() const
{
    return m_nOpenMode;
}

template<class DataStruct, class DataDuty>
void CPiXmlBase<DataStruct, DataDuty>::OpenMode( int val )
{
    m_nOpenMode = val;
}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::FirstCreateDone()
{
    bool bRet = true;
    if (m_bNewCreate)
    {
        bRet = DoFirstCreateDone();
    }    
    return bRet;
}

template<class DataStruct, class DataDuty>
bool CPiXmlBase<DataStruct, DataDuty>::DoFirstCreateDone()
{
    return true;

}
