// DllCommonMFC.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "DllCommonMFC.h"
#include "UI\PiFileDialog.h"
#include "PiString.h"
#include "platform/DelphiTrans.h"

Pi_NameSpace_Using

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static ARR_STRING g_lstSel;
//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CDllCommonMFCApp

BEGIN_MESSAGE_MAP(CDllCommonMFCApp, CWinApp)
END_MESSAGE_MAP()


// CDllCommonMFCApp 构造

CDllCommonMFCApp::CDllCommonMFCApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDllCommonMFCApp 对象

CDllCommonMFCApp theApp;

const GUID CDECL _tlid = { 0x6C7E93CE, 0x24A1, 0x4E7B, { 0x95, 0x30, 0x53, 0xE9, 0x21, 0xC4, 0x43, 0x4D } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CDllCommonMFCApp 初始化

BOOL CDllCommonMFCApp::InitInstance()
{
	CWinApp::InitInstance();

	// 将所有 OLE 服务器(工厂)注册为运行。  这将使
	//  OLE 库得以从其他应用程序创建对象。
	COleObjectFactory::RegisterAll();

	return TRUE;
}

// DllGetClassObject - 返回类工厂

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}


// DllCanUnloadNow - 允许 COM 卸载 DLL

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}


// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll())
		return SELFREG_E_CLASS;

	return S_OK;
}


// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll(FALSE))
		return SELFREG_E_CLASS;

	return S_OK;
}





NCCOMMON_API int NCCOMMON_CALL SelectFileOrDir(tagSELECT_FILE_DIR* pTag)
{
	int nSel = 0;
	tstring  strSelectPath;
	tstring strBtnOkName(_T("确定"));
	if (!pTag->szTitle || !*pTag->szTitle)
	{
		pTag->szTitle = _T("选择文件(目录)");
	}

	if (!pTag->szBtnOkName || !*pTag->szBtnOkName)
	{
		pTag->szBtnOkName = strBtnOkName.c_str();
	}
	tstring strFilter = CDelphiTrans::FilterToMFC(pTag->szFilter);
	pTag->szFilter = strFilter.c_str();

	CPiFileDialog dlgFile(pTag->szTitle, pTag->szFilter, pTag->bSelectMulti);
	dlgFile.SetParam(pTag);
	if (!dlgFile.Popup())
	{
		return nSel;
	}
	g_lstSel = dlgFile.GetSelect();
	nSel = g_lstSel.size();
	return nSel;
}

NCCOMMON_API const wchar_t* QuerySelectFile(int nIndex)
{
	return g_lstSel.at((UINT)nIndex).c_str();

}

NCCOMMON_API bool NCCOMMON_CALL PopSaveDialog(tagSELECT_FILE_DIR* pTag, wchar_t* szPath)
{
	if (!szPath)
	{
		return false;
	}
	tstring strFilter = CDelphiTrans::FilterToMFC(pTag->szFilter);
	pTag->szFilter = strFilter.c_str();

	tstring strPath = CPIUITool::PopSaveDialog(pTag).c_str();
	if (strPath.length() >= MAX_PATH)
	{
		_tcscpy_s(szPath, MAX_PATH, _T("buf too small"));
		return false;
	}
	_tcscpy_s(szPath, MAX_PATH, strPath.c_str());
	return strPath.length() > 0;
}

