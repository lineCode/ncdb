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
	T&& operator &() {return m_pI};
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
void IPtr<T, piid>::Release()
{
	if (m_pI)
	{
		m_pI->Release();
	}
}

