#pragma once
#include <vector>
#include "PiTypeDef.h"
using namespace std;
namespace ns_PiPi
{


template <class ContainerDist, class ContainerSource, class InputIterator, class FindIterator>
class CDealParam
{
public:
    ContainerDist ctnDist;      //被查找区域容器 dist
    ContainerSource ctnFind;      //源查找区域容器 source
    InputIterator itDist;   //查找到时, dist的迭代器
    FindIterator itSource; //查找到时, source的迭代器
    bool        bFind;      //查找结果

};

/************************************************************************/
/*  fun: 参数类, 传进FindArrayInArrayEx
    remark: FindFirst里的每一个在 sourceFirst里查找, 如果找到, 调用指定的处理函数
/************************************************************************/
template<class ContainerDist, class ContainerSource,  class InputIterator, class FindIterator/*, class FunDealResult*/>
class CFindArrayInArrayParam
{
private:
    typedef bool (*FunFind)(InputIterator&, FindIterator&);
    typedef void (*PredDealNewCar)(const CDealParam<ContainerDist, ContainerSource, InputIterator, FindIterator> & dParam);
public:
    ContainerDist       ctnDist;         //source容器
    InputIterator       distFirst;  //元素将在这里查找, 第一个迭代器
    InputIterator       distEnd;    //最后一个迭代器
    ContainerSource     ctnFind;
    FindIterator        FindFirst;  //需要查找元素的第一个迭代器
    FindIterator        FindEnd;
    FunFind             funFind;    //用来判断是否查找到
    PredDealNewCar       funDealRet;//对查找结果进行处理
};


template<class T>
bool DeleteVector(typename T& container, const typename T::value_type& ele );

/************************************************************************/
/*  fun:在数组里查找 一个数组中的每个元素是否存在, 并用查找结果来调用指定的函数
    param:sourceFirst, 在这里查找, 目标第一个迭代器
            sourceEnd, 目标最后一个迭代器(iterator.end())
            FindFirst: 需要查找元素的第一个迭代器
            FindEnd: 需要查找元素的最后一个迭代器
            fun: 对查找结果要调用的函数(对象), 传入参数: 1-当前查找的元素,2-查找结果
/************************************************************************/

template <class InputIterator, class T, class Function>
void FindArrayInArray(InputIterator sourceFirst, InputIterator sourceEnd, 
                      InputIterator FindFirst, InputIterator FindEnd, Function fun);

/************************************************************************/
/*  fun:增强型, 在sourceIt里 查找每一个 FindIt, 可以在结构体里查找 某个字段, eg: 
        class A
        {
            string s;
            int     value;
        }
        根据 一个string来查找相应的元素
    param: fun:函数(对象),接受两个参数:source 元素, 将在这里查找; find, 要查找的元素
    ret:无法对原先的两个容器进行添加或删除元素操作
/************************************************************************/
template <class Container, class ContainerSource, class InputIterator, class FindIterator/*, class FunFindPred, class FunDealResult*/>
void FindArrayInArrayEx(const CFindArrayInArrayParam<Container, ContainerSource, InputIterator, FindIterator/*, FunFindPred, FunDealResult*/>& findParam);

/************************************************************************/
/*  fun:stl for_each 增强版, 可以传进个参数,作为断言函数的第二个参数
/************************************************************************/
template<class Iterator, class Function, class Param>
void for_each(Iterator itBegin, Iterator itEnd, Function fun, Param& par);

/************************************************************************/
/*  fun:for_each可修改增加删除 源迭代器的内容
    remark: 需要自己决定是否递增迭代器
/************************************************************************/
template<class Containter, class Iterator, class Function, class Param>
void for_each_alter(Containter& ct, Iterator& itBegin, Iterator& itEnd, Function fun, Param& par);

/************************************************************************/
/*  fun:stl find_if 增强版, 自定义断言函数, 并且可以传进个参数, 作为断言函数的第二个参数
/************************************************************************/
template<class Iterator, class Function, class Param>
Iterator find_if(Iterator itBegin, Iterator itEnd, Function fun, Param& par);

/************************************************************************/
/*  fun: 增加
/************************************************************************/
template <class Iterator, class IntType>
void Advance(Iterator& it, IntType& nPosTotal, UNLONG nPos);

template <class Iterator>
typename Iterator::value_type GetListElem(Iterator& ctn, UNINT nPos);


template<class T>
bool DeleteVector(typename T& container, const typename T::value_type& ele )
{
    for (T::iterator it = container.begin();it != container.end();++it)
    {
        if( (*it) == ele)
        {
            container.erase(it);
            return true;
        }
    }
    return false;
};

template <class InputIterator, class T, class Function>
void FindArrayInArray(InputIterator sourceFirst, InputIterator sourceEnd, 
                 InputIterator FindFirst, InputIterator FindEnd, Function fun)
{
    InputIterator FindIt = FindFirst;
    for (;FindIt != FindEnd; ++FindIt)
    {
        bool bRet = (sourceEnd != find(sourceFirst, sourceEnd, *FindIt));
        fun(*FindIt, bRet);
    }
    
}

template <class Container, class ContainerSource, class InputIterator, class FindIterator/*, class FunFindPred, class FunDealResult*/>
void FindArrayInArrayEx(const CFindArrayInArrayParam<
                        Container,
                        ContainerSource,
                        InputIterator, 
                        FindIterator/*, 
                        FunFindPred, 
                        FunDealResult*/>& findParam)
{
    FindIterator findIt = findParam.FindFirst;
    for (;findIt != findParam.FindEnd;/* ++findIt*/)
    {
        InputIterator distIt = findParam.distFirst;//从头开始查找
        bool bFind = false;
        for (;distIt  != findParam.distEnd; ++distIt)
        {
            bFind = findParam.funFind(distIt, findIt);
            if (bFind)
            {
                break;//找到了
            }
        }

        //对找到和没找到做处理
        CDealParam<Container,
            ContainerSource,
            InputIterator, 
            FindIterator> dParam;
        dParam.bFind    = bFind;
        dParam.ctnDist  = findParam.ctnDist;
        dParam.ctnFind  = findParam.ctnFind;
        dParam.itDist   = distIt;
        dParam.itSource = findIt;

        findParam.funDealRet(dParam);
    }
}

template<class Iterator, class Function, class Param>
void for_each(Iterator itBegin, Iterator itEnd, Function fun, Param& par)
{
    for (Iterator it = itBegin; it != itEnd; ++it)
    {
        fun((*it), par);
    }
}

template<class Containter, class Iterator, class Function, class Param>
void for_each_alter(Containter& ct, Iterator& itBegin, Iterator& itEnd, Function fun, Param& par)
{
    //Iterator::iterator itEnd = itSource.end();
    for (Iterator it = itBegin; it != itEnd;)
    {
        fun(ct, it, par);
    }
}

template<class Iterator, class Function, class Param>
Iterator find_if(Iterator itBegin, Iterator itEnd, Function fun, Param& par)
{
    for (Iterator it = itBegin; it != itEnd; ++it)
    {
        if(fun((*it), par))
        {
            return it;
        }
    }
    return itEnd;
}

template<class Iterator, class Function, class Param>
Iterator remove_if(Iterator itBegin, Iterator itEnd, Function fun, Param& par)
{
    for (Iterator it = itBegin; it != itEnd; ++it)
    {
        if(fun((*it), par))
        {
            return it;
        }
    }
    return itEnd;
}

template <class Iterator, class IntType>
void Advance(Iterator& it, IntType& nPosTotal, UNLONG nPos)
{
    advance(it, nPos);
    nPosTotal += nPos;
}

template <class Iterator>
typename Iterator::value_type GetListElem( Iterator& ctn, UNINT nPos )
{
	Iterator::iterator itBegin = ctn.begin();
	advance(itBegin, nPos);
	return *itBegin;
}

}



class CPiStlSupply
{
public:
	template <class Iterator>
	static Iterator Advance(Iterator& it, UNLONG nPos)
	{
		advance(it, nPos);
		return it;
	}
};


template<class Key, class MapType>
typename MapType::mapped_type GetElement(const MapType& map, Key k)
{
	auto it = map.find(k);
	if (it == map.end())
	{
		return MapType::mapped_type();
	}
	return it->second;
}