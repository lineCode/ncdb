NCCOMMON_API int NCCOMMON_CALL SelectFileOrDir(tagSELECT_FILE_DIR* pTag);
NCCOMMON_API const wchar_t* NCCOMMON_CALL QuerySelectFile(int nIndex);
/************************************************************************
fun:	弹窗保存对话框
param:	szPath: 用户选择后的路径， 缓存区需要在外部分配好， 大小为MAX_PATH  260;
memo:
************************************************************************/
NCCOMMON_API bool NCCOMMON_CALL PopSaveDialog(tagSELECT_FILE_DIR* pTag, wchar_t* szPath);


struct tagSELECT_FILE_DIR
	{
		HWND			hParent;			//所属窗口， 可以为空
		const wchar_t*	szBeginDir;			//初始显示目录, 可为空
		const wchar_t*	szBeginFileName;	//初始显示文件名
		const wchar_t*	szTitle;			//窗口标题， 可为空
		const wchar_t*	szBtnOkName;		//ok按钮要替换成的名字，  如果为空则显示为"确定"
		const wchar_t*	szFilter;			//过滤字符串
		bool			bCenterToParent;	//true相对父窗口居中， 否则屏幕居中
		bool			bSelectMulti;		//true为允许选择多个文件
		
		//以下成员属性内部使用
		//int				nSelectCount;		
		//int				pFileLst;			

		tagSELECT_FILE_DIR()
			:hParent(nullptr)
			, szBeginDir(nullptr)
			, szBeginFileName(nullptr)
			, szTitle(nullptr)
			, szBtnOkName(nullptr)
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

		tagSAVE_FILE()
			:hParent(nullptr)
			, szBeginDir(nullptr)
			, szTitle(nullptr)
			, szFilter(nullptr)
			, szBeginFileName(nullptr)
			, bCenterToParent(true)
		{}
	};





