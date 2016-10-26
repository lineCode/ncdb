#include "stdafx.h"
#include "DataAssist.h"


CDataAssist::CDataAssist(void)
{
    m_pNodeRoot = NULL;
    m_pResNodeCur = NULL;
}


CDataAssist::~CDataAssist(void)
{
}

void CDataAssist::AddNote( TCHAR* szName )
{
    
    ResNode* pNodeNew = new ResNode;
    pNodeNew->strNodeName = szName;
    
    if (m_pResNodeCur)
    {
        pNodeNew->pParent = m_pResNodeCur;
        m_pResNodeCur->lstChildNode.push_back(pNodeNew);
        m_pResNodeCur = pNodeNew;       //进入到子层
    }
    else    
    {//第一个节点
        m_pNodeRoot = m_pResNodeCur = pNodeNew;
    }
    
}

void CDataAssist::AddAttributeSelf( TCHAR* szAtbName, TCHAR* szValue )
{
    ResAttribute* pRA = new ResAttribute;
    pRA->strAtbName = szAtbName;
    pRA->strAtbValue = szValue;

    m_pResNodeCur->lstAttribute.push_back(pRA);
}

void CDataAssist::AddAttributeChild( TCHAR* szAtbName, TCHAR* szValue )
{
    ResNode* pNodeNew = new ResNode;
    pNodeNew->bChildAttribute = true;
    
    ResAttribute* pRA = new ResAttribute;
    pRA->strAtbName = szAtbName;
    pRA->strAtbValue = szValue;
    
    pNodeNew->lstAttribute.push_back(pRA);

    pNodeNew->pParent = m_pResNodeCur;
    m_pResNodeCur->lstChildNode.push_back(pNodeNew);
}

void CDataAssist::PopLayer()
{
    m_pResNodeCur = m_pResNodeCur->pParent;
}

bool CDataAssist::SaveToDist()
{
    return true;
}

bool CDataAssist::Init()
{
    m_pNodeRoot = m_pResNodeCur = new ResNode;
    
    return true;
}

void CDataAssist::ClearAllData()
{
    UnInit();
}

void CDataAssist::UnInit()
{
    if(m_pNodeRoot)
    {
        delete m_pNodeRoot;
    }
}
