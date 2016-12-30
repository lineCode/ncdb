#include <windows.h>
#include "NCCef.h"
#include "UI\PiUITool.h"
#include "UI\PiDataSource.h"

void OutInfo(tcpchar sz1, tcpchar sz2)
{
    tstring str(sz1);
    str += sz2;
    str += _T("\n");
    OutputDebugString(str.c_str());
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	{
		RECT rt = { 0, 0, 100, 100 };
		CPiDataSource piSour;
		//piSour.SetWindow(hWnd);
		piSour.PrepareDrag();
		//piSour.GeneralPic(strTempFile.c_str());
		//piSour.Drag(strTempFile.c_str());

		piSour.BeginDrag(_T("e:\\work\\svn\\nc\\src\\“发帖子”韩文怎么写？_百度知道.htm"), rt);
		return 1;

	}

	CNCCef* m_pCef = new CNCCef(hInstance);
	/*{
		CPIUITool::tagSELECT_FILE_DIR tag{};
		tag.szTitle = _T("title save");
		tag.szBtnOkName = _T("保存吧");
		tag.bCenterToParent = true;
		tstring strPath = CPIUITool::PopSaveDialog(&tag);
		OutputDebugString(strPath.c_str());
		return 0;
	}
    {
        CPIUITool::tagSELECT_FILE_DIR tag{};
        tag.szTitle = _T("title");
        tag.szBtnOkName = _T("发送吧");
        tag.bSelectMulti = true;
		

        int nSel = CPIUITool::SelectFileOrDir(&tag);
        for (int i = 0; i < nSel; ++i)
        {
            OutInfo(CPIUITool::QuerySelectFile(i), _T(""));
        }
        return 1;
    }*/
	m_pCef->Parent(NULL);
	OutputDebugString(L"cef enter\n");
	int nRet = 0;
	if (nRet = m_pCef->NeedReturn() >= 0)
	{
		OutputDebugString(L"cef enter return\n");
		return nRet;
	}
	m_pCef->Init(false);
	//m_pCef->Create(NULL);
	m_pCef->Start();
	OutputDebugString(L"cef will Uninit\n");
	m_pCef->UnInit();
	//m_pCef->Load(_T("sdf"));
	return 1;
}