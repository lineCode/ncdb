#include "StdAfx.h"
#include "FileMng.h"
#include "functional.h"


CFileMng::CFileMng(void)
{
    m_bRecursion    = true;
    m_bFindFile     = true;
}


CFileMng::~CFileMng(void)
{
}

bool CFileMng::Init()
{
    return true;
}

bool CFileMng::Search( tcpchar szPath /*= NULL */ )
{
    if (szPath && *szPath)
    {
        m_strPath = szPath;
    }

    FileEnumeration(m_strPath.c_str(), m_bRecursion, m_bFindFile, CFileMng::EnumFunc, this);
    SearchDone();
    return true;
}

bool CFileMng::EnumFunc( tcpchar lpFileOrPath, void* pUserData )
{
    CFileMng* pMng = reinterpret_cast<CFileMng*>(pUserData);
    if(pMng->_CheckFile(lpFileOrPath))
    {
        pMng->DealValidFile(lpFileOrPath);
    }
    return true;
}

bool CFileMng::_CheckFile( tcpchar lpFileOrPath )
{
    return true;
}

bool CFileMng::DealValidFile( tcpchar szPath )
{
    m_lstPath.push_back(szPath);
    return true;
}

bool CFileMng::SetPath( tcpchar szPath )
{
    m_strPath   = szPath;
    return true;
}

bool CFileMng::AddFileSift( tcpchar szSuffix )
{
    m_lstSuffix.push_back(szSuffix);
    return true;
}

bool CFileMng::SearchDone()
{
    throw std::exception("The method or operation is not implemented.");
}
