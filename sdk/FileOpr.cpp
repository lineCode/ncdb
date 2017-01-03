#include "StdAfx.h"
#include "define_gnl.h"
#include "FileOpr.h"
#include <io.h>
#include <errno.h>
#include <assert.h>
#include <sstream>
#include "StrCoding.h"
#include "Math/PiMathUnit.h"
#include "ResCommon.h"
#include "RAIILock.h"
using namespace std;
Pi_NameSpace_Using

CFileOpr::CFileOpr(void)
{
	m_pFile			= NULL;
	m_pFileData		= NULL;
	m_dwFileSize	= 0;
	Init();
}

CFileOpr::~CFileOpr(void)
{

	/*if (m_pFileData)
	{
		delete m_pFileData;
		m_pFileData = NULL;
	}*/

	Close();
	UnInit();
}

bool CFileOpr::Init()
{
	InitializeCriticalSection(&m_cs);
	return true;
}

void CFileOpr::SetErrDesc( tcpchar szDesc )
{
    m_strErrDesc = szDesc;
}

tstring CFileOpr::GetErrDesc()
{
    return m_strErrDesc;
}

bool CFileOpr::UnInit()
{
	DeleteCriticalSection(&m_cs);
	return true;
}

bool CFileOpr::Open(tcpchar filePath, tcpchar cMode/* = "r"*/, int shflag /*= _SH_DENYWR*/)
{
	//errno_t err = 0;
    FILE* pFileTemp = ::_tfsopen(filePath, cMode, shflag);
	if (0 == pFileTemp)
	{
		int Err = errno ;
        tostringstream oss;
        oss<<INFO_ERROR _T("open file:")<< filePath<<_T(" failed!");
        this->SetErrDesc(oss.str().c_str());
		return false;
		//errno;
		//int value = 0;
		//err = _get_errno(&value);
		//m_pFile = NULL;//是否需要释放内存
	}

	if(0 != fseek(pFileTemp, 0, SEEK_SET))
	{
        this->SetErrDesc(_T("移动文件指针失败"));
		return false;
	}
	if( ::setvbuf(pFileTemp, NULL, _IONBF, 0))
	{
		return false;
	}
    //如果能成功打开再关闭之前的
    if (m_pFile)
    {
        assert(Close());
        //return false;
    }
    m_pFile = pFileTemp;

    m_strFilePath = filePath;
	return true;
}

bool CFileOpr::Open(tstring& strFile, tcpchar cMode /*= _T("rb")*/, int shflag /*= _SH_DENYWR*/)
{
    return this->Open(strFile.c_str(), cMode, shflag);
}

bool CFileOpr::Read( void*& pBuf, UNLONG MaxSize, UNLONG* nReadReal /*= NULL*/ )
{
	//delete m_pFileData;
    UNLONG nMemSize = MaxSize + 1;
	m_pFileData = new char[nMemSize];
	memset(m_pFileData, 0, nMemSize);

    if(!ReadBuf(m_pFileData, MaxSize, nReadReal))
    {
        return false;
    }

	/*size_t dwRead = fread(m_pFileData, 1, MaxSize, m_pFile);
    
	if (dwRead != MaxSize && !feof(m_pFile))
	{
		////AfxMessageBox(L"readedCount = 0");
		return false;
	}
	/ *dwRead = feof(m_pFile);
	m_dwFileSize = dwRead;* /
    if (nReadReal)
    {
        *nReadReal = dwRead;
    }*/
    pBuf = m_pFileData;
	return true;
}

bool CFileOpr::ReadBuf( void* pBuf, UNLONG MaxSize, UNLONG* nReadReal /*= NULL*/ )
{
    //delete m_pFileData;
    UNLONG nMemSize = MaxSize + 1;
	size_t dwRead = fread(pBuf, 1, MaxSize, m_pFile);
    
	if (dwRead != MaxSize && !feof(m_pFile))
	{
		return false;
	}
	/*dwRead = feof(m_pFile);
	m_dwFileSize = dwRead;*/
    if (nReadReal)
    {
        *nReadReal = dwRead;
    }
	return true;
}

bool CFileOpr::Write( const void* data, UNLONG dwSize )
{
    EnterCriticalSection(&m_cs);
    
	size_t ret = fwrite(data, 1, dwSize, m_pFile);
    LeaveCriticalSection(&m_cs);
	if(ret < dwSize)
	{
		return false;
	}
    
	return true;
}

bool CFileOpr::Flush()
{
	if(EOF == ::fflush(m_pFile))
	{
		return false;
	}
	return true;
}


bool CFileOpr::Close()
{
    if (m_pFile)
    {
        if(0 != ::fclose(m_pFile))
        {
            return false;
        }
        m_pFile = NULL;
        //m_pFile = NULL;
    }

	return true;
}


char* CFileOpr::GetFileData()
{
	if (m_pFileData)
	{
		return m_pFileData;
		
	}else
	{	
        void* pBuf = NULL;
        this->Read(pBuf, this->GetFileSize());
		return (char*)pBuf;
	}
}


UNLONG CFileOpr::GetFileSize()
{
	return _filelength(_fileno(m_pFile));

	if (m_dwFileSize)
	{
		return m_dwFileSize;
	}

	fseek(m_pFile, 0, SEEK_SET);
	UNLONG dwSize = 0;
	size_t dwRet = 0;
	char cBuf[2048] = {0};
	while(!feof(m_pFile))
	{
		//dwSize = ftell(m_pFile);
		//dwSize = fseek(m_pFile,1, SEEK_CUR);
		
		dwSize += fread(&cBuf,1, sizeof(cBuf),m_pFile);
	}
	return dwSize;
}

UNLONG CFileOpr::GetFileSize( tcpchar szPath )
{
    UNLONG nSize = 0;
    CFileOpr f;
    if( ! f.Open(szPath) )
    {
        return nSize;
    }
    return f.GetFileSize();
}

tstring CFileOpr::GetFilePath()
{
    return m_strFilePath;
}

bool CFileOpr::RemoveData( tcpchar szFile, DWORD nEraseSize, int nStartPos )
{
    if (nEraseSize <= 0)
    {
        return true;
    }
    tstring strPath(szFile);
    //
    //分块读取原来文件, 写入到新文件, 
    //跳过要擦除的文件数据区域, 写入到新文件后, 替换掉原来文件
    // 临时文件标识FILE_ATTRIBUTE_TEMPORARY, FILE_FLAG_DELETE_ON_CLOSE

    //设置指针到合适位置, 
    //对删除区域左右两边数据读取写入到新文件
    UNLONG nLeftEnd = nStartPos;
    UNLONG nRightStart = nStartPos + nEraseSize;
    
    const int nReadMaxMemSize = NUM_FILE_SIZE_1M * 70;
    CFileOpr file;
    if(!file.Open(strPath))
    {
        return false;
    }

    CFileOpr fileTemp;
    if( !fileTemp.Open(strPath + _T("_"), _T("wb")))
    {
        return false;
    }

    UNLONG nSizeSrc = file.GetFileSize();
    UNLONG nRead = 0;
    //void* pBuf = NULL;
    NewMemAndInit(pBuf, nReadMaxMemSize);
    CRAIIMemArr raii(pBuf);

    UNLONG nReadTotal = 0;



    int nReadSize = min(nLeftEnd - nReadTotal, nReadMaxMemSize);
    file.SetPos(0);
    while(nReadTotal < nLeftEnd && file.ReadBuf(pBuf, nReadSize, &nRead))
    {
        nReadTotal += nRead;
        fileTemp.Write(pBuf, nRead);
        nReadSize = min(nLeftEnd - nReadTotal, nReadMaxMemSize);
    }

    if(nLeftEnd != fileTemp.GetFileSize())  //判断左边区域是否全部写入到新文件
    {
        return false;
    }

    nReadTotal = 0;
    UNLONG nBlackSize = nSizeSrc - nRightStart;
    nReadSize = min(nBlackSize - nReadTotal, nReadMaxMemSize);
    file.SetPos(nRightStart);
    while(nReadTotal < nBlackSize && file.ReadBuf(pBuf, nReadSize, &nRead))
    {
        nReadTotal += nRead;
        fileTemp.Write(pBuf, nRead);
        nReadSize = min(nBlackSize - nReadTotal, nReadMaxMemSize);
    }

    fileTemp.Flush();
    if(nSizeSrc - nEraseSize != fileTemp.GetFileSize())  //判断两边总大小是否全部写入到新文件
    {
        return false;
    }

    
    return file.DeleteFile() && fileTemp.Rename(file.GetFilePath().c_str());
}

bool CFileOpr::RemoveData( PCTSTR szFile, DWORD nEraseSize, ENUM_FILE_POS nPos )
{
    if (pos_begin == nPos)
    {
        return CFileOpr::RemoveData(szFile, nEraseSize, 0);
    }
    else
    {
        assert(0 && "no suppose pos");
    }
    return true;
}

bool CFileOpr::SetPos( UNLONG nRightStart, int nOrigin /*= SEEK_SET*/)
{
    return fseek(m_pFile, nRightStart, nOrigin) == 0;
}

bool CFileOpr::DeleteFile()
{
    Close();
    return DeleteFile(m_strFilePath.c_str());
}

bool CFileOpr::DeleteFile( tcpchar szPath )
{
    return _tremove(szPath) == 0;

}

bool CFileOpr::Rename( tcpchar szNewName )
{
    Close();
    return Rename(m_strFilePath.c_str(), szNewName);
}

bool CFileOpr::Rename( tcpchar szOldName, tcpchar szNewName )
{
    return _trename(szOldName, szNewName) == 0;
    
}



bool CFileOpr::IsModuleBit64( tcpchar szPath )
{
    {
        /*IMAGE_DOS_HEADER idh;
        FILE *f = fopen(strPath.c_str(), "rb");
        fread(&idh, sizeof(idh), 1, f);

        IMAGE_FILE_HEADER ifh;
        fseek(f, idh.e_lfanew + 4, SEEK_SET);
        fread(&ifh, sizeof(ifh), 1, f);

        fclose(f);*/
    }

    bool bIs64 = false;
    CFileOpr file;
    DealBoolRet( file.Open(szPath) );

    IMAGE_DOS_HEADER IDH ={0};
    IMAGE_FILE_HEADER ifh = {0};
    const int nSignatureLen = 4;
    DealBoolRet( file.ReadBuf(&IDH, sizeof(IMAGE_DOS_HEADER)));
    DealBoolRet( file.SetPos(IDH.e_lfanew + nSignatureLen) );
    DealBoolRet( file.ReadBuf(&ifh, sizeof(IMAGE_FILE_HEADER)) );
    bIs64 = ( ifh.Machine == IMAGE_FILE_MACHINE_AMD64
            || ifh.Machine == IMAGE_FILE_MACHINE_IA64 );
    return bIs64;
}

bool CFileOpr::Save( void* pData, int nSize, tcpchar szFile )
{
	if (!pData || !nSize
		|| !szFile)
	{
		return false;
	}
	CFileOpr file;
	if(!file.OpenWrite(szFile))
	{
		return false;
	}
	return file.Write(pData, nSize);
}

bool CFileOpr::OpenWrite( tcpchar szFile )
{
	return Open(szFile, _T("wb"), _SH_DENYWR);
}

bool CFileOpr::IsOpen()
{
	return m_pFile != NULL;
}
