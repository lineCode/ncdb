#include "StdAfx.h"
#include "RAIILock.h"
#include "PiLock.h"
#include "PiTypeDef.h"

Pi_NameSpace_Begin
#if defined(BOOST_SHARE_PTR) || defined(M_STL_RAII)

#ifdef BOOST_SHARE_PTR
Pi_M_NameSpace_Using(boost)
#else
Pi_M_NameSpace_Using(std::tr1)
#endif

CRAIILock::CRAIILock( CPiLock* pLock )
#ifdef BOOST_SHARE_PTR
	:shared_ptr<CPiLock>(pLock, FuncUnLock)
#elif defined(M_STL_RAII)
	:std::tr1::shared_ptr<CPiLock>(pLock, FuncUnLock)
#endif
{
	pLock->Lock();
}

#else
CRAIILock::CRAIILock( CPiLock* pLock )
{
	if (!pLock)
	{
		return;
	}
	m_pLock = pLock;
	m_pLock->Lock();
}

CRAIILock::CRAIILock( auto_ptr<CPiLock>& pLock )
{
	if (!pLock.get())
	{
		return;
	}
	m_pLock = pLock.get();
	m_pLock->Lock();
}

CRAIILock::~CRAIILock(void)
{
	if (!m_pLock)
	{
		return;
	}
	m_pLock->UnLock();
}

void CRAIILock::reset()
{
	m_pLock->UnLock();
	m_pLock = NULL;
}

#endif


#if defined(BOOST_SHARE_PTR) || defined(M_STL_RAII)

CRAIILock::CRAIILock( auto_ptr<CPiLock>& pLock )
#ifdef BOOST_SHARE_PTR
:shared_ptr<CPiLock>(pLock.get(), FuncUnLock)
#else
:std::tr1::shared_ptr<CPiLock>(pLock.get(), FuncUnLock)
#endif
{
	pLock->Lock();

}



CRAIILock::~CRAIILock(void)
{
}

#else




#endif

void FuncUnLock(void* p)
{
	((CPiLock*)p)->UnLock();
}

void FuncClearMemArr(void* p)
{
    ClearMemArr(p);
}

Pi_NameSpace_End