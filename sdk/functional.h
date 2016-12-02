#pragma once
#include <Windows.h>
#include "PiTypeDef.h"
#include "define_gnl.h"
using namespace std;

#define _FUNCTIONAL_H_COMMON

#ifdef _FUNCTIONAL_H_COMMON 

//namespace ns_Functional
//{
//    const char* szDataFormat = "%04d/%02d/%02d %02d:%02d:%02d";
//}


//int 分解成带小数的浮点数
double IntResolveToFloat(int Num, UNCHAR decimal);

string  GetLocalDateTime(const char* cFormat);



wstring GetCurrentDirIniName();



//精确的延时指定的时间, MilliSecond
void Delay(UINT nMilliSecond);

/************************************************************************/
/*	fun: 内存原样转换成16进制的字符串                                                                     
/************************************************************************/
string MemToHex(byte* pMem, UNLONG dwSize);

/************************************************************************/
/*  fun: 内存拷贝到string， eg: char cChar[] = {'1','2'};  ->  string("12");
/************************************************************************/
void MemToString(string& strDist, void* pMem, UNINT nSize);
bool IsBigEndian();

//申请N个对象的内存, 返回每个对象指针组成的vector
template <class T>
vector<T*> NewPointArrar(T* tSize, UNLONG iCount);

/************************************************************************/
/*  fun:    把容器里的所有元素转成普通数组形式
    remark: 元素必须是同一类型
/************************************************************************/
template <class T>
typename T::value_type* ContainerToArray(T& container);

/************************************************************************/
/*  fun:    把数组内容转成指定容器类型的数据
/************************************************************************/
template <class T, class Ctn>
void ArrayToContainer(const T& pArr, UNLONG dwCount, Ctn& container);

//判断字符 是否是 0-F 的数字
bool IsHex(char cSource);



//判断字符串每个字符是否都是16进制内
bool DecideInHex(const string strSource);

/************************************************************************
        fun:	按行格式化内存块
        param:  
        ret:    
        remark: 
/************************************************************************/
template<class T>
void AdjustArrByLine(T* arr, UNINT nSize, UNINT nCountPerLine, ostringstream& oss);

template <class T>
vector<T*> NewPointArrar(T* tSize, UNLONG iCount)
{
	vector<T*> vt;
	for (UNLONG i = 0; i< iCount; ++i)
	{
		T* pNew= new T;
		vt.push_back(pNew);
	}
	return vt;
}

template <class T>
typename T::value_type* ContainerToArray(T& container)
{
    
    UNLONG dwMemSize = sizeof(T::value_type) * container.size();
    T::value_type* pMem = new T::value_type[container.size()];
    memset(pMem, 0, dwMemSize);
    T::value_type* pMemTemp = pMem;
    for (T::iterator it = container.begin(); it != container.end(); ++it)
    {
        *pMemTemp++ = *it;
    }
    return pMem;
}

template <class T, class Ctn>
void ArrayToContainer(T& pArr, UNLONG dwCount, Ctn& container)
{
    //vector<int> lInt;
    //typedef vector<int> CN_Type;

    for(UNLONG i = 0; i < dwCount; ++i)
    {
        container.push_back(*(pArr++));
    }
    
}


template<class T>
void AdjustArrByLine(T* arr, UNINT nSize, UNINT nCountPerLine, ostringstream& oss)
{
	UNINT nCount = 0;
	for (UNINT i = 0; i < nSize; ++i )
	{
		oss << (int)arr[i] << " ";
		
		if ((i + 1) % nCountPerLine  == 0) 
		{
			oss<<"\r\n";
		}
	}
}


/************************************************************************
    fun:	删除指定容器内所有元素保存的指针
    param:  
    ret:    none
    remark: 
/************************************************************************/
template<class T>
void ClearElemPtr(T& ctn)
{
    //list ctn;
    //list::iterator it2;
    for (T::iterator it = ctn.begin(); it != ctn.end(); ++it)
    {
        ClearMem((*it));
    }
}


/************************************************************************
    fun:    获取模块exe名
    param: 
    remark: 
************************************************************************/
tstring GetSelfModuleName();
tstring GetSelfModuleTitle();

void* GetFileData( tcpchar szfilePath );
tstring GetFileDataStr( tcpchar szfilePath, bool bUtf8 = false);

/**************************************** PA无用 ****************************************/







#endif

/************************************************************************
        fun:	把容器里的所有HANDLE转成数组形式
        param:  
        ret:    
        remark: 
/************************************************************************/

template<class Container, typename TypeNew>
 TypeNew* HandleToArr(Container& ctn);


template<class Container, typename TypeNew>
TypeNew* HandleToArr(Container& ctn)
{
	Container::iterator it = ctn.begin();

	size_t nSize = ctn.size();
	TypeNew* pElemArr = new TypeNew[nSize];
	for (size_t i = 0; i < nSize; ++i)
	{
		pElemArr[i] = *it;
		advance(it, 1);
	}
	return pElemArr;
}

template<class Cn>
tcpchar* CtStrToArr(Cn& ctn)
{
    Cn::iterator it = ctn.begin();

    size_t nSize = ctn.size();
    tcpchar* pElemArr = new tcpchar[nSize];
    for (size_t i = 0; i < nSize; ++i)
    {
        pElemArr[i] = it->c_str();
        advance(it, 1);
    }
    return pElemArr;
}

/************************************************************************
        fun: 获取系统temp临时目录
        param: 
        remark: 
************************************************************************/
tstring GetSysTempFolder();



/***************************进程, 线程, 文件等系统内核相关**********************************/

/************************************************************************
        fun:    判断指定进程名是否存在
        param: 
        remark: 
************************************************************************/
bool IsProcessExist( tcpchar szProcessName);


typedef bool (*EnumerateFunc) (tcpchar lpFileOrPath, void* pUserData );
/************************************************************************
        fun: 
        param: 参数3:bool, true->查找文件, false-> 查找目录
        remark: 
    ************************************************************************/
bool FileEnumeration(tcpchar lpPath, bool bRecursion, bool bEnumFiles, EnumerateFunc pFunc, void* pUserData);
bool FileEnumeration(tcpchar lpPath, bool bRecursion, bool bEnumFiles, LST_STRING& lstFile);
bool CB_EnumerateFunc(tcpchar lpFileOrPath, void* pUserData );



//tstring GetFileNameFromPath(tcpchar szPath);



TimeType GetFileTimeModify(tcpchar szPath);

void OutInfo(tcpchar sz1);
void OutInfo(tcpchar sz1, tcpchar sz2);
void OutInfo(tcpchar sz1, StringCRef sz2);
void OutInfo(tcpchar sz1, __int64 nValue);

bool DecodeUrl( tstring& strTime );

tstring MakeGUID();

/*------------------------------------------------------------
		fun:	根据分隔符, 提取出每段字符串放入列表
		ret:	
		param: 
		remark:	
------------------------------------------------------------*/
//ARR_STRING SpilitStrToArray(const tstring& strSrc, tcpchar szSpilit);
//LST_STRING SpilitStr(const tstring& strSrc, tcpchar szSpilit);
//PAIR_STRING SpilitStrTwoPart(const tstring& strSrc, tcpchar szSpilit);
//MAP_STRING SpilitStrMap(const tstring& strSrc, tcpchar szSpilit);

/*根据分隔符, 把列表中的每串字符串分成左右两部分, 返回map
	abc=4      =>        map["abc"]=4
	cf=5       =>		map["cf"]=5
*/
//MAP_STRING SpilitStrMap( const LST_STRING& lstSrc, tcpchar szSpilit );
//ARR_PAIR_STR SpilitStrToPairArray( const LST_STRING& lstSrc, tcpchar szSpilit );



POINT Client2Screen(HWND hwnd, long nlParam);
//POINT Client2Screen(HWND hwnd, CPoint nlParam);

template <class Container> 
bool MakeSequenceString(Container& ctn, tcpchar szFormat, int nBegin, int nEnd);

template <class Container>
bool MakeSequenceString( Container& ctn, tcpchar szFormat, int nBegin, int nEnd )
{
	if (nBegin < 0 || nEnd < 0)
	{
		return false;
	}
	ctn.clear();

	STLString strFm;
	FOR_COUNT( nEnd - nBegin + 1, i)
	{
		strFm.Format(szFormat, i + nBegin);
		ctn.push_back(strFm.c_str());
	}
	
	return true;
}

/************************************************************************
        fun:    调整矩形在指定的矩形rtBig范围里面
        param: 参数3 ptSpacing, 距离外部矩形的边框距离
        remark: 
************************************************************************/
bool MoveInRect(const RECT& rtBig, RECT& rtSrc, const POINT& ptSpacing );