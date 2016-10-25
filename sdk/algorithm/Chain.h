#pragma once
#include "PiTypeDef.h"

template <class T>
class CChain
{
	struct Node
	{
		T value;
		Node* next;
	};
public:
	CChain(void);
	~CChain(void);

public:
	bool Append(Node*);
	bool Append(T);
	UNLONG CopyAllToMemory(T* mem);
	/************************************************************************/
	/*	fun: 释放所有节点,子节点的内存                                                                     */
	/************************************************************************/
	bool DeleteAll();
private:
	Node* m_pNode;
	Node* m_pLast;
};



template <class T>
CChain<T>::CChain(void)
{
	m_pNode = NULL;
	m_pLast = NULL;
}

template <class T>
CChain<T>::~CChain(void)
{
	DeleteAll();
}

template <class T>
bool CChain<T>::Append(Node* pNode)
{
	if (m_pLast == NULL)
	{
		m_pNode = m_pLast = pNode;
	}
	else
	{
		m_pLast->next = pNode;
		m_pLast = pNode;
	}
	
	return true;
}

template <class T>
bool CChain<T>::Append(T value)
{
	Node* pNode = new Node;
	pNode->value = value;
	pNode->next = NULL;

	if (m_pLast == NULL)
	{
		m_pNode = m_pLast = pNode;
	}
	else
	{
		m_pLast->next = pNode;
		m_pLast = pNode;
	}

	return true;
}

template <class T>
UNLONG CChain<T>::CopyAllToMemory(T* mem)
{
	Node* pNode = m_pNode;
	DWORD i = 0;
	for (;pNode;++i)
	{
		*(mem + i) = pNode->value;
		pNode = pNode->next;
	}
	return i;
}

template <class T>
bool CChain<T>::DeleteAll()
{
	Node* pNext = m_pNode;
	while(pNext)
	{
		Node* pTemp = pNext->next;
		delete pNext;		
		pNext = pTemp;
	}
	return true;
}