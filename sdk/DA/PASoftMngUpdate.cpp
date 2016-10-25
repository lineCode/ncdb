#include "StdAfx.h"
#include "PASoftMngUpdate.h"
#include "PiZip.h"
#include "define_gnl.h"
#include "UCommon.h"
#include "PathLight.h"
#include "XmlUtil.h"
#include "ListMng.h"
#include "doString.h"
#include "SysFilePathMgr.h"
#include "functional.h"

#define STRING_UPDATE_FILE_NAME         _T("update.xml")

CPASoftMngUpdate::CPASoftMngUpdate( USOFT nSoftType )
{
    m_nSoftType = nSoftType;
}


CPASoftMngUpdate::~CPASoftMngUpdate(void)
{
}

bool CPASoftMngUpdate::ParseFileUpdatePath( tstring strPathConfig, CLstPath& lstPath )
{
    CXmlUtil xml;
    if(!xml.Open(strPathConfig.c_str()))
    {
        return false;
    }

    tstring strFile;
    tstring strPath;
    //CPathLight path(strPathConfig);
    tstring strSrcDir = CPathLight::GetParentPathFromPath(strPathConfig.c_str());

    while( 
        xml.GetChildData(_T("file"), strFile) 
        && xml.GetChildData(_T("path"), strPath) 
        )
    {
        strPath = ParsePathMap(strPath);/*根据符号 和类型获取对应的路径*/
        //OutInfo(_T("distPath:"), strPath.c_str());
        if ( ! CPathLight::IsValidPath(strPath.c_str()) )
        {
            continue;
        }
        //合成完整路径
        tagStrPair* pPair = new tagStrPair;
        pPair->strSrc = strSrcDir + _T("\\") + strFile;
        pPair->strDist = strPath;
        lstPath.Add(pPair);
    }
    return true;
}

bool CPASoftMngUpdate::UpdateFile( CLstPath& lstPath )
{
    tagStrPair* pPair = NULL;
    while(pPair = lstPath.Next())
    {
        CDirectoryUtil::CreateMultipleDirectory(TO_CSTRING(pPair->strDist));
        if(! CFileUtil::CopyFile(TO_CSTRING(pPair->strSrc), TO_CSTRING(pPair->strDist)))
        {
            return false;
        }
    }
    
    return true;
}

bool CPASoftMngUpdate::UpdateSmall( tstring strSrcZip )
{
    tstring strDistDir;    //解压后的所在父目录
    bool bOk = true;
    //TODO:获取系统临时目录
    FOR_COUNT(1, i)
    {
        if(!CPiZip::UnZip(strSrcZip.c_str(), CSysFilePathMgr::GetSysTempFolder(), strDistDir))
        {
            break;
        }

        CPathLight path(strDistDir );
        path.AddSubSession(STRING_UPDATE_FILE_NAME);

        tstring strConfigPath(path.GetPath());
        CLstPath mp;
        bOk = ParseFileUpdatePath(strConfigPath, mp);
        if(!bOk)
        {
            break;
        }

        bOk = CPASoftMngUpdate::UpdateFile(mp);
    }
    
    CFileUtil::DeleteFile(CString(strDistDir.c_str()));
    return bOk;
}

tstring CPASoftMngUpdate::ParsePathMap( const tstring& strPath )
{
    return m_pathMap.GetMapPath(m_nSoftType, strPath);
}

bool CPASoftMngUpdate::Init()
{
    m_pathMap.Init();
    return true;
}
