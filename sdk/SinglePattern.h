#pragma once
#include <Windows.h>
template <class T>
class CSinglePattern
{
public:
	virtual ~CSinglePattern(void);
private:
	CSinglePattern(void);
	
public:
	//设置新的对象,返回之前的对象
	
	//T* SetObj(T* pNewObj);
	static T*       GetObj();
    static  void    DeleteObj();

	void    Init();
	void    UnInit();
    
public:
	
	
private:
    CRITICAL_SECTION	m_cs; //对子类提供线程保护
    static T*	m_UniqueObj;
};

template<class T>
T* CSinglePattern<T>::m_UniqueObj = 0;




template <class T>
CSinglePattern<T>::CSinglePattern(void)
{
	m_UniqueObj = NULL;

	memset(&m_cs, 0, sizeof(CRITICAL_SECTION));
}

template <class T>
CSinglePattern<T>::~CSinglePattern(void)
{
	if (m_UniqueObj)
	{
		delete m_UniqueObj;
		m_UniqueObj = NULL;
	}
}


template <class T>
T* CSinglePattern<T>::GetObj()
{
	
	//主线程中调用来初始化, 其他线程就不用new对象, 因为没做线程保护
	//线程加锁
	//....
	if(!m_UniqueObj)
	{
		m_UniqueObj = new T;
		m_UniqueObj->Init();
	}
	//解锁
	return m_UniqueObj;
}

template <class T>
void CSinglePattern<T>::DeleteObj()
{
    if(m_UniqueObj)
    {
        delete m_UniqueObj;
    }
}

template <class T>
void CSinglePattern<T>::Init()
{

}
