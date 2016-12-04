#pragma once
#include "..\PiTypeDef.h"
#include <Windows.h>
#include <shlwapi.h>
#include <shellapi.h>
using std::string;




class CPIUITool
{
public:
	struct tagSELECT_FILE_DIR
	{
		HWND			hParent;			//所属窗口， 可以为空
		const wchar_t*	szBeginDir;			//初始显示目录, 可为空
		const wchar_t*	szTitle;			//窗口标题， 可为空
		const wchar_t*	szBtnOkName;		//ok按钮要替换成的名字，  如果为空则显示为"确定"
		const wchar_t*	szFilter;			//过滤字符串
		bool			bCenterToParent;	//true相对父窗口居中， 否则屏幕居中
		bool			bSelectMulti;		//true为允许选择多个文件
		
		//以下成员属性内部使用
		//int				nSelectCount;		
		//int				pFileLst;			

		tagSELECT_FILE_DIR()
			:hParent(NULL)
			, szBeginDir(NULL)
			, szTitle(NULL)
			, szBtnOkName(NULL)
            , szFilter(nullptr)
			, bCenterToParent(true)
			, bSelectMulti(false)
		{

		}
	};
	struct tagSAVE_FILE
	{
		HWND			hParent;			//所属窗口， 可以为空
		const wchar_t*	szBeginDir;			//初始显示目录, 可为空
		const wchar_t*	szTitle;			//窗口标题， 可为空
		const wchar_t*	szFilter;			//过滤字符串
		const wchar_t*	szBeginFileName;	//默认文件名
		bool			bCenterToParent;	//true相对父窗口居中， 否则屏幕居中
		bool			bTypeSmall;			//true使用小窗口样式， 跟打开对话框一样的大小

		tagSAVE_FILE()
			:hParent(NULL)
			, szBeginDir(nullptr)
			, szTitle(nullptr)
			, szFilter(nullptr)
			, szBeginFileName(nullptr)
			, bCenterToParent(true)
			, bTypeSmall(false)
		{}
	};
	/************************************************************************/
	/*	fun: 获取一个拖放文件路径, 窗口需要支持拖放:通过DragAcceptFiles设置
	/************************************************************************/
	static tstring GetOneDragFilePath(const HDROP& hd);
	static tstring SelectDirectory(HWND hParent, tcpchar szBeginDir, tcpchar szTitle);

	/************************************************************************
		fun:	弹出选择对话框，  可选择文件或目录
		param:	szBeginDir, 弹窗后显示的起始目录， 如果目录不存在则显示上次的目录
		memo:	
	************************************************************************/
	static int SelectFileOrDir(tagSELECT_FILE_DIR* pTag);
	static ARR_STRING& GetFileList();
	static void AlterPath();
	static tcpchar QuerySelectFile(UNINT nIndex);

	/************************************************************************
		fun:	保存对话框
		param:	
		memo:	
	************************************************************************/
	static tstring PopSaveDialog(tagSAVE_FILE* pTag);
private:
	static		ARR_STRING		m_FileList;
};
