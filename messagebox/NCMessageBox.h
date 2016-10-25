// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 NCMESSAGEBOX_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// NCMESSAGEBOX_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef NCMESSAGEBOX_EXPORTS
#define NCMESSAGEBOX_API __declspec(dllexport)
#else
#define NCMESSAGEBOX_API __declspec(dllimport)
#endif

//按钮定义
#define BTN_OK                       0
#define BTN_OKCANCEL                 1
#define BTN_YESRETRYCANCEL           2
#define BTN_YESNO                    3

//图标定义
#define NCICON_WARN                  0
#define NCICON_ERROR                 1
#define NCICON_QUESTION              2
#define NCICON_HINT                  3
#define NCICON_CORRECT               4

//返回值
#define MSGID_CANCEL		         0
#define MSGID_OK	                 1
#define MSGID_RETRY                  2
#define MSGID_CANCEL_NOWARN		     3
#define MSGID_OK_NOWARN	             4
#define MSGID_RETRY_NOWARN           5

//***************************************************************************
//                 HWND hWnd:          父窗口句柄，如果没有的话可传入NULL
//                 LPCTSTR lpText:     提示信息
//                 LPCTSTR lpCaption:  标题
//                 UINT uType:         按钮类型
//                 UINT uIcon:         图标类型
//                 POINT pPosition:    窗口弹出位置
//                 BOOL bCheck:        不再提示复选框
//***************************************************************************
NCMESSAGEBOX_API int NCMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType = BTN_OK, UINT uIcon = NCICON_WARN, POINT pPosition = { -1, -1 }, BOOL bCheck=FALSE);

//***************************************************************************
//                 
//               LPCTSTR lpCaption:    标题
//                 
//***************************************************************************
NCMESSAGEBOX_API void NCWarnWnd(LPCTSTR lpCaption);