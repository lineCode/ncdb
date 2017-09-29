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

	static CXmlManage* getInstance()	////构造唯一对象
	{
		return m_pXmlManage;
	}

	//		加载XML文件
	BOOL	LoadXml(const char*	pfileLocation);

	//		获取一行数据，存入m_vecString中
	BYTE	GetRowAttribute();

	//		获取数据
	string	GetData(string strName);

	//		进入子节点
	BOOL	ToChild();

	//		进入父节点的下一个邻居节点
	BOOL	ToParentNext();
	//		进入父节点的下一个邻居节点的子节点
	BOOL	ToParentNextChild();

	//		获取元素起始标记名
	string  GetValue();
	//		判断数据是否已经读取结束
	//BOOL	IsEnd();
	//void	Xml_Const_Data_go(TiXmlDocument*	pTiXmlDocument);					///执行
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


#define	only_pXmlManage	CXmlManage::getInstance()		////唯一单件对象
