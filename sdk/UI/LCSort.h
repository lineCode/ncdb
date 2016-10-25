#pragma once
#include <windows.h>
#include <memory>

// CLCSort

typedef int  (CALLBACK* pfnCompare)(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
class CLCSort : public CListCtrl
{
	DECLARE_DYNAMIC(CLCSort)

public:
	struct TWO_POINT
	{	
		int			nColumn;		//当前点击的是哪一列
		CLCSort*	pLC;			//CListCtrl指针
	};
	CLCSort();
	virtual ~CLCSort();

protected:
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);

public:
	/************************************************************************/
	/*	fun: item排序接口
	/************************************************************************/
	bool SortAllItem();
	/************************************************************************/
	/*	fun: 用来指示列添加完成
	/************************************************************************/
	void EndInsetColumn();	

	/************************************************************************/
	/*	fun: 指定列当前是否为倒序排序
	/************************************************************************/
	bool ReverseColumnSort(UNINT nColumn);
	/************************************************************************/
	/*	fun: 获取当前应该进行的排序状态
		ret: 当前是否应该升序, true -> 升序,   false -> 降序
	/************************************************************************/
	bool GetSortOrder();
	pfnCompare SetCompareFun(pfnCompare pfn);
private:
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
private:
	bool*		m_pIsSortAsc;		//每一列当前的排序状态是否为升序,
	bool		m_bSortAsc;		//当前是否应该升序, true -> 升序,   false -> 降序
	bool		m_bIsSortSupport;
	int			m_nClientColumn;	//当前点击的列
	pfnCompare	m_pfnCompare;		//
};


