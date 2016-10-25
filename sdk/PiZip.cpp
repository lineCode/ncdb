#include "StdAfx.h"
//#include <windows.h>
#include "PiZip.h"



//#include "zip.h"
#include "unzip.h"
#include "functional.h"
#include "define_gnl.h"
#include <shlobj.h>
#include "PathLight.h"
//#include "zipdef.h"

//using namespace PAZipNS;

//#pragma comment(lib, shell32.lib)
#define FOR_EACH_PATH(Cn, It) \
    FOR_EACH_ITER(LST_PATH, Cn, It)

#define IsFolder(nAttr) ((nAttr) & FILE_ATTRIBUTE_DIRECTORY)
#define ZipOK(nRet) ((nRet) == ZR_OK)
CPiZip::CPiZip(void)
{
    m_hZip       = NULL;
}


CPiZip::~CPiZip(void)
{
}

bool CPiZip::CompressFolder( tcpchar szZipPath, tcpchar szDir /*= NULL*/)
{
//     ZipAdd(hz,"znsimple.txt", "c:\\simple.txt");
    //ZipAddFolder(m_hZipCurrent, _T("test\\1_5"));      //参数2, 会在压缩包内部创建 指定的路径
    //ZipAdd(m_hZipCurrent,"test\\znsimple.bmp", "D:\\test\\1_5\\ch1-05.l"); //参数2, 在压缩包内的路径和文件名
    //CloseZip(m_hZipCurrent);
    //tcpchar szDir = _T("d:\\sdk");
    ZipPath(szZipPath);

    //压缩指定目录 或者 使用多个目录
    if (szDir)
    {
        m_lstPath.clear();
        m_lstPath.push_back(szDir);
    }
    if (m_lstPath.size() <= 0)
    {
        return false;
    }

    return RunCompress();
}

bool CPiZip::CompressFolder( tcpchar szZipPath, const LST_PATH& lstDir )
{
    m_lstPath = lstDir;
    ZipPath(szZipPath);
    return RunCompress();

    //return true;

}

bool CPiZip::DealFileFunc( tcpchar lpFileOrPath, void* pUserData )
{
    CPiZip* pZip = static_cast<CPiZip*>(pUserData);
    pZip->_AddFile(lpFileOrPath);    //每一个文件添加到当前
    return true;
    
}

bool CPiZip::DealDirFunc( tcpchar lpFileOrPath, void* pUserData )
{
    
    CPiZip* pZip = static_cast<CPiZip*>(pUserData); 

    tstring strPathPrev = pZip->CurrentZipLayer(); //保存之前的目录

    pZip->AppendDirLayer(lpFileOrPath); //zip层追加目录
    pZip->_AddFolderCB(lpFileOrPath);

    pZip->CurrentZipLayer(strPathPrev.c_str()); //还原到原来的zip目录

    return true;
}

bool CPiZip::_AddFolderCB( tcpchar szDir )
{

    FileEnumeration(szDir, false, true, CPiZip::DealFileFunc, this);

    FileEnumeration(szDir, false, false, CPiZip::DealDirFunc, this); //递归子目录
    return true;

}

bool CPiZip::_AddFile( tcpchar lpFileOrPath )
{
    //m_strCurrent;
    //tstring strFileName = GetFileNameFromPath(lpFileOrPath); /* 根据路径获取文件名*/
	tstring strInnerPath = m_strCurrentZipLayer + CPathLight::GetFileNameFromPath(lpFileOrPath).c_str();
    tcpchar szInnerPath = strInnerPath.c_str();/* 根据当前层次 和文件名 算出在zip内的路径*/;
    

    ZipAdd(m_hZip, szInnerPath, lpFileOrPath); //参数2, 在压缩包内的路径和文件名

    return true;
    
}

bool CPiZip::CreateZipFile( tcpchar szZip )
{
    if(!szZip)
    {
        return false;
    }
    CPathLight::CreateMultipleDirectory(CPathLight::GetDirFromPath(szZip).c_str());
    m_hZip = CreateZip(szZip, 0);
    return m_hZip > 0;

}

void CPiZip::AppendDirLayer( tcpchar lpFileOrPath )
{
    m_strCurrentZipLayer += CPathLight::GetFileNameFromPath(lpFileOrPath) + _T("\\");
    ZipAddFolder(m_hZip, m_strCurrentZipLayer.c_str());
}

bool CPiZip::AddZipFolder( tcpchar szPath )
{
    m_lstPath.push_back(szPath);
    return true;
}

bool CPiZip::_AddFolder( tcpchar szPath )
{
    m_strCurrentZipLayer.clear();
    this->AppendDirLayer(szPath); //zip层追加目录
    _AddFolderCB(szPath);
    return true;
}

bool CPiZip::RunCompress()
{
    if(!CreateZipFile(m_strZipPath.c_str()))
    {
        return false;
    }

    FOR_EACH_PATH(m_lstPath, it)
    {
        if(!_AddFolder((*it).c_str()))
        {
            return false;
        }
    }

    return ZR_OK == CloseZip(m_hZip);
    
}

bool CPiZip::UnZip( tcpchar szZipPath, tcpchar szDistPath , tcpchar szUnZipFile /*= NULL*/)
{
    //在指定最终目录上, 创建个以 zip包文件名 为名的目录, 解压后放在该目录
    if(!szZipPath || ! CPathLight::IsFileExist(szZipPath))
    {
        return false;
    }
    tstring strDistDir(szDistPath);
    /*strDistDir += _T("\\");
    strDistDir += CPathLight::GetFileTitleFromPath(szZipPath);
*/

    ::SHCreateDirectoryEx(NULL, strDistDir.c_str(), NULL);

    tchar szTempPath[_MAX_FNAME] = {0};
    ::GetCurrentDirectory(MAX_PATH, szTempPath);

    ::SetCurrentDirectory(strDistDir.c_str());  //先设置后, 在打开zip
    //TODO: 考虑提前返回, 当前路径没有还原的情况

    HZIP hzPack = OpenZip(szZipPath, 0);
    if(!hzPack)
    {
        return false;
    }

	if (szUnZipFile)
	{
		//解压指定文件到指定目录
		int nIndex = 0;
		ZIPENTRY ze = {0};
		if( ! ZipOK(FindZipItem(hzPack, szUnZipFile, false, &nIndex, &ze)))
		{
			return false;
		}
		if(!ZipOK(UnzipItem(hzPack, nIndex, CPathLight::GetFileNameFromPath(ze.name).c_str())))
		{
			return false;
		}
		return true;
	}
	else
	{
		ZIPENTRY ze = {0};
		if(!ZipOK(GetZipItem(hzPack, -1, &ze)))
		{
			return false;
		}
		int nCount = ze.index;
		//找到指定文件夹名, 释放到指定目录

		for (int i = 0; i < nCount; ++i)
		{
			if(!ZipOK(GetZipItem(hzPack, i, &ze)))
			{
				return false;
			}
			if(!ZipOK(UnzipItem(hzPack, i, ze.name)))
			{
				return false;
			}

		}
	}
    CloseZip(hzPack);
    ::SetCurrentDirectory(szTempPath);  //还原程序当前目录
    return true;

}

bool CPiZip::UnZipPack( tcpchar szZipPath, tcpchar szDistPath, tstring& strDistRoot )
{
    if (!UnZip(szZipPath, szDistPath))
    {
        return false;
    }
    CPathLight pathConfig(szDistPath);
    pathConfig.AddSubSession(CPathLight::GetFileTitleFromPath(szZipPath));
    strDistRoot = pathConfig.GetPath();
    return true;
}

bool CPiZip::CompressFile( tcpchar szZipPath, VT_PATH& lstFile )
{
    ZipPath(szZipPath);

    if(!CreateZipFile(szZipPath))
    {
        return false;
    }

    m_strCurrentZipLayer.clear();
    FOR_EACH_ITER(VT_PATH, lstFile, it)
    {
        _AddFile(it->c_str());
    }

    return ZR_OK == CloseZip(m_hZip);

}
