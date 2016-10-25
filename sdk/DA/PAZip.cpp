#include "StdAfx.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "PAZip.h"
#include "PiZip.h"
#include "functional.h"
#include "SysFilePathMgr.h"
#include "UCommon.h"
#include "PAProdictPathMng.h"
#include "PADebug.h"
#include "PathLight.h"

CPAZip::CPAZip(void)
{
}


CPAZip::~CPAZip(void)
{
}

bool CPAZip::Init()
{
    m_pZip      = auto_ptr<CPiZip>(new CPiZip);
    tstring strPathDist;
    //初始化计算出所有路径

    strPathDist = CPAProdictPathMng::GetPathMMWorkPlacement();
    AddDist(pt_mm_panel, strPathDist);

    //strPathDist = strUserDocumentPath + STRING_DIR_SUFFIX_MM_COLOR;
    strPathDist = CPAProdictPathMng::GetPathMMColor();
    AddDist(pt_mm_color, strPathDist);

    //strPathDist = strUserDocumentPath + STRING_DIR_SUFFIX_MM_BRUSH;
    strPathDist = CPAProdictPathMng::GetPathMMCtBrush();
    AddDist(pt_mm_brush, strPathDist);
    
    //strPathDist = strUserDataPath + STRING_DIR_SUFFIX_IM_PANEL;
    strPathDist = CPAProdictPathMng::GetPathIMWorkPlacement();
    AddDist(pt_im_panel, strPathDist);

    //strPathDist = strUserDocumentPath + STRING_DIR_SUFFIX_IM_COLOR;
    strPathDist = CPAProdictPathMng::GetPathIMColor();
    AddDist(pt_im_color, strPathDist);

    //strPathDist = strUserDocumentPath + STRING_DIR_SUFFIX_IM_BRUSH;
    strPathDist = CPAProdictPathMng::GetPathIMCtBrush();
    AddDist(pt_im_brush, strPathDist);

    return true;
}

bool CPAZip::PackConfig( PCTSTR szZipPath, ENUM_PACK_TYPE packType )
{
    //获取路径
    switch (packType)
    {
    case type_mm:
        {
            m_pZip->AddZipFolder(m_lstPath.find(pt_mm_panel)->second.strPathSoft.c_str());
            m_pZip->AddZipFolder(m_lstPath.find(pt_mm_color)->second.strPathSoft.c_str());
            m_pZip->AddZipFolder(m_lstPath.find(pt_mm_brush)->second.strPathSoft.c_str());
        }
        break;
    case type_im:
        {
             m_pZip->AddZipFolder(m_lstPath.find(pt_im_panel)->second.strPathSoft.c_str());
             m_pZip->AddZipFolder(m_lstPath.find(pt_im_color)->second.strPathSoft.c_str());
             m_pZip->AddZipFolder(m_lstPath.find(pt_im_brush)->second.strPathSoft.c_str());
        }
        break;
    }
    
    return m_pZip->CompressFolder(szZipPath);
    

}

bool CPAZip::UnPackonfig( PCTSTR szZipPath, ENUM_PACK_TYPE packType )
{
    //解压到某个临时目录, 会增加个跟压缩包文件名相同的目录
    
    tstring strPathTemp(GetDirFromPath(szZipPath).c_str());
    
    if(!m_pZip->UnZip(szZipPath, strPathTemp.c_str()))
    {
        return false;
    }

    strPathTemp += '\\';
    strPathTemp += CPathLight::GetFileTitleFromPath(szZipPath);
    strPathTemp += '\\';

    //查找目录, 把目录剪切到对应的目录(先删除目标目录)
    switch(packType)
    {
    case type_mm:
        {
            CutToMM(strPathTemp);
        }
        break;
    case type_im:
        {
            CutToIM(strPathTemp);
        }
        break;

    }

    /***************** 删除压缩包释放出来的目录 *****************/
    CFileUtil::DeleteFile(CString(strPathTemp.c_str()));
    return true;
    
}

bool CPAZip::CutToMM( const tstring& strPathSour )
{
    /***************** 找到3个配置文件夹, 进行覆盖剪切操作 *****************/

    if(CutFileCoverFull(strPathSour + m_lstPath[pt_mm_panel].strFolder, m_lstPath[pt_mm_panel].strPathSoft)
        && CutFileCoverFull(strPathSour + m_lstPath[pt_mm_color].strFolder, m_lstPath[pt_mm_color].strPathSoft)
        && CutFileCoverFull(strPathSour + m_lstPath[pt_mm_brush].strFolder, m_lstPath[pt_mm_brush].strPathSoft)
    )
    {
        return true;
    }
    
    return false;
    
}

bool CPAZip::CutToIM( tstring strPathSour )
{
    /***************** 找到3个配置文件夹, 进行覆盖剪切操作 *****************/
    if (
        CutFileCoverFull(strPathSour + m_lstPath[pt_im_panel].strFolder, m_lstPath[pt_im_panel].strPathSoft)
        && CutFileCoverFull(strPathSour + m_lstPath[pt_im_color].strFolder, m_lstPath[pt_im_color].strPathSoft)
        && CutFileCoverFull(strPathSour + m_lstPath[pt_im_brush].strFolder, m_lstPath[pt_im_brush].strPathSoft)
        )
    {
        return true;
    }

    return false;
    
}

void CPAZip::AddDist( ENUM_PATH_TYPE nType , tstring strDistPath )
{
    //根据 type, 获取到前缀, 中间字符串, 最后目录

    m_lstPath.insert(pair<ENUM_PATH_TYPE, PATH_INFO>(nType, 
        PATH_INFO( strDistPath, GetFileNameFromPath(strDistPath.c_str()) ))
        );
}

bool CPAZip::CutFileCoverFull( tstring szSour, tstring szDist )
{
    if (!::PathFileExists(szSour.c_str()))
    {
        return false;
    }
    
    
    /***************** 删除最终目录 *****************/
    CFileUtil::DeleteFile(CString(szDist.c_str()));


    /***************** 目录移动到指定目录 *****************/
    //如果是目录, 重指向上层目录
//     bool bDir = true;
//     if(bDir)
//     {
//         szDist = GetParentPathFromPath(szDist.c_str());
//     }
    //最终目录不存在, 会自动创建
    if(!CFileUtil::CopyFile(CString(szSour.c_str()), CString(szDist.c_str())))
    {
        return false;
    }

    /***************** 删除源目录 *****************/
    CFileUtil::DeleteFile(CString(szSour.c_str()));
    
    return true;
    
}
