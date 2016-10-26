#pragma once
#include <string>
#include <map>
using namespace std;
class CUNet;

//typedef map<string, string> DownFileList;

class CDownFileManager;
class CNet
{
public:
	CNet(void);
	~CNet(void);
public:
	bool DownLoadFileFromWeb(const string& strUrl, const string& strSavePath);
	//bool DownLoadFileFromWeb(const DownFileList& mapDownInfo);

private:
	CDownFileManager*			m_pDownManager;
};

