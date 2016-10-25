#include "StdAfx.h"
#include "UpNet.h"
#include "InternetHttp.h"
#include "DownFileManager.h"

CNet::CNet(void)
{
	m_pDownManager	= new CDownFileManager(NULL, this);
}


CNet::~CNet(void)
{
}

bool CNet::DownLoadFileFromWeb(const string& strUrl, const string& strSavePath)
{
	//return m_pDownManager->(strUrl, strSavePath);
	return true;
}

// bool CNet::DownLoadFileFromWeb( const DownFileList& mapDownInfo )
// {
// 	if (0 >= mapDownInfo.size())
// 	{
// 		return false;
// 	}
// 
// 	//string strUrlHost = mapDownInfo.at(0);
// 	//m_pDownManager->DownloadInit();
// 	//for ()
// 	{
// 	}
// 	//m_pDownManager->DownloadUnInit();
// 	return true;
// }
