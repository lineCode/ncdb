#pragma once
#include "PiTypeDef.h"
#include "define_gnl.h"
#include "ListMng.h"
#include "ThreadLight.h"
#include "ResCommon.h"


//bool (T::*)():  返回bool, 无参数的接口

template<class T>
class CFuncCallControl
:public CThreadLight
{
    typedef bool (T::*FuncVoid)();
    typedef list<FuncVoid> LST_FUNC;

public:
    CFuncCallControl(T* pObj);
    ~CFuncCallControl(void);
public:
	virtual DWORD DoWork() ;

	bool Init();
	bool UnInit();
    bool Execute();
    bool Add(FuncVoid func);
    bool SetEndCallNormal(FuncVoid func);
    bool SetEndCallError(FuncVoid func);
	
private:
	T*								m_pObj;
	LST_FUNC						m_lstFunc;
	typename LST_FUNC::iterator*	m_itDelete;
	FuncVoid						m_funcEndCallNormal;
	FuncVoid						m_funcEndCallError;
};

template<class T>
bool CFuncCallControl<T>::UnInit()
{
	CThreadLight::UnInit();
	ClearMem(m_itDelete);
	return true;
}

template<class T>
bool CFuncCallControl<T>::Init()
{
	this->SetThreadName(_T("CFuncCallControl"));
	return CThreadLight::Init();
}

template<class T>
DWORD CFuncCallControl<T>::DoWork()
{
	bool bRet = true;
	m_itDelete = new LST_FUNC::iterator;
	LST_FUNC::iterator& it = *m_itDelete;
    FOR_EACH_ITER_NO_DESC(LST_FUNC, m_lstFunc, it)
    {
        if( ! ((m_pObj->*(*it))() ))
        {
            bRet = false;
	        (m_pObj->*m_funcEndCallError)();
	        break;
        }
    }
    if (bRet)
    {
        (m_pObj->*m_funcEndCallNormal)();
	}
	ClearMem(m_itDelete);

	return true;
}

template<class T>
CFuncCallControl<T>::~CFuncCallControl( void )
{
	UnInit();
}

template<class T>
bool CFuncCallControl<T>::SetEndCallError( FuncVoid func )
{
    m_funcEndCallError = func;
    return true;
}

template<class T>
bool CFuncCallControl<T>::SetEndCallNormal( FuncVoid func )
{
    m_funcEndCallNormal = func;
    return true;
}

template<class T>
CFuncCallControl<T>::CFuncCallControl( T* pObj )
{
    m_pObj  = pObj;
    m_funcEndCallNormal = NULL;
    m_funcEndCallError = NULL;
	m_itDelete			= nullptr;
}

template<class T>
bool CFuncCallControl<T>::Add( FuncVoid func )
{
    m_lstFunc.push_back(func);
    return true;
}

template<class T>
bool CFuncCallControl<T>::Execute()
{
	return CThreadLight::Run();
    
}

