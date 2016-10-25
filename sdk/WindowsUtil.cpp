#include "StdAfx.h"
#include "WindowsUtil.h"
#include "AutoType.h"
#include "functional.h"
#include <atlwin.h>
#include <algorithm>
CWindowsUtil::CWindowsUtil(void)
{
}


CWindowsUtil::~CWindowsUtil(void)
{
}

bool CWindowsUtil::EnumAllTopWindow()
{
    EnumWindows(EnumWindowsProc, (LPARAM)this);
    return true;
}

tagWindowInfo* CWindowsUtil::GetWindowsFullScreen()
{
    POINT pt = GetScreenSize();
    tstring strTemp;
    strTemp.resize(100);
    tstring strClassName;
    int n = 0;
    CAutoType at;
    FOR_EACH_ITER(LST_WINDOW, m_lstElem, it)
    {
        tagWindowInfo* pWndInfo = *it;
        
        ::GetClassName(pWndInfo->hWnd, &strTemp.at(0), 100);
        strClassName = strTemp.c_str();
        if (strClassName == _T("SK_OSD_WNDCLASS"))
        {
            //continue;
        }
        if(pWndInfo->rt == CRect(0, 0, pt.x, pt.y) && FilterWnd(pWndInfo))
        {
            strTemp.resize(100);
            _itot((int)pWndInfo->hWnd, &strTemp.at(0), 16);
            CString strMsg = pWndInfo->strName.c_str();
            strMsg += _T(", ");
            strMsg += strClassName.c_str();
            //AfxMessageBox(strMsg);
            OutInfo(strTemp.c_str());
            //n++;
            return pWndInfo;
        }
    }
    return NULL;
}

bool CWindowsUtil::FilterWnd( tagWindowInfo* pWndInfo )
{
    tstring strTemp;
    strTemp.resize(200);
    ::GetClassName(pWndInfo->hWnd, &strTemp.at(0), 200);
    tstring strClassName(strTemp.c_str());
    if( find(m_lstFilder.begin(), m_lstFilder.end(), strClassName) != m_lstFilder.end())
    {
        return false;
    }
    return true;
}

bool CWindowsUtil::Init()
{
    m_lstFilder.push_back(_T("SK_OSD_WNDCLASS"));
    m_lstFilder.push_back(_T("WorkerW"));
    m_lstFilder.push_back(_T("Progman"));
    return true;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)
{
    CWindowsUtil* pWndUt = reinterpret_cast<CWindowsUtil*>(lParam);
    tstring strCaption;
    tstring strSize;
    tstring strMsg;
    tstring strTemp;

    tagWindowInfo* pWndInfo = new tagWindowInfo;
    pWndInfo->hWnd = hwnd;
    CWindow wnd(hwnd);
    (strCaption);
    //CRect  rt;
    /*strTemp.resize(100);
    ::GetClassName(wnd, &strTemp.at(0), 100);*/

    wnd.GetWindowRect(&pWndInfo->rt);
    CAutoType at;
    strSize += (tcpchar)at.SetData(pWndInfo->rt.left);
    strSize += _T(",");
    strSize += (tcpchar)at.SetData(pWndInfo->rt.right);
    strSize += _T(",");
    strSize += (tcpchar)at.SetData(pWndInfo->rt.top);
    strSize += _T(",");
    strSize += (tcpchar)at.SetData(pWndInfo->rt.bottom);
    strTemp.resize(500);
    wnd.GetWindowText(&strTemp.at(0), 500);
    pWndInfo->strName = strTemp.c_str();

    pWndUt->Add(pWndInfo);
    strMsg = pWndInfo->strName + _T(":") + strSize;
    //OutInfo(strMsg.c_str());
    return TRUE;
}
