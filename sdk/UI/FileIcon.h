#pragma once
//////////////////////////////////////////////////////////////////////////
///

enum IconSize
{
	SMALL_ICON = 0,
	MEDIUM_ICON
};
class CFileIcon 
{
public:
	CFileIcon(void);
	~CFileIcon(void);

	LPCTSTR GetIconFile(LPCTSTR fileName, BOOL isFolder, BYTE iconSize);
	static CFileIcon *getInstance();

private:
	static CFileIcon *s_pInstance;
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	bool SaveHIconToPngFile(HICON hIcon, LPCTSTR lpszPicFileName);
	CDuiString GetIconPath(LPCTSTR fileName, bool isFolder, byte iconSize);
};
