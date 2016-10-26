#pragma once
#include "tTypeDef.h"
#include <tchar.h>
#include <string>
#include <list>
using namespace std;

struct ResAttribute
{
    tstring strAtbName;     //属性名
    tstring strAtbValue;    //属性值
};

struct ResNode
{
    typedef list<ResAttribute*> LST_ATTR;
    typedef list<ResNode*>      LST_NODE;

    tstring strNodeName;    //节点名
    bool bChildAttribute;    /* 是否是父节点的子属性, 如 
        kImageID(reT=image) */
    LST_ATTR  lstAttribute;   //节点自身的属性
    LST_NODE   lstChildNode;//子节点
    ResNode*        pParent;    //可选, 父节点指针
    ResNode()
    {
        pParent = NULL;
        bChildAttribute = false;
    }
    ~ResNode()
    {
        for (LST_ATTR::iterator it = lstAttribute.begin(); it != lstAttribute.end(); 
            ++it)
        {
            delete *it;
        }
        for (LST_NODE::iterator it = lstChildNode.begin(); it != lstChildNode.end(); 
            ++it)
        {
            delete *it;
        }
    }
};

class CDataAssist
{
public:
    CDataAssist(void);
    ~CDataAssist(void);
public:

    bool Init();
    /************************************************************************
		增加一个新的节点, 进入一层nNew, 之后所有操作都针对新的一层nNew
	/************************************************************************/
    void AddNote(TCHAR* szName);

    /************************************************************************
		fun:添加节点本身的属性
	/************************************************************************/
    void AddAttributeSelf(TCHAR* szAtbName, TCHAR* szValue);

    /************************************************************************
		function：	添加当前节点的子属性,在嵌套里面的属性
		remark:		把这看成节点, 但bChildAttribute 为true;   
                最终格式: 把这也看成节点, 但子节点名和属性名一样
	/************************************************************************/
    void AddAttributeChild(TCHAR* szAtbName, TCHAR* szValue);


    /************************************************************************
		fun:返回上级节点层次pParent, 之后所有操作都针对新的一层pParent
        remark: 从大括号{ }里面弹出到上一层
	/************************************************************************/
    void PopLayer();
    /************************************************************************
		保存到最终的数据结构里
	/************************************************************************/
    bool SaveToDist();
    /************************************************************************
		function：	清除所有节点的数据, 释放内存
		remark:		
	/************************************************************************/
    void ClearAllData();
    void UnInit();
    ResNode*        m_pNodeRoot;
    ResNode*        m_pResNodeCur;     //保存解析出来的节点
};

