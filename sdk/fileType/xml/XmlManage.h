#pragma once
#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <stdint.h>
#include <string>

#include <math.h>
#include "tinystr.h"
#include "tinyxml.h"

using namespace std;

class CXmlManage
{
private:
	//	<name ,value>
	typedef map<string,string> MAP_DATA;
private:
	CXmlManage(){m_mapData.clear();};
	CXmlManage(const CXmlManage& xmlManage){}
public:
	~CXmlManage(){m_mapData.clear();};

	static CXmlManage* getInstance()	////����Ψһ����
	{
		return m_pXmlManage;
	}

	//		����XML�ļ�
	BOOL	LoadXml(const char*	pfileLocation);

	//		��ȡһ�����ݣ�����m_vecString��
	BYTE	GetRowAttribute();

	//		��ȡ����
	string	GetData(string strName);

	//		�����ӽڵ�
	BOOL	ToChild();

	//		���븸�ڵ����һ���ھӽڵ�
	BOOL	ToParentNext();
	//		���븸�ڵ����һ���ھӽڵ���ӽڵ�
	BOOL	ToParentNextChild();

	//		��ȡԪ����ʼ�����
	string  GetValue();
	//		�ж������Ƿ��Ѿ���ȡ����
	//BOOL	IsEnd();
	//void	Xml_Const_Data_go(TiXmlDocument*	pTiXmlDocument);					///ִ��
public:
	static CXmlManage *m_pXmlManage;
private:
	TiXmlDocument	mTiXmlDocument;
	//TiXmlElement* pRootElement;
	TiXmlElement	*m_pResource;
	TiXmlElement	*m_pResourceLast;
	//BYTE			m_byAllRow;
	//BYTE			m_ByCurrentRow;
	//TiXmlAttribute* *m_pAttribute;
public:
	MAP_DATA		m_mapData;
};

class CXmlManageHelper
{
public:
	~CXmlManageHelper()
	{
		CXmlManage *pXmlManager = CXmlManage::getInstance();
		if (pXmlManager)
		{
			delete pXmlManager;
			pXmlManager = NULL;
		}
	}
private:
	static CXmlManageHelper m_xmlManageHelper;
};


#define	only_pXmlManage	CXmlManage::getInstance()		////Ψһ��������
