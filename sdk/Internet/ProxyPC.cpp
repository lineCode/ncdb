#include "StdAfx.h"
#include "ProxyPC.h"
#include "functional.h"
#include "FileOpr.h"
#include "PiRegex.h"
#include "RAIILock.h"
#include "StrCoding.h"
using namespace ns_pi_regex;

auto_ptr<CProxyPC> CProxyPC::m_pInst;
auto_ptr<CLock>  Create_Auto_Ptr(CProxyPC::m_pLock, CLock);

CProxyPC::CProxyPC(void)
{
}


CProxyPC::~CProxyPC(void)
{
}

bool CProxyPC::InitData()
{
    //从同级目录中获取出保存代理文件的内部数据
    tstring strPath = GetModuleFoloder();
    strPath += _T("\\config\\proxy");
    CFileOpr file;
    file.Open(tstring(strPath).c_str());
    UNLONG nSize = file.GetFileSize();
    void* pFileData = NULL;
    file.Read(nSize, pFileData);
    CPiRegex regex;
    //proxy:  23.34.45.66:8888
    LST_STRING lstString;
    if(!regex.GetIpPort(s2w(string((char*)pFileData)).c_str(), lstString))
    {
        return false;
    }
    SetProxyList(lstString);
    return true;
}

CProxyPC* CProxyPC::GetInst()
{
    CRAIILock raii(m_pLock->Lock());
    if(!m_pInst.get())
    {
        CProxyPC* pInst = new CProxyPC;
        pInst->Init();
        m_pInst.reset(pInst);
    }
    return m_pInst.get();
}

bool CProxyPC::ReleaseInst()
{
    CRAIILock raii(m_pLock->Lock());
    CProxyPC* pTemp = m_pInst.get();
    if(pTemp)
    {
        ClearMem(pTemp);
    }
    return true;
}

