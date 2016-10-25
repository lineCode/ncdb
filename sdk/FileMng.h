#pragma once
#include "piTypedef.h"
#include <memory>

typedef list<tstring>  LST_STRING;

class CFileMng
{
public:
    CFileMng(void);
    virtual ~CFileMng(void);
public:
    bool Init();
    bool SetPath(tcpchar szPath);
    bool AddFileSift(tcpchar szSuffix);
    bool Search(tcpchar szPath = NULL );
    static bool EnumFunc (tcpchar lpFileOrPath, void* pUserData );
    virtual bool _CheckFile( tcpchar lpFileOrPath );
    virtual bool DealValidFile(tcpchar szPath);
    virtual bool SearchDone();
protected:
    LST_STRING      m_lstSuffix;

private:
    tstring         m_strPath;
    LST_STRING      m_lstPath;

    bool            m_bRecursion;
    bool            m_bFindFile;
};

