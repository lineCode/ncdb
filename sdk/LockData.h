#pragma once
#include "PiTypeDef.h"
#include "memory"
#include "Lock.h"
using std::auto_ptr;

template <class T>
class CLockData
{
public:
	CLockData(const T& data);
	~CLockData(void);

	/************************************************************************
        fun:	
        param:  
        ret:    
        remark: 
	/************************************************************************/
	operator T();

	/************************************************************************
        fun:	赋值新数据, 并返回新数据
        param:  
        ret:    
        remark: 
	/************************************************************************/
    T operator =(const T& data);
	T operator ++();

	bool operator ==(const T& data);
	bool operator !=(const T& data);
	/************************************************************************
        fun:	设置新数据, 返回旧数据
        param:  
        ret:    
        remark: 
	/************************************************************************/
	T SetData(const T& data);
	//GetData();
private:
	T		m_data;
	auto_ptr<CLock>		m_pLock;
};

template <class T>
CLockData<T>::CLockData(const T& data)
{
	m_data = data;
	m_pLock = auto_ptr<CLock>(new CLock);
}


template <class T>
CLockData<T>::~CLockData( void )
{

}

template <class T>
T CLockData<T>::operator++()
{
    m_pLock->Lock();
    T dataTemp = ++m_data;
    m_pLock->UnLock();
    return dataTemp;
}

template <class T>
bool CLockData<T>::operator!=( const T& data )
{
	return !(this->operator ==(data));
}

template <class T>
bool CLockData<T>::operator==( const T& data )
{
	m_pLock->Lock();
	bool bRet = (m_data == data);
	m_pLock->UnLock();
	return bRet;
}

template <class T>
T CLockData<T>::operator=( const T& data )
{
	this->SetData(data);
	return data;
}



template <class T>
T CLockData<T>::SetData( const T& data )
{
	m_pLock->Lock();

	T dataOld = m_data;
	m_data = data;

	m_pLock->UnLock();
	return dataOld;
}

template <class T>
CLockData<T>::operator T()
{
	m_pLock->Lock();

	T data = m_data;

	m_pLock->UnLock();
	return data;
}

