template <class T>
void Swap(T& a, T& b);


//template <class T>
//bool StraightInsertionSort(T* Arr )
//{
//	DWORD dwSize = _countof(arr);
//	//DWORD 
//	for (DWORD i = 1;i<dwSize;++i)
//	{
//		DWORD j = i - 1;
//		for(; Arr[i] < Arr[j] && j >= 0; --j)
//		{
//			
//		}
//		//插入到位置，从插入位置到之前i - 1的元素都向右移动
//		j = max(j, 0);
//		T temp = Arr[i];
//
//		for (int iMove = i; iMove>= j; ++iMove)
//		{
//			Arr[iMove] = Arr[iMove-1];
//		}
//		Arr[j] = temp;
//	}
//	return true;
//};
