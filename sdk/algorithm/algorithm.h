#include "PiTypeDef.h"
#include <math.h>
#include "Chain.h"

template <class T>
void Swap(T& a, T& b);


template <class T>
void SwapIfMax(T* arr, DWORD pos1, DWORD pos2);

template<class T>
DWORD Max(T* arr, DWORD pos1, DWORD pos2);

template<class T>
DWORD Max(T* arr, DWORD lCount);

template<class T>
DWORD MaxIn3(T* arr, DWORD pos1, DWORD pos2, DWORD pos3);

template<class T>
DWORD Min(T* arr, DWORD pos1, DWORD pos2);

template<class T>
bool isHeap(T* arr, DWORD parent, DWORD pos2, DWORD pos3);

template<class T>
void MergerGroup(T* arr, DWORD dwPos1, DWORD dwPos2, DWORD dwGroupSize, DWORD dwTwoGroupTotalSize);


template<class T>
void RadixSort(T* arr ,DWORD lCount);

byte GetValueLength(DWORD dwValue);

template<class T>
byte GetMaxDigitLen(T* arr ,DWORD lCount);

byte GetDigit(DWORD dwValue, byte byDigit);

/************************************************************************/
/* 直接插入排序                                                         
 version: 1.0 ，2012-6-8 13:36:31  
 remark:	1. 首次实现，可正确排序，但数组大小是int类型，应该用DWORD类型，否则不接受大数组
			2. 每次都要bNeedMove 标识多次赋值了，望改进
/************************************************************************/
// 10 2 5 17 4 
template <class T>
bool StraightInsertionSort(T* Arr ,DWORD lCount, DWORD dwPos = 1)
{
	//DWORD dwPos = 1;//元素间的间隔,希尔排序中也会引用到
	DWORD dwSize = lCount;
	bool bNeedMove = false;
	for (DWORD i = dwPos;i<dwSize;i+= dwPos )
	{
		long j = i - dwPos ;
		bNeedMove = false;
		for(; j >= 0 && Arr[i] < Arr[j] ;j -= dwPos )
		{
			bNeedMove = true;
		}
		//插入到位置，从插入位置到之前i - 1的元素都向右移动
		T temp = Arr[i];
		j += dwPos ;//插入的位置
		if (bNeedMove)
		{
			for (int iMove = i; iMove> j; iMove -= dwPos)
			{
				Arr[iMove] = Arr[iMove - dwPos];
			}
			Arr[j] = temp;
		}
		
	}
	return true;
};

/************************************************************************/
/* function: 直接选择排序     
    argm:1 - 要排序的数组首地址, 2 - 元素个数
/************************************************************************/
template<class T>
bool SimpleSelectionSort(T* arr ,DWORD lCount)
{
	DWORD i = 0;
	const T* pTemp = arr;
	for(;i < lCount;++i)
	{
		//获取最小的元素index
		DWORD dwMinIndex = Min(pTemp + i, lCount - i);
		if (dwMinIndex + i != i)
		{
			Swap(arr[i], arr[dwMinIndex + i]);
		}
	}
	return true;
}


/************************************************************************/
/* 希尔排序                                                                     */
/************************************************************************/
template<class T>
bool ShellSort(T* arr ,DWORD lCount)
{
	//先对半分分组, gPos = n/2; 每次增量必须<= 元素个数/2
	UNLONG dwPos = lCount/2;
	while(1)
	{
		
		//对分组后的每组进行直接插入排序
		for (UNLONG i = 0;i<dwPos;++i)
		{
			StraightInsertionSort(arr + i, lCount - i, dwPos);
		}
		if (1 == dwPos)
		{
			break;
		}
		dwPos = dwPos/2;
		// 在折半分组, gPos = gPos/2;
		
	}
	
	return true;
}

/************************************************************************/
/* 堆排序                                                                     */
/***********************************************************************/
template<class T>
void HeapSort(T* arr ,DWORD lCount)
{
    //建初始堆
    BuildHeap(arr, lCount);

    //调整堆    

	//调整最大值到后面, 从后面开始一直调整到第二个元素
	
	for (DWORD i = 1;i<lCount;++i)
	{
		Swap(arr[0], arr[lCount - i]);
		if(!isHeap(arr,0, 1, 2))
		{
			HeapAdjust(arr, 1, lCount - i);
		}
	}	
};


template<class T>
void BuildHeap(T* arr, DWORD lCount)
{
	for (UNLONG dwPos = lCount/2; dwPos > 0 ;--dwPos)
	{
		HeapAdjust(arr, dwPos,  lCount);
	}

}

template<class T>
void HeapAdjust(T* arr, DWORD NoteId, DWORD dwTotal)
{
	DWORD dwMaxIndex = dwTotal - 1;
	if(NoteId < 1)
	{
		return ;
	}
	UNLONG dwParent = NoteId - 1;
	UNLONG dwLChild = NoteId * 2 - 1;
	UNLONG dwRChild = NoteId * 2 + 1  - 1;

	dwRChild = (dwRChild > dwMaxIndex ) ? dwMaxIndex  : dwRChild;
	if (dwLChild <= dwMaxIndex  && dwRChild <= dwMaxIndex)
	{
		//找出父节点,和左右孩子中的最大值
		UNLONG arrTemp[] = { arr[dwParent], arr[dwLChild], arr[dwRChild] };
		UNLONG dwMaxPos = MaxIn3(arr, dwParent, dwLChild, dwRChild);

		if (dwMaxPos != dwParent)
		{
			Swap(arr[dwParent], arr[dwMaxPos]);

			//如果被交换项有孩子, 也要调整堆
			if (dwMaxPos <= dwTotal/2)
			{
				HeapAdjust(arr, dwMaxPos + 1, dwTotal);
			}
		}
	}
}

/************************************************************************/
/*	fun: 判断是否为堆                                                                     */
/************************************************************************/
template<class T>
bool isHeap(T* arr, DWORD parent, DWORD pos2, DWORD pos3)
{
	return (arr[parent] > arr[pos2] ) && (arr[parent] > arr[pos3]);
}

/************************************************************************/
/*	fun:冒泡排序                                                                   
	
/************************************************************************/
template<class T>
bool BubbleSort(T* arr, DWORD lCount)
{
	for (DWORD i = 0;i< lCount -1;++i)
	{
		SingleBubble(arr,lCount - i);
	}
	
	Max(arr, lCount);
	return true;
}

template<class T>
void SingleBubble(T* arr, DWORD dwUnSort)
{
	for (DWORD i = 0;i < dwUnSort - 1;++i)
	{
		DWORD dwMaxPos = Max(arr, i, i + 1);

		if(i == dwMaxPos)
		{
			//需要交换
			Swap(arr[i], arr[i + 1]);
		}

	}
}

/************************************************************************/
/*	fun:快速排序                                                                   
	ret:递归实现
/************************************************************************/
template<class T>
void QuickSort(T* arr, DWORD dwCount)
{
	if(dwCount < 2 )
	{
		return ;
	}

	DWORD dwBasePos = 0;//关键数据的索引位置
	DWORD dwComparePos = dwCount - 1;
	char cOffsetToNext = -1;
	bool bLeft = true;
	while(dwBasePos != dwComparePos)
	{
		if((arr[dwBasePos] > arr[dwComparePos]) == bLeft )
		{
			Swap(arr[dwBasePos], arr[dwComparePos]);
			cOffsetToNext = 0 - cOffsetToNext;//下一个元素的偏移变为相反数
			bLeft  = !bLeft;//关键值在另一边

			//交换指针位置,并移到下一个待比较位置
			UNLONG dwTemp = dwComparePos;
			dwComparePos = dwBasePos + cOffsetToNext;
			dwBasePos = dwTemp;
		}
		else
		{
			dwComparePos +=  cOffsetToNext;
		}
	}
	
	//对左边和右边都分别进行一趟快速排序
	QuickSort(arr, dwBasePos);
	QuickSort(arr + dwBasePos + 1, dwCount -1 - dwBasePos);
}


/************************************************************************/
/*	fun:归并排序                                                                   
	ret:稳定排序
/************************************************************************/

template<class T>
void MergerSort(T* arr, DWORD dwCount)
{
	DWORD dwCountPerGroup = 2;
	//DWORD dwMaxMergerCroupSize = ceil(dwCount/2.0) ; //向上取整
	for (DWORD dwGroupSize = 1;dwCount > dwGroupSize; dwGroupSize *=2)
	{
		MergerSortSingle(arr, dwCount, dwGroupSize);
	}
}

//对这一层的所有组进行 指定大小的合并
template<class T>
void MergerSortSingle(T* arr, DWORD dwArrCount, DWORD dwGroupSize)
{
	//合并的元素, mergeA 和 mergeB
	DWORD dwNeedCount = ceil(ceil(dwArrCount/(double)dwGroupSize) / 2);//总共需要几次合并,这一趟才能合完
	DWORD dwCount = 0;
	for (DWORD i = 0;i<dwArrCount;i += dwGroupSize * 2)
	{
		//判断最后一组待合并数组个数是不是有少
		//DWORD dwTwoGroupSize = (dwGroupSize * 2);
		DWORD dwRemainder = dwArrCount % dwGroupSize;

		// 下面值有错
		dwCount++;//大组索引顺序
		DWORD dwSecond = i +  dwGroupSize;//mergeB所在的索引
		DWORD dwLack = dwRemainder && (dwCount == dwNeedCount) ? dwGroupSize - dwRemainder : 0;
		bool bValid = dwSecond < dwArrCount;// 待合并组是否有项2
		if (bValid)
		{
			MergerGroup(arr , i, i + dwGroupSize, dwGroupSize, dwLack);
		}
		
	}
}

template<class T>
void MergerGroup(T* arr, DWORD dwPos1, DWORD dwPos2, DWORD dwGroupSize, DWORD dwLack)
{
	//DWORD dwMaxIndex = dwGroupSize - 1;
	DWORD dwAfterMerger = dwGroupSize * 2 -dwLack;
	DWORD dwComparePos1 = dwPos1;
	DWORD dwComparePos2 = dwPos2;
	DWORD dwMoveTime1 = 0;
	DWORD dwMoveTime2 = 0;
	//需分配空间来存放合并后的数据
	T* pMergerTemp = new T[dwAfterMerger];
	memset(pMergerTemp, 0, sizeof(T) * dwAfterMerger);
	DWORD i = 0;
	for (i = 0; i < dwAfterMerger && (dwMoveTime1 < dwGroupSize && dwMoveTime2 < dwGroupSize - dwLack) 
		;i += 1)
	{
		DWORD dwMaxPos = Min(arr, dwComparePos1, dwComparePos2);

		pMergerTemp[i] = arr[dwMaxPos];

		//移动指针到下一个
		if (dwComparePos1 == dwMaxPos)
		{
			dwComparePos1++;
			++dwMoveTime1;
		}
		else
		{
			//Swap(arr[dwPos1 + dwComparePos1], arr[dwPos2 + dwComparePos1]);
			dwComparePos2++;
			++dwMoveTime2;
		}
	}
	DWORD dwMoveTotal = dwMoveTime1 + dwMoveTime2;



	//如果有个指针移出区域了,另一个指针剩下的数据直接追加放入
	if (dwMoveTotal < dwAfterMerger)
	{
		//还没移动完的组,余下部分直接复制过去
		DWORD dwRemainPos = 0;
		if(dwMoveTime1 < dwGroupSize)
		{
			dwRemainPos = dwComparePos1;
		}
		else
		{
			dwRemainPos = dwComparePos2;
		}
		//DWORD dwRemainPos = (dwMoveTime1 <= dwMoveTime2 )? dwComparePos1 : dwComparePos2;
		memcpy_s(pMergerTemp + dwMoveTotal, sizeof(T) * (dwAfterMerger - dwMoveTotal), arr + dwRemainPos, sizeof(T) * (dwAfterMerger - dwMoveTotal));
	}

	DWORD dwTemp = sizeof(T) * dwAfterMerger; 
	memcpy_s(arr + dwPos1, dwTemp, pMergerTemp, dwTemp);
	delete [] pMergerTemp;
	//dwGroupSize
}


/************************************************************************/
/*	fun:基数排序                                            
	remart: 只适用于自然数. 里面涉及到字符串操作,性能有待改进
/***********************************************************************/
template<class T>
void RadixSort(T* arr ,DWORD lCount)
{
	byte byLen = GetMaxDigitLen(arr, lCount);//需要几轮
	for (byte i = 0;i<byLen;++i)
	{
		
		RadixSortSingle(arr, lCount, i);
	}
}

template<class T>
void RadixSortSingle(T* arr ,DWORD lCount, byte byRadix)
{
	//分配辅助空间
	CChain<DWORD>* pChainArr = new CChain<DWORD>[10];// 0-9

	for (DWORD i = 0;i<lCount;++i)
	{
		byte byValue = GetDigit(arr[i], byRadix);
		
		pChainArr[byValue].Append(arr[i]);//放到基数对应的位置
	}

	//按顺序复制到原有内存, 实现了元素顺序的调整
	DWORD dwCopyed = 0;
	for (DWORD i = 0;i<10;++i)// 链表 0-9
	{
		//pChainArr[i].m_pNode;
		dwCopyed += pChainArr[i].CopyAllToMemory(arr + dwCopyed);
	}
	delete [] pChainArr;
}
/************************************************************************/
/*	fun: 获取 数值的最长数位长度
/************************************************************************/
template<class T>
byte GetMaxDigitLen(T* arr ,DWORD lCount)
{
	DWORD dwMaxValue = arr[Max(arr, lCount)];
	return GetValueLength(dwMaxValue);
	
}

/************************************************************************
	fun: 获取一个整数有几位                                     
***********************************************************************/
byte GetValueLength(DWORD dwValue)
{
	char cTemp[30] = {0};
	_itoa_s(dwValue, cTemp, _countof(cTemp), 10);
	return strlen(cTemp);
	
}
/************************************************************************/
/*	fun:获取指定位上数值, 个位=0, 十位=1, 
	remark: 位数不够的,返回0
/************************************************************************/
byte GetDigit(DWORD dwValue, byte byDigit)
{
	char cTemp[30] = {0};
	_itoa_s(dwValue, cTemp, _countof(cTemp), 10);

	// ascii - 48 = 数字
	UNINT nLen = strlen(cTemp);
	if ( nLen < byDigit + 1)
	{
		return 0;
	}

	byte byDigitValue = cTemp[nLen - 1 -  byDigit] - 48;
	return byDigitValue;
}

/***********查找算法*****************************/

/************************************************************************/
/*	fun: 二分查找
/************************************************************************/
template<class T>
int BinarySearch(T* arr, DWORD dwCount, const T& key)
{
	DWORD dwLow = 0;
	DWORD dwHigh = dwCount - 1;
	
	DWORD dwMid = 0;
	int iPos = 0;
	for (;dwLow <= dwHigh;)
	{
		dwMid = (dwLow + dwHigh) / 2;
		if (key > arr[dwMid] )
		{
			dwLow = dwMid + 1;
		}
		else if(key < arr[dwMid])
		{
			dwHigh = dwMid - 1;
		}
		else
		{
			//找到了
			return dwMid;
		}
		++iPos;
	}
	return -1;
}

/************************************************************************/
/*	fun:分块查找                                                            
/************************************************************************/
//BlockSearch()
//{
//	//确定要分成几块
//}


/*-----辅助函数---------------------------------------------------*/

/************************************************************************/
/*  fun: 获取数组中的最小值                                        
    ret: 最大值的位置    
/************************************************************************/
template<class T>
DWORD Min(T* arr, DWORD lCount)
{
	UNLONG dwMinIndex = 0;
	
	for (UNLONG i = 1;i<lCount;++i)
	{
		if (arr[i] < arr[dwMinIndex])
		{
			dwMinIndex = i;
		}
	}
	return dwMinIndex;
}

template<class T>
DWORD Min(T* arr, DWORD pos1, DWORD pos2)
{
	if (arr[pos1] <= arr[pos2])
	{
		return pos1;
	}
	return pos2;
}
/************************************************************************/
/*  fun: 获取数组中的最大值                                        
    ret: 最大值的位置    
/************************************************************************/
template<class T>
DWORD Max(T* arr, DWORD lCount)
{
    UNLONG dwMaxIndex = 0;

    for (UNLONG i = 1;i<lCount;++i)
    {
        if (arr[i] > arr[dwMaxIndex])
        {
            dwMaxIndex = i;
        }
    }
    return dwMaxIndex;
}

/************************************************************************/
/*	fun:数组中3个值比较, 返回最大值的索引                                                                     
	ret:返回最大值的索引 
/************************************************************************/
template<class T>
DWORD MaxIn3(T* arr, DWORD pos1, DWORD pos2, DWORD pos3)
{
	UNLONG dwMaxIndex = 0;
	DWORD max1 = Max(arr, pos1, pos2);
	return Max(arr, max1, pos3);
}

//
/************************************************************************/
/*	fun:最大值,数组中元素的比较                                                                     
	ret:返回最大值的索引 
/************************************************************************/
template<class T>
DWORD Max(T* arr, DWORD pos1, DWORD pos2)
{
	if (arr[pos1] >= arr[pos2])
	{
		return pos1;
	}
	return pos2;
}

template <class T>
void Swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b= temp;
}

//如果最大值顺序不对,则交换位置
template <class T>
void SwapIfMax(T* arr, DWORD pos1, DWORD pos2)
{
	if(pos2 == Max(arr, pos1, pos2))
	{
		Swap(arr[pos1], arr[pos2]);
	}
	
}