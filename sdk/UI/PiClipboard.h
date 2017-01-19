#pragma once
class CPiClipboard
{
public:
	CPiClipboard();
	~CPiClipboard();

	static CPiClipboard *getInstance();
	BOOL CopyFilesToClipboard(std::vector<CDuiString> const & vecFiles);
	std::vector<CDuiString> GetFilesFromClipboard();
	CDuiString CreateGUID();
	bool CreateTempFile(LPCTSTR filePath);
	std::vector<CDuiString> GetLogicalDriver();
private:
	static CPiClipboard *s_pInstance;
};

