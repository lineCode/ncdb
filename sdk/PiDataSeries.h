#pragma once
#include "PiTypeDef.h"
#include "ResCommon.h"

class CFileOpr;
class CDataSeries
{
public:
    CDataSeries(void);
    ~CDataSeries(void);
public:
    bool Open(tcpchar szDBPath);
    bool OpenRead(tcpchar szDBPath);
    bool Init();
    bool UnInit();
    CDataSeries& operator <<(tstring& strData);
    CDataSeries& operator >>(tstring& strData);

    CDataSeries& operator <<(UNINT& strData);
    CDataSeries& operator >>(UNINT& strData);

    CDataSeries& operator <<(int& strData);
    CDataSeries& operator >>(int& strData);
    auto_ptr<CFileOpr>       m_pFile;
};

