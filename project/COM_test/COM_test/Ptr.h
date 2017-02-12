#pragma once
#include <unknwnbase.h>
template<class T, const IID* piid>
class IPtr
{
public:
	IPtr()
	{
		m_pI = nullptr;
	}

	IPtr(IUnknown* pUnknown);
	~IPtr();
public:
	operator T*() { return m_pI; };
	T* operator=(IUnknown* pUnknown);

	T** operator &() { return &m_pI;  };
	T& operator *(){ return *m_pI; }
	T* operator ->(){ return m_pI; }

	
protected:
	void Release();
private:
	T*		m_pI;
};





template<class T, const IID* piid>
IPtr<T, piid>::~IPtr()
{
	Release();
}


template<class T, const IID* piid>
T* IPtr<T, piid>::operator=(IUnknown* pUnknown)
{
	if (m_pI == pUnknown)
	{
		return m_pI;
	}
	if (m_pI)
	{
		m_pI->Release();
		m_pI = NULL;
	}
	pUnknown->QueryInterface(*piid, (void**)&m_pI);
	return m_pI;
}


template<class T, const IID* piid>
void IPtr<T, piid>::Release()
{
	if (m_pI)
	{
		m_pI->Release();
	}
}

