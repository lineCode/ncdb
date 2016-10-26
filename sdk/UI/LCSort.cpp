// LCSort.cpp : 实现文件
//

#include "stdafx.h"
#include "RFID_Data_Server.h"
#include "LCSort.h"


// CLCSort

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

IMPLEMENT_DYNAMIC(CLCSort, CListCtrl)

CLCSort::CLCSort()
{
	m_pIsSortAsc	= NULL;
	m_bIsSortSupport = NULL;
	m_nClientColumn = 0;
	m_pfnCompare	= &CLCSort::CompareFunc;
}

CLCSort::~CLCSort()
{
	if (m_pIsSortAsc)
	{
		delete[] m_pIsSortAsc;
		m_pIsSortAsc	= NULL;
	}
}


BEGIN_MESSAGE_MAP(CLCSort, CListCtrl)
	
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CLCSort::OnLvnColumnclick)
END_MESSAGE_MAP()



// CLCSort 消息处理程序



void CLCSort::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_nClientColumn = pNMLV->iSubItem;

	this->SortAllItem();

	*pResult = 0;
}

void CLCSort::EndInsetColumn()
{
	DWORD dwColumnCount = this->GetHeaderCtrl()->GetItemCount();
	m_pIsSortAsc = new bool[dwColumnCount];
	memset(m_pIsSortAsc, 0, dwColumnCount * sizeof(bool));

}

bool CLCSort::ReverseColumnSort( UNINT nColumn )
{
	m_bSortAsc = m_pIsSortAsc[nColumn] = !m_pIsSortAsc[nColumn];
	return m_bSortAsc;
}

bool CLCSort::GetSortOrder()
{
	return m_bSortAsc;
}

bool CLCSort::SortAllItem()
{
	/************************************************************************/
	/*	通过设置每个item的 lParam数据为自身当前排列索引值, 在排序时通过lParam来标示一个item,
		保存在附加32bit值, 但这回覆盖掉之前设置的lParam
	/************************************************************************/

	//如果是默认的比较函数, 则采用赋索引的方式来标识每一个item
	if (&CLCSort::CompareFunc == m_pfnCompare )
	{
		int nCount = this->GetItemCount();
		assert(nCount >= 0 && "GetItemCount < 0");

		for (int i = 0; i < nCount; ++i)
		{
			if(!this->SetItemData(i, i))
			{
				assert(0 && "SetItemData fail");
			}
		}

	}
	/************************************************************************
		如果是其他比较函数, 则无需设置数字标识, 每项的lparam可以唯一标示一个项
	/************************************************************************/

	TWO_POINT tp;
	tp.pLC		= this;
	tp.nColumn	= m_nClientColumn;

	this->ReverseColumnSort(m_nClientColumn);

	this->SortItems(m_pfnCompare, (DWORD_PTR)&tp);
	return true;
}

int CALLBACK CLCSort::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	typedef CLCSort::TWO_POINT TWO_POINT;
	TWO_POINT* pTP = (TWO_POINT*)lParamSort;
	CLCSort* pLC = pTP->pLC;
	int nItemIndex = 0;
	bool bAsc	= false;

	CString str1;
	CString str2;

	nItemIndex = (int)lParam1;
	str1 = pLC->GetItemText(nItemIndex, pTP->nColumn);

	nItemIndex = (int)lParam2;
	str2 = pLC->GetItemText(nItemIndex, pTP->nColumn);

	int nRet = str1.Compare(str2);  // <0 : 小于
	bAsc = pLC->GetSortOrder();
	if (!bAsc)
	{
		nRet = -nRet;
	}
	// <0 参数1排前面, >0 参数1排后面
	return nRet;
}


pfnCompare CLCSort::SetCompareFun( pfnCompare pfn )
{
	pfnCompare pOld = m_pfnCompare;
	m_pfnCompare = pfn;
	return pOld;
}
