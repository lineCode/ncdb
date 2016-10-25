#pragma once
#include "define_gnl.h"
#include "PiTypeDef.h"
#include "ResCommon.h"
#include "Lock.h"
#include "RAIILock.h"
template<typename TypeElem, typename TypeElemCn>
class CListMng
{
public:
	CListMng(void);
	CListMng(const CListMng& lm) ;
    CListMng<TypeElem, TypeElemCn>& operator =(const CListMng<TypeElem, TypeElemCn>& lm) ;
    virtual ~CListMng(void);
public:
	bool Add(TypeElem* pElem);
    bool AddFront(TypeElem* pElem);
    bool Delete(TypeElem* pElem);
    bool Clear();
    TypeElem* First();
    /************************************************************************
        fun:    获取列表中的下一个元素
        param: 
        remark: 如果当前位置为初始位置, 返回第一个元素.   当前位置在末尾, 返回NULL
    ************************************************************************/
    TypeElem* Next();
    /************************************************************************
        fun:    类似于接口Next(), 区别是如果当前元素在末尾时, 会返回到第一个元素而不是NULL
        param: 
        remark: 
    ************************************************************************/
    TypeElem* NextCycle();

    /************************************************************************
        fun:    返回列表中的上一个元素的指针
        param: 
        remark: 如果当前位置为初始位置, 返回最后一个元素. 在头部 -> NULL
    ************************************************************************/
    TypeElem* Previous();

    /************************************************************************
        fun:    类似于接口Previous(), 区别是
        param: 
        remark: 如果当前位置为头部则返回NULL,  初始位置返回最后一个元素
    ************************************************************************/
    TypeElem* PreviousCycle();
    TypeElem* Last();
    /************************************************************************
        fun:    让内部元素位置回到头部初始化位置
        param: 
        remark: 
    ************************************************************************/
    bool ResetPos();
    bool SetPos(TypeElem* pElem);
    UNLONG      Count();
    bool SetData(const TypeElemCn& cn);
    TypeElemCn& GetList();
protected:
    TypeElemCn        m_lstElem;
	typename TypeElemCn::iterator*      m_pitElemCur;
	CLock							m_lockLstElem;
    //typename TypeElemCn::iterator&      m_itElemCur;
};

template<typename TypeElem, typename TypeElemCn>
CListMng<TypeElem, TypeElemCn>::CListMng(void)
	:m_pitElemCur(new TypeElemCn::iterator)
{
    ResetPos();
}


template<typename TypeElem, typename TypeElemCn>
CListMng<TypeElem, TypeElemCn>::CListMng( const CListMng& lm )
	:m_lstElem(lm.m_lstElem)
	,m_pitElemCur(new TypeElemCn::iterator)
{
	ResetPos();
	
}

template<typename TypeElem, typename TypeElemCn>
CListMng<TypeElem, TypeElemCn>::~CListMng(void)
{
    Clear();
}

template<typename TypeElem, typename TypeElemCn>
CListMng<TypeElem, TypeElemCn>& CListMng<TypeElem, TypeElemCn>::operator=( const CListMng<TypeElem, TypeElemCn>& lm ) 
{
	//*thislm;
	this->m_pitElemCur = new TypeElem TypeElemCn::iterator(lm.m_pitElemCur);
	return *this;
}

template<typename TypeElem, typename TypeElemCn>
bool CListMng<TypeElem, TypeElemCn>::AddFront( TypeElem* pElem )
{
	CRAIILock raii(m_lockLstElem.Lock());
	m_lstElem.push_front(pElem);
	return true;	
}

template<typename TypeElem, typename TypeElemCn>
bool CListMng<TypeElem, TypeElemCn>::Add( TypeElem* pElem )
{
	CRAIILock raii(m_lockLstElem.Lock());
    m_lstElem.push_back(pElem);
    return true;
}

template<typename TypeElem, typename TypeElemCn>
bool CListMng<TypeElem, TypeElemCn>::SetPos( TypeElem* pElem )
{
    FOR_EACH_ITER(TypeElemCn, m_lstElem, it)
    {
        if (*it == pElem)
        {
            m_itElemCur = it;
            break;
        }
    }
    return true;
}

template<typename TypeElem, typename TypeElemCn>
TypeElem* CListMng<TypeElem, TypeElemCn>::PreviousCycle()
{
    
    TypeElem* pElem = this->Previous();
    if (! pElem && this->Count())
    {
        pElem = this->Previous();
    }

    return pElem;
}

template<typename TypeElem, typename TypeElemCn>
TypeElem* CListMng<TypeElem, TypeElemCn>::NextCycle()
{
    TypeElem* pElem = this->Next();
    if (! pElem && this->Count())
    {
        pElem = this->Next();
    }

    return pElem;
}

template<typename TypeElem, typename TypeElemCn>
bool CListMng<TypeElem, TypeElemCn>::ResetPos()
{
	TypeElemCn::iterator&      m_itElemCur = *m_pitElemCur;
    m_itElemCur = m_lstElem.end();
    return true;
}

template<typename TypeElem, typename TypeElemCn>
bool CListMng<TypeElem, TypeElemCn>::Clear()
{
	CRAIILock raii(m_lockLstElem.Lock());
    FOR_EACH_ITER(TypeElemCn, m_lstElem, it)
    {
        delete *it;
    }
	ClearMem(m_pitElemCur);
    m_lstElem.clear();
    return true;
}

template<typename TypeElem, typename TypeElemCn>
UNLONG CListMng<TypeElem, TypeElemCn>::Count()
{
	CRAIILock raii(m_lockLstElem.Lock());
    return m_lstElem.size();
}

template<typename TypeElem, typename TypeElemCn>
bool CListMng<TypeElem, TypeElemCn>::SetData( const TypeElemCn& cn )
{
    m_lstElem = cn;
    return true;
}

template<typename TypeElem, typename TypeElemCn>
TypeElem* CListMng<TypeElem, TypeElemCn>::Last()
{
	CRAIILock raii(m_lockLstElem.Lock());
	TypeElemCn::iterator&      m_itElemCur = *m_pitElemCur;
    TypeElem* pElem = NULL; //TODO:如何让指针默认构造出来值为0
    m_itElemCur = m_lstElem.end();
    if (m_lstElem.size())
    {
        --m_itElemCur;//如果是空数组, 不能进行该操作
    }
    
    if(IsValidIter(m_itElemCur, m_lstElem))
    {
        pElem = *m_itElemCur;
    }

    return pElem;
}

template<typename TypeElem, typename TypeElemCn>
bool CListMng<TypeElem, TypeElemCn>::Delete( TypeElem* pElem )
{
	CRAIILock raii(m_lockLstElem.Lock());
	TypeElemCn::iterator&      m_itElemCur = *m_pitElemCur;
    FOR_EACH_ITER(TypeElemCn, m_lstElem, it)
    {
        if (*it == pElem)
        {
            it = m_itElemCur = m_lstElem.erase(it);
			ClearMem(pElem);
            break;
        }
    }
    return true;
}

template<typename TypeElem, typename TypeElemCn>
TypeElemCn& CListMng<TypeElem, TypeElemCn>::GetList()
{
    return m_lstElem;
}

template<typename TypeElem, typename TypeElemCn>
TypeElem* CListMng<TypeElem, TypeElemCn>::Previous()
{
	CRAIILock raii(m_lockLstElem.Lock());
	TypeElemCn::iterator&      m_itElemCur = *m_pitElemCur;
    TypeElem* pElem = NULL; //TODO:如何让指针默认构造出来值为0
    if (!IsValidIter(m_itElemCur, m_lstElem) 
        )
    {
        Last();
    }
    else if (m_itElemCur == m_lstElem.begin())
    {
        ResetPos();
    }
    else
    {
        --m_itElemCur;
    }
    if(IsValidIter(m_itElemCur, m_lstElem))
    {
        pElem = *m_itElemCur;
    }
    return pElem;
}

template<typename TypeElem, typename TypeElemCn>
TypeElem* CListMng<TypeElem, TypeElemCn>::Next()
{
	CRAIILock raii(m_lockLstElem.Lock());
	TypeElemCn::iterator&      m_itElemCur = *m_pitElemCur;
    TypeElem* pElem = NULL; //TODO:如何让指针默认构造出来值为0
    if (!IsValidIter(m_itElemCur, m_lstElem))
    {
        m_itElemCur = m_lstElem.begin();
    }
    else
    {
        ++m_itElemCur;
    }

    if(IsValidIter(m_itElemCur, m_lstElem))
    {
        pElem = *m_itElemCur;
    }
    
    return pElem;

}

template<typename TypeElem, typename TypeElemCn>
TypeElem* CListMng<TypeElem, TypeElemCn>::First()
{
	CRAIILock raii(m_lockLstElem.Lock());
	TypeElemCn::iterator&      m_itElemCur = *m_pitElemCur;
    TypeElem* pElem = NULL; //TODO:如何让指针默认构造出来值为0
    m_itElemCur = m_lstElem.begin();
    if(IsValidIter(m_itElemCur, m_lstElem))
    {
        pElem = *m_itElemCur;
    }

    return pElem;
}

