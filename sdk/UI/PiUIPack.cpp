#include "StdAfx.h"
#include "PiUIPack.h"

#ifdef PI_CXIMAGE
#include "ximage.h"
#endif
#include <shlwapi.h>
#include "define_gnl.h"
#include <shellapi.h>
#include <shobjidl.h>
#include <objbase.h>
#include <windows.h>
#include <Shtypes.h>
#include <shlguid.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")


typedef HRESULT (*FuncDwmIsCompositionEnabled)(BOOL *pfEnabled);

CPiUIPack::CPiUIPack(void)
{
}

CPiUIPack::~CPiUIPack(void)
{
}

POINT CPiUIPack::GetCursorPos()
{
	POINT pt = {0};
	::GetCursorPos(&pt);
	return pt;
}

bool CPiUIPack::OpenUrl( tcpchar szUrl )
{
	if (!*szUrl)
	{
		return false;
	}
	ShellExecute(NULL, _T("open"), szUrl, NULL, NULL, SW_SHOWNORMAL);
	return true;
}

bool CPiUIPack::OpenBin( tcpchar szBin , tcpchar szParam )
{
	if (!*szBin
		|| PathFileExists(szBin) != TRUE)
	{
		return false;
	}
	::ShellExecute(NULL, _T("open"), szBin, szParam, NULL, SW_SHOWNORMAL);
	return true;
}

bool CPiUIPack::MakeShortCut( tcpchar szLinkPath, tcpchar szFilePath, tcpchar szParam )
{
	if (!szFilePath || !szLinkPath
		|| !*szFilePath || !*szLinkPath)
	{
		return false;
	}

	HRESULT hres;
	IShellLink *psl = NULL;
	IPersistFile *pPf = NULL;
	bool bRet = false;

	do
	{
		hres = CoInitialize(NULL);
		DealBoolBreak(!FAILED(hres));
		
		hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
		DealBoolBreak(SUCCEEDED(hres));
		
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&pPf);
		DealBoolBreak(SUCCEEDED(hres));

		
		hres = psl->SetPath(szFilePath);
		DealBoolBreak(SUCCEEDED(hres));
		if(szParam)
		{
			hres = psl->SetArguments(szParam);
			DealBoolBreak(SUCCEEDED(hres));
		}
		hres = pPf->Save(szLinkPath, TRUE);
		DealBoolBreak(SUCCEEDED(hres));
		pPf->Release();
		bRet = true;

	} while (0);

	if (psl != NULL)
	{
		psl->Release();
	}

	CoUninitialize();
	return bRet;
}

bool CPiUIPack::ChangeShortCutIcon( StringCRef strLinkPath, StringCRef strIconPath )
{
	if (strLinkPath.empty() || strIconPath.empty())
	{
		return false;
	}

	HRESULT hres;
	IShellLink *psl = NULL;
	IPersistFile *pPf = NULL;
	bool bRet = false;

	do
	{
		hres = CoInitialize(NULL);
		if (FAILED(hres))
		{
			break;
		}

		hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
		if (FAILED(hres))
		{
			break;
		}

		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&pPf);
		if (FAILED(hres))
		{
			break;
		}

		hres = pPf->Load(strLinkPath.c_str(), STGM_READWRITE);    
		if (FAILED(hres))
		{
			break;
		}

		hres = psl->SetIconLocation(strIconPath.c_str(), 0);
		if (FAILED(hres))
		{
			break;
		}

		pPf->Save(strLinkPath.c_str(), TRUE);
		if (FAILED(hres))
		{
			break;
		}

		bRet = true;

	} while (0);

	if (pPf != NULL)
	{
		pPf->Release();
	}

	if (psl != NULL)
	{
		psl->Release();
	}

	CoUninitialize();

	return bRet;
	
}

bool CPiUIPack::Jpg2Ico( tcpchar szPicPath, tcpchar szPathIcon )
{
#ifndef PI_CXIMAGE
	return false;
#else
	CxImage  image;

	// bmp -> jpg

	if(!image.Load(szPicPath, CXIMAGE_FORMAT_JPG))
	{
		return false;
	}

	if (!image.IsValid())
	{
		return false;
	}

	if(!image.IsGrayScale())
	{
		image.IncreaseBpp(24);
	}

	image.SetJpegQuality(100);
	CPathLight::CreateMultipleDirectory(CPathLight::GetDirFromPath(szPathIcon).c_str());
	if(!image.Save(szPathIcon, CXIMAGE_FORMAT_ICO))
	{
		return false;
	}
	return true;
#endif

}


bool CPiUIPack::IsAeroStyle()
{
	{
		BOOL bEnable = FALSE;
		HMODULE hDll = LoadLibrary(_T("dwmapi.dll"));
		FuncDwmIsCompositionEnabled pFun = (FuncDwmIsCompositionEnabled)GetProcAddress(hDll, "DwmIsCompositionEnabled");
		if (pFun)
		{
			pFun(&bEnable);
			OutputDebugString(_T("已经获取到aero接口"));
		}
		else
		{
			OutputDebugString(_T("不支持获取aero接口"));
		}
		/*if (DwmIsCompositionEnabled(&bEnable))
		{
		}*/
		//OutInfo(_T("-------dwm值："), bEnable);
		FreeLibrary(hDll);
		return bEnable;
	}




	HKEY hKey;
	DWORD CompositionVaule = 0;
	DWORD dwType = REG_DWORD;
	DWORD cbData = sizeof(DWORD);
	LPCTSTR Regkey = _T("Software\\Microsoft\\Windows\\DWM");
	int ret=RegOpenKeyEx(HKEY_CURRENT_USER, Regkey, 0, KEY_READ | KEY_WRITE | KEY_WOW64_64KEY, &hKey);
	if (ERROR_SUCCESS==ret) 
	{
		ret=RegQueryValueEx(hKey, _T("Composition"), NULL, &dwType, (PBYTE)&CompositionVaule, &cbData);
		if (ERROR_SUCCESS==ret) 
		{
			if (CompositionVaule == 0)//不为0的话 说明可能不是aero风格
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}