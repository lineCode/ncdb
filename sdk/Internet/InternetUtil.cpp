#include "StdAfx.h"
#include "InternetUtil.h"
#include <Windows.h>
#include <shellapi.h>
#include <afxinet.h>
#pragma comment(lib, "shell32.lib")

CInternetUtil::CInternetUtil(void)
{
}

CInternetUtil::~CInternetUtil(void)
{
}

bool CInternetUtil::OpenUrl( tcpchar szUrl )
{
	ShellExecute(NULL, _T("open"), szUrl, NULL, NULL, SW_SHOWNORMAL);
	return true;
}


tstring CInternetUtil::GetPublicIP()
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
}