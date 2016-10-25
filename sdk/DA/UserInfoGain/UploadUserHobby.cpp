#include "StdAfx.h"
#include "UploadUserHobby.h"
#include "functional.h"

CUploadUserHobby::CUploadUserHobby(void)
{
    m_strHobbyFileDir   = _T("D:\\test\\DA");
}


CUploadUserHobby::~CUploadUserHobby(void)
{
}

bool CUploadUserHobby::Init()
{
    Create_Auto_Ptr(m_pUploadUtil, CUploadUtil);
    return true;
}

bool CUploadUserHobby::UploadUserHobbyData()
{
    if(!IsCanDo())
    {
        return true;
    }
    UploadAllHobbyData();
    return true;
}

bool CUploadUserHobby::IsCanDo()
{
    return true;

}

bool CUploadUserHobby::IsTimeUp()
{
    return true;

}

bool CUploadUserHobby::UploadAllHobbyData()
{
    //枚举指定目录下所有 文件名符合格式的文件
    FileEnumeration(m_strHobbyFileDir.c_str(), false, true, &CUploadUserHobby::DealFileFunc
        , this);
    return true;
    
}

bool CUploadUserHobby::DealFileFunc( tcpchar lpFileOrPath, void* pUserData )
{
    //对每个文件进行上传, 删除操作

    CUploadUserHobby* pHobby = static_cast<CUploadUserHobby*>(pUserData);
    if(pHobby->IsFileHobby(lpFileOrPath))
    {
        pHobby->UploadDataFile(lpFileOrPath);
        pHobby->DeleteFile(lpFileOrPath);
    }
    
    return true;

}

bool CUploadUserHobby::UploadDataFile( tcpchar szFile )
{
    TRACE(_T("UPLOAD--"));
    TRACE(szFile);
    TRACE(_T("\n"));

    //上传功能内网已实现
        //  m_pUploadUtil->Upload(szFile);

    return true;
    
}

bool CUploadUserHobby::IsFileHobby( tcpchar lpFileOrPath )
{
    tstring strFilePath(lpFileOrPath);
    /* TODO: 判断文件是否符合用户习惯文件名的规则*/    
    if(strFilePath.find(_T("2")) != string::npos 
        && strFilePath.find(_T(".clt")) != string::npos )
    {
        return true;
    }
    return false;
    
}

bool CUploadUserHobby::DeleteFile( tcpchar lpFileOrPath )
{
    TRACE(_T("delete--"));
    TRACE(lpFileOrPath);
    TRACE(_T("\n"));
    return true;
    
}

/**************************************** 上传功能组件实现 ****************************************/

bool CUploadUtil::Upload( tcpchar szFile )
{
    //TODO: 获取出文件的时间
    return true;

}
