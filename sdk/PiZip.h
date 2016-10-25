#pragma once
#include "PiTypeDef.h"
#include <windows.h>
#include "zip.h"
#include <list>
#include <vector>
using namespace std;

typedef bool (*EnumerateFunc) (tcpchar lpFileOrPath, void* pUserData );
typedef list<tstring>       LST_PATH;
typedef vector<tstring>     VT_PATH;

using namespace PAZipNS;
class CPiZip
{
    
public:
    //using namespace ZIP_N;
    CPiZip(void);
    ~CPiZip(void);
public:
    /**************************************** 接口 ****************************************/
    bool AddZipFolder(tcpchar szPath);
    bool CompressFolder(tcpchar szZipPath, tcpchar szDir = NULL);
    bool CompressFolder( tcpchar szZipPath, const LST_PATH& lstDir);
    bool CompressFile(tcpchar szZipPath, VT_PATH& lstFile);
	/*------------------------------------------------------------
			fun:	解压zip文件到指定目录
			ret:	
			param: 1. szZipPath, zip包路径; 2.szDistPath存放的目录 3.szUnZipFile, 要解压的文件名, 包含内部层次结构
			remark:	解压后的文件名以zip内的文件名, 运行过程中会更改当前目录
	------------------------------------------------------------*/
	static bool UnZip(tcpchar szZipPath, tcpchar szDistPath, tcpchar szUnZipFile = NULL);
	
    /************************************************************************
        fun: 解压到指定目录
        ret: strDistRoot, 返回解压后的文件所在目录
    /************************************************************************/
    static bool UnZipPack(tcpchar szZipPath, tcpchar szDistPath, tstring& strDistRoot);
    
    bool RunCompress();
    tstring ZipPath() const { return m_strZipPath; }
    void ZipPath(tstring val) { m_strZipPath = val; }

    tcpchar CurrentZipLayer() const { return m_strCurrentZipLayer.c_str(); }
    void CurrentZipLayer(tcpchar val) { m_strCurrentZipLayer = val; }

protected:
    /**************************************** 方法 ****************************************/
    bool CreateZipFile(tcpchar szZip);
    bool _AddFolderCB(tcpchar szDir);
    bool _AddFile( tcpchar lpFileOrPath );
    void AppendDirLayer( tcpchar lpFileOrPath );
    static bool DealFileFunc(tcpchar lpFileOrPath, void* pUserData );
    static bool DealDirFunc(tcpchar lpFileOrPath, void* pUserData );
    /************************************************************************
        fun:    增加压缩目录到zip
        param: 
        remark: 
    ************************************************************************/
    bool _AddFolder( tcpchar szPath );
    
private:
    /**************************************** 属性 ****************************************/
    HZIP                m_hZip;
    
    tstring                     m_strCurrentZipLayer;
    tstring                     m_strZipPath;  //保存的zip数据文件路径
    
    LST_PATH                    m_lstPath;
};

