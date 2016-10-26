#include "StdAfx.h"
#include <tchar.h>
#include <Winerror.h>
//#include <afxinet.h>
#include <winsock2.h> 
#include "HardwareQuery.h"

#include "Math/mathUnit.h"
#include "functional.h"
#include "StrCoding.h"
#include "FileOpr.h"
#include "define_gnl.h"
#include <Nb30.h>

#pragma comment(lib,"ws2_32.lib") 
#pragma comment(lib,"Netapi32.lib") //FOR Netbios Function
Pi_NameSpace_Using

CHardwareQuery::CHardwareQuery(void)
{
    ::memset(&m_si, 0, sizeof(SYSTEM_INFO));
    ::memset(&m_mse, 0, sizeof(MEMORYSTATUSEX));
}


CHardwareQuery::~CHardwareQuery(void)
{
}

UNINT CHardwareQuery::GetCpuThreadCountTotal()
{
    return m_si.dwNumberOfProcessors;
}

bool CHardwareQuery::QueryInfo()
{
    enum {err = 0};
    UNINT dwCpuThreadCount = 0;

    ::GetSystemInfo(&m_si);
    dwCpuThreadCount = m_si.dwNumberOfProcessors;
    return true;
}

bool CHardwareQuery::QueryMemoryState()
{
    ::memset(&m_mse, 0, sizeof(MEMORYSTATUSEX));
    m_mse.dwLength = sizeof(MEMORYSTATUSEX);

    
    return GlobalMemoryStatusEx(&m_mse) == TRUE;
}

DWORDLONG CHardwareQuery::GetMemoryPhysicalTotal()
{
    QueryMemoryState();
    return m_mse.ullTotalPhys;
}

DWORDLONG CHardwareQuery::GetMemoryPhysicalAvailable()
{
    QueryMemoryState();
    return m_mse.ullAvailPhys;
}

En_SYSTEM_VERSION CHardwareQuery::GetSystemVersionLocal()
{
    enum {err = ver_unKnow};
    OSVERSIONINFOEX ovie = {0};
    ovie.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    ::GetVersionEx((OSVERSIONINFO*)&ovie);

    //ovie.dwMajorVersion
    tstring strVersionNum(ToVersionString(ovie.dwMajorVersion, ovie.dwMinorVersion));
    //string strVersionNum;
    LST_VERSION_TABLE::iterator itFind = m_lstTableVersion.find(strVersionNum);
    if (itFind == m_lstTableVersion.end())
    {
        return (En_SYSTEM_VERSION)err;
    }

    return itFind->second;
}

bool CHardwareQuery::Init()
{
    //LST_VERSION_TABLE::value_type
    m_lstTableVersion.insert(LST_VERSION_TABLE::value_type(tstring(_T("5.0")), ver_win_2000));
    m_lstTableVersion.insert(LST_VERSION_TABLE::value_type(tstring(_T("5.1")), ver_win_xp));
    m_lstTableVersion.insert(LST_VERSION_TABLE::value_type(tstring(_T("5.2")), ver_win_server_2003));
    
    m_lstTableVersion.insert(LST_VERSION_TABLE::value_type(tstring(_T("6.1")), ver_win_7));
	m_lstTableVersion.insert(LST_VERSION_TABLE::value_type(tstring(_T("6.2")), ver_win_8));
	m_lstTableVersion.insert(LST_VERSION_TABLE::value_type(tstring(_T("6.3")), ver_win_8_1));
	m_lstTableVersion.insert(LST_VERSION_TABLE::value_type(tstring(_T("10.0")), ver_win_10));

    QueryInfo();
    return true;
}

tstring CHardwareQuery::ToVersionString( DWORD dwMajorVersion, DWORD dwMinorVersion )
{
    const int nSize = 20;
    //tstring strBuf(nSize, 0);
    tchar cBuf[20] = {0};
    _itot_s(dwMajorVersion, cBuf, nSize, 10);
    tstring strVersion;
    strVersion += cBuf;

    strVersion += _T(".");

    _itot_s(dwMinorVersion, cBuf, nSize, 10);
    strVersion += cBuf;
    return strVersion;
}

bool CHardwareQuery::IsSystemBit64()
{
    if (IsSoftBit64())  //64位能运行, 则是64位软件(32位运行不了, 但以后高位可以运行低位的)
    {
        return true;
    }
    typedef   BOOL   (WINAPI   *LPFN_ISWOW64PROCESS)   (HANDLE,   PBOOL); 

    LPFN_ISWOW64PROCESS   
        fnIsWow64Process   =   (LPFN_ISWOW64PROCESS)GetProcAddress( 
        GetModuleHandle( _T("kernel32")), "IsWow64Process"); 

    BOOL bBit64 = FALSE;

    if (!fnIsWow64Process)
    {
        return bBit64;
    }

    if(!fnIsWow64Process(GetCurrentProcess(), &bBit64))
    {
        return bBit64;
    }
    return bBit64 == TRUE;
}

float CHardwareQuery::GetMemoryUsePercent()
{
    float fPercent = 0;
    fPercent = NUM_PERCENT_MAX 
        - DIGIT_TO_PERCENT( INT_TO_FLOAT(GetMemoryPhysicalAvailable()) / GetMemoryPhysicalTotal() );
    return fPercent;
}

void CHardwareQuery::GetLocalIp()
{
    if(!InitNetLib())
    {
        return;
    }

    char host_name[255] = {0}; 
    //获取本地主机名称 
    if (gethostname(host_name, sizeof(host_name)) == SOCKET_ERROR) { 
        //printf("Error %d when getting local host name.n", WSAGetLastError()); 
        return ; 
    } 
    printf("Host name is: %s\n", host_name); 
    //从主机名数据库中得到对应的“主机” 
    struct hostent *phe = gethostbyname(host_name); 
    if (phe == 0) { 
        printf("Yow! Bad host lookup."); 
        return; 
    } 
    //循环得出本地机器所有IP地址 
    for (int i = 0; phe->h_addr_list[i] != 0; ++i) { 
        struct in_addr addr; 
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr)); 
        string strIp = inet_ntoa(addr);
        //printf("Address %d : %s\n" , i, ); 
    } 

    UnInitNetLib();
    return ; 
}

bool CHardwareQuery::InitNetLib()
{
    WSAData wsaData; 
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) 
    {
        return false;
    }
    return true;

}

bool CHardwareQuery::UnInitNetLib()
{
    WSACleanup(); 
    return true;
}

tstring CHardwareQuery::GetIEVersion()
{
    /***************** 从注册表获取到IE6 *****************/
    tstring strVersion;
    tstring strRegName;
    tcpchar szVerRegHigh = _T("svcVersion");
    tcpchar szVerRegLow = _T("Version");

    HKEY hKey = HKEY_LOCAL_MACHINE;
    HKEY hRet;
    tcpchar szSubKey = _T("SOFTWARE\\Microsoft\\Internet Explorer");
//     En_SYSTEM_VERSION enSysVer = GetSystemVersionLocal();
//     if (enSysVer == ver_win_8)
//     {
//         strRegName = _T("svcVersion");  //win8 版本键名
//     }
//     else
//     {
//         strRegName = _T("Version");
//     }

    LONG nRet = 0;
    nRet= ::RegOpenKeyEx(hKey, szSubKey, NULL, KEY_READ, &hRet);
    if(nRet != ERROR_SUCCESS )
    {
        return strVersion;
    }
    tchar cTemp[100] = {0};
    tchar cBuf[100] = {0};
    DWORD nData = 0;
    DWORD nBufSize = sizeof(cBuf);
    DWORD nDataType;
    
    --nBufSize;

    nRet =::RegQueryValueEx(hRet, szVerRegHigh, NULL, NULL, LPBYTE(cBuf), &nBufSize);
    if(ERROR_SUCCESS != nRet)
    {
        _itot(nRet, cTemp, 10);
        //PAOut10(_T("IE highVer fail:"), cTemp);

        nRet =::RegQueryValueEx(hRet, szVerRegLow, NULL, NULL, LPBYTE(cBuf), &nBufSize);
        if(ERROR_SUCCESS != nRet)
        {
            _itot(nRet, cTemp, 10);
            //PAOut10(_T("IE LowVer fail:"), cTemp);
        }
    }
    strVersion = cBuf;
/*
    nRet =::RegQueryValueEx(hRet, szVerRegLow, NULL, NULL, LPBYTE(cBuf), &nBufSize)
    else if(ERROR_SUCCESS == nRet)
    {
        strVersion = cBuf;
        return strVersion;
    }
    
    _itoa(nRet, cBuf, 10);
    OutInfo(_T("IE LowVer fail:"), cBuf);
*/
    ::RegCloseKey(hRet);
    
    return strVersion;
}
/*

tstring CHardwareQuery::GetPublicIP()
{


#define error _T("")
#define STRING_WEB_IP _T("http://ip.dnsexit.com")

    CInternetSession    session;
    CHttpFile           *pFile  = NULL;
    CStdioFile          *ptFile = NULL;
    CString             strTemp = _T("");
    DWORD               dwStat  = 0;
    tstring             strIp;
    tchar               cIp[30] = {0};
    try
    {
        pFile = (CHttpFile *)session.OpenURL(STRING_WEB_IP);
        pFile->QueryInfoStatusCode(dwStat);
        if(dwStat == HTTP_STATUS_OK)
        {
            ptFile = (CStdioFile *)pFile;

            string     strIpTemp;
            if(ptFile->ReadString(strTemp))
            {
                CStringA strTempA = (char*)strTemp.GetBuffer();
                strTempA.TrimRight();
                strcpy((char*)cIp, strTempA.GetBuffer());
                
#ifdef _UNICODE
            MultiByteToWideChar( CP_ACP, 0, strTempA, strTempA.GetLength()
                , cIp, _countof(cIp));
#endif
            }
            strIp = cIp;
            pFile->Close();
            session.Close();
            return strIp;
        }
    }
    catch(CException* e)
    {
        //cout<<_T("open url failed")<<endl;
        return error;
    }
    return error;
}*/

bool CHardwareQuery::IsSoftBit64()
{
#ifdef _WIN64
    return true;
#endif
    return false;
}


tstring CHardwareQuery::GetSystemVersionName()
{
	tstring name;
	OSVERSIONINFOEX osvi;  
	BOOL bOsVersionInfoEx;  

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));  
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);  
	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);  

	// win7的系统版本为NT6.1   
	if (/*VER_PLATFORM_WIN32_NT == osvi.dwPlatformId && */   
		osvi.dwMajorVersion == 5 &&   
		osvi.dwMinorVersion == 1 )  
	{  
		name = _T("xp");
	}  
	else if (/*VER_PLATFORM_WIN32_NT == osvi.dwPlatformId && */   
		osvi.dwMajorVersion == 6 &&   
		osvi.dwMinorVersion == 0 ) 
	{  
		name = _T("vista");
	}
	else if (/*VER_PLATFORM_WIN32_NT == osvi.dwPlatformId && */   
		osvi.dwMajorVersion == 6 &&   
		osvi.dwMinorVersion == 1 ) 
	{  
		name = _T("win7");
	}
	else if (/*VER_PLATFORM_WIN32_NT == osvi.dwPlatformId && */   
		(osvi.dwMajorVersion > 6 &&   
		osvi.dwMinorVersion == 0)||  
		(osvi.dwMajorVersion == 6 &&   
		osvi.dwMinorVersion == 2)) 
	{  
		name = _T("win8");
	}

	return name;
}

#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
tstring CHardwareQuery::GetMACAddress()
{
		/*bool ret = false;

		ULONG outBufLen = sizeof(IP_ADAPTER_ADDRESSES);
		PIP_ADAPTER_ADDRESSES pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
		if (pAddresses == NULL) 
			return false;
		// Make an initial call to GetAdaptersAddresses to get the necessary size into the ulOutBufLen variable
		if(GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW)
		{
			free(pAddresses);
			pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
			if (pAddresses == NULL) 
				return false;
		}

		if(GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen) == NO_ERROR)
		{
			// If successful, output some information from the data we received
			for(PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses; pCurrAddresses != NULL; pCurrAddresses = pCurrAddresses->Next)
			{
				// 确保MAC地址的长度为 00-00-00-00-00-00
				if(pCurrAddresses->PhysicalAddressLength != 6)
					continue;
				TCHAR acMAC[32] = {0};
				
				_stprintf_s(acMAC, 32, _T("%02X-%02X-%02X-%02X-%02X-%02X"),
					int (pCurrAddresses->PhysicalAddress[0]),
					int (pCurrAddresses->PhysicalAddress[1]),
					int (pCurrAddresses->PhysicalAddress[2]),
					int (pCurrAddresses->PhysicalAddress[3]),
					int (pCurrAddresses->PhysicalAddress[4]),
					int (pCurrAddresses->PhysicalAddress[5]));
				return acMAC;
				ret = true;
				break;
			}
		}*/
	size_t nStringLength = 100;
	tstring strMac(nStringLength,0);
	NCB ncb;
	LANA_ENUM lenum;
	ADAPTER_STATUS state;
	UCHAR ucReturnCode;
	ncb.ncb_command=NCBENUM;
	ncb.ncb_buffer=(UCHAR*)&lenum;
	ncb.ncb_length=sizeof(lenum);
	ucReturnCode=Netbios(&ncb);
	if(lenum.length>=0)
	{
		int num=lenum.lana[0];
		memset(&ncb,0,sizeof(ncb));
		ncb.ncb_command=NCBRESET;
		ncb.ncb_lana_num=num;
		ucReturnCode=Netbios(&ncb);
		memset(&ncb,0,sizeof(ncb));

		ncb.ncb_command = NCBASTAT;
		ncb.ncb_lana_num = num;
		ncb.ncb_buffer = (unsigned char *)&state;
		ncb.ncb_length = sizeof(state);
		strcpy( (char *)ncb.ncb_callname, "*" );
		ucReturnCode = Netbios(&ncb);
		_stprintf_s(&strMac.at(0), nStringLength, _T("%02X-%02X-%02X-%02X-%02X-%02X"),
			state.adapter_address[0],
			state.adapter_address[1],
			state.adapter_address[2],
			state.adapter_address[3],
			state.adapter_address[4],
			state.adapter_address[5]);
	}

	return strMac.c_str();
}

tstring CHardwareQuery::GetSystemBit()
{
	tstring strBit(_T("32"));
	if(IsSystemBit64())
	{
		strBit = _T("64");
	}
	return strBit;
}

tstring CHardwareQuery::GetCPUID()
{
	//x64不支持汇编
	#if 0
tstring strID(30, 0);
	unsigned long s1,s2;
	unsigned char vendor_id[]="------------";

	__asm{
		mov eax,01h
			xor edx,edx
			cpuid
			mov s1,edx
			mov s2,eax
	}
	_stprintf_s(&strID.at(0), strID.length(), _T("%08X%08X"),s1,s2);
	return strID;
#endif
	return _T("");
}
