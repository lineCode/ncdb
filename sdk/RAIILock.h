#pragma once
#include <memory>
#include "ResCommon.h"

Pi_NameSpace_Begin
//ºê, M_STL_RAII, BOOST_SHARE_PTR, M_PI_RAII

#ifdef BOOST_SHARE_PTR
#include <boost/shared_ptr.hpp>
#endif

class CPiLock;

void FuncClearMemArr(void* p);
void FuncUnLock(void* p);


#if defined(BOOST_SHARE_PTR) || defined(M_STL_RAII)

class CRAIILock
#ifdef BOOST_SHARE_PTR
	:public boost::shared_ptr<CPiLock>
#elif defined (M_STL_RAII)

	//:public std::tr1::shared_ptr<CLock>
#endif
{
public:
	CRAIILock(CPiLock* pLock);
    CRAIILock(auto_ptr<CPiLock>& pLock);
    ~CRAIILock(void);
};

#else

class CRAIILock
{
public:
	CRAIILock(CPiLock* pLock);
	CRAIILock(auto_ptr<CPiLock>& pLock);
	~CRAIILock(void);
public:
	void reset();
private:
	CPiLock*	m_pLock;
};

#endif

#if defined(BOOST_SHARE_PTR) || defined(M_STL_RAII)
class CRAIIHandle
{
public:
    CRAIIHandle(HANDLE h)
        :m_handle(h, ::CloseHandle)
    {

    }
private:
#ifdef BOOST_SHARE_PTR
	boost::shared_ptr<void> m_handle;
#elif defined M_STL_RAII

	std::tr1::shared_ptr<void> m_handle;
#endif
};


#endif





#if defined(BOOST_SHARE_PTR) || defined(M_STL_RAII)

class CRAIIMemArr
{
public:
    CRAIIMemArr(void* p)
        :m_handle(p, FuncClearMemArr)
    {

    }
private:
#ifdef BOOST_SHARE_PTR
	boost::shared_ptr<void> m_handle;
#else
	std::tr1::shared_ptr<void> m_handle;
#endif
};

#else

template<class T>
class CRAIIMem
{
public:
	CRAIIMem(T* p)
		:m_pData(p)
	{

	}
	~CRAIIMem() {delete m_pData;}

#if defined(BOOST_SHARE_PTR) || defined(M_STL_RAII)
#else
	T* operator ->()
	{
		return m_pData;

	}
	operator T*()
	{
		return m_pData;
	}
	T* get() const
	{	// return wrapped pointer
		return m_pData;
	}
	T* release()
	{
		T* pR = m_pData;
		m_pData = NULL;
		return pR;

	}
#endif

private:

	T* m_pData;

};


class CRAIIMemArr
{
public:
	CRAIIMemArr(void* p)
		:m_pData(p)
	{

	}
	~CRAIIMemArr() {delete [] m_pData;}
private:
	void* m_pData;

};
#endif

Pi_NameSpace_End