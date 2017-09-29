#include "stdafx.h"
#include "XmlManage.h"

CXmlManage* CXmlManage::m_pXmlManage = new CXmlManage();
CXmlManageHelper CXmlManageHelper::m_xmlManageHelper;



BOOL CXmlManage::LoadXml(const char*	pfileLocation)		///直接传入路劲
{
	//TiXmlDocument	mTiXmlDocument;
	BOOL bRet = mTiXmlDocument.LoadFile(pfileLocation,TIXML_ENCODING_UTF8);
	if (!bRet)
	{
		int nErr = mTiXmlDocument.ErrorId();
		return FALSE;
	}
	TiXmlElement* pRootElement = mTiXmlDocument.RootElement();
	if (!pRootElement)
	{
		return FALSE;
	}
	m_pResource = pRootElement->FirstChildElement();
	if (!m_pResource)
	{
		return FALSE;
	}
	//m_byAllRow = m_pResource->ValueTStr().size();
	//m_ByCurrentRow = 1;
	return TRUE;
}

BYTE CXmlManage::GetRowAttribute()
{
	if (m_pResource)
	{  
		
		m_mapData.clear();
		TiXmlAttribute* pAttribute = m_pResource->FirstAttribute();//获取entity elment的第一个属性
		if(!pAttribute)
		{
			return 0;
		}
		while (pAttribute)
		{
			string strName = pAttribute->Name();
			string	strValue = pAttribute->Value();
			m_mapData.insert(pair<string, string>(strName, strValue));
			//m_vecString.push_back(vstr);
			pAttribute = pAttribute->Next();
		}
		m_pResourceLast = m_pResource;
		m_pResource = m_pResource->NextSiblingElement();
		return 1;
	}
	return 0;
}


string CXmlManage::GetData(string strName)
{
	CStringA cstrErr;
	string str;
	MAP_DATA::const_iterator it = m_mapData.find(strName);
	if (it != m_mapData.end())
	{
		str = it->second;
		if(!str.length())
		{
			cstrErr.Format(("error: CXmlManage::GetData [%s] not data!"),strName.c_str());
			//AfxMessageBox(cstrErr);
			exit(0);
		}
	}
	/*else
	{
		cstrErr.Format(_T("error: CXmlManage::GetData [%s] not attribute!"),CGlobalOpt::CharToWchar(strName.c_str()));
		AfxMessageBox(cstrErr);
		exit(0);
	}*/
	return str;
}

BOOL CXmlManage::ToChild()
{
	m_pResource = m_pResource->FirstChildElement();
	if (!m_pResource)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CXmlManage::ToParentNext()
{
	m_pResource  = m_pResource ? m_pResource : m_pResourceLast;
	if (!m_pResource)
	{
		return FALSE;
	}
	TiXmlNode* pParent = m_pResource->Parent();
	if (pParent)
	{
		m_pResource = pParent->ToElement();
		m_pResource = m_pResource->NextSiblingElement();
	}
	
	return m_pResource != NULL;
}

BOOL CXmlManage::ToParentNextChild()
{
	if (ToParentNext())
	{
		return ToChild();
	}
	return FALSE;
}

string CXmlManage::GetValue()
{
	return m_pResource->Value();
}

/*
BOOL CXmlManage::IsEnd()
{
	if (m_byAllRow+1>=m_ByCurrentRow)
	{
		return FALSE;
	}
	return TRUE;
}
*/
