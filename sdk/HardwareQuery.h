#pragma once
#include <Windows.h>
#include <map>
#include <string>
#include "PiTypeDef.h"
using namespace std;

enum En_SYSTEM_VERSION
{
    ver_unKnow = 0,
    ver_win_2000,
    ver_win_xp,
    ver_win_server_2003,
    ver_win_server_2003_R2,
    ver_win_vista,
    ver_win_server_2008,
    ver_win_7,
	ver_win_8,
	ver_win_8_1,
	ver_win_10,
};
typedef map<tstring, En_SYSTEM_VERSION> LST_VERSION_TABLE;





class CHardwareQuery
{
    
public:
    CHardwareQuery(void);
    ~CHardwareQuery(void);
    
public:
    bool Init();


    UNINT GetCpuThreadCountTotal();
    DWORDLONG GetMemoryPhysicalTotal();

    //剩余可用内存, 单位bytes
    DWORDLONG GetMemoryPhysicalAvailable();

    En_SYSTEM_VERSION GetSystemVersionLocal();
	static bool IsSystemBit64();
	static tstring GetSystemBit();
    bool QueryInfo();
    bool QueryMemoryState();
    tstring ToVersionString( DWORD dwMajorVersion, DWORD dwMinorVersion );
    float GetMemoryUsePercent();
    static tstring GetPublicIP();
    static void GetLocalIp();
    static tstring GetIEVersion();
    static bool InitNetLib();
    static bool UnInitNetLib();
    static bool IsSoftBit64();
	static tstring GetSystemVersionName();
	static tstring GetMACAddress();
	static tstring GetCPUID();
private:
    SYSTEM_INFO             m_si;
    MEMORYSTATUSEX          m_mse;
    LST_VERSION_TABLE           m_lstTableVersion;
};

