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
    ContainerDist ctnDist;      //�������������� dist
    ContainerSource ctnFind;      //Դ������������ source
    InputIterator itDist;   //���ҵ�ʱ, dist�ĵ�����
    FindIterator itSource; //���ҵ�ʱ, source�ĵ�����
    bool        bFind;      //���ҽ��

};

/************************************************************************/
/*  fun: ������, ����FindArrayInArrayEx
    remark: FindFirst���ÿһ���� sourceFirst�����, ����ҵ�, ����ָ���Ĵ�����
/************************************************************************/
template<class ContainerDist, class ContainerSource,  class InputIterator, class FindIterator/*, class FunDealResult*/>
class CFindArrayInArrayParam
{
private:
    typedef bool (*FunFind)(InputIterator&, FindIterator&);
    typedef void (*PredDealNewCar)(const CDealParam<ContainerDist, ContainerSource, InputIterator, FindIterator> & dParam);
public:
    ContainerDist       ctnDist;         //source����
    InputIterator       distFirst;  //Ԫ�ؽ����������, ��һ��������
    InputIterator       distEnd;    //���һ��������
    ContainerSource     ctnFind;
    FindIterator        FindFirst;  //��Ҫ����Ԫ�صĵ�һ��������
    FindIterator        FindEnd;
    FunFind             funFind;    //�����ж��Ƿ���ҵ�
    PredDealNewCar       funDealRet;//�Բ��ҽ�����д���
};


template<class T>
bool DeleteVector(typename T& container, const typename T::value_type& ele );

/************************************************************************/
/*  fun:����������� һ�������е�ÿ��Ԫ���Ƿ����, ���ò��ҽ��������ָ���ĺ���
    param:sourceFirst, ���������, Ŀ���һ��������
            sourceEnd, Ŀ�����һ��������(iterator.end())
            FindFirst: ��Ҫ����Ԫ�صĵ�һ��������
            FindEnd: ��Ҫ����Ԫ�ص����һ��������
            fun: �Բ��ҽ��Ҫ���õĺ���(����), �������: 1-��ǰ���ҵ�Ԫ��,2-���ҽ��
/************************************************************************/

template <class InputIterator, class T, class Function>
void FindArrayInArray(InputIterator sourceFirst, InputIterator sourceEnd, 
                      InputIterator FindFirst, InputIterator FindEnd, Function fun);

/************************************************************************/
/*  fun:��ǿ��, ��sourceIt�� ����ÿһ�� FindIt, �����ڽṹ������� ĳ���ֶ�, eg: 
        class A
        {
            string s;
            int     value;
        }
        ���� һ��string��������Ӧ��Ԫ��
    param: fun:����(����),������������:source Ԫ��, �����������; find, Ҫ���ҵ�Ԫ��
    ret:�޷���ԭ�ȵ���������������ӻ�ɾ��Ԫ�ز���
/************************************************************************/
template <class Container, class ContainerSource, class InputIterator, class FindIterator/*, class FunFindPred, class FunDealResult*/>
void FindArrayInArrayEx(const CFindArrayInArrayParam<Container, ContainerSource, InputIterator, FindIterator/*, FunFindPred, FunDealResult*/>& findParam);

/************************************************************************/
/*  fun:stl for_each ��ǿ��, ���Դ���������,��Ϊ���Ժ����ĵڶ�������
/************************************************************************/
template<class Iterator, class Function, class Param>
void for_each(Iterator itBegin, Iterator itEnd, Function fun, Param& par);

/************************************************************************/
/*  fun:for_each���޸�����ɾ�� Դ������������
    remark: ��Ҫ�Լ������Ƿ����������
/************************************************************************/
template<class Containter, class Iterator, class Function, class Param>
void for_each_alter(Containter& ct, Iterator& itBegin, Iterator& itEnd, Function fun, Param& par);

/************************************************************************/
/*  fun:stl find_if ��ǿ��, �Զ�����Ժ���, ���ҿ��Դ���������, ��Ϊ���Ժ����ĵڶ�������
/************************************************************************/
template<class Iterator, class Function, class Param>
Iterator find_if(Iterator itBegin, Iterator itEnd, Function fun, Param& par);

/************************************************************************/
/*  fun: ����
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
        InputIterator distIt = findParam.distFirst;//��ͷ��ʼ����
        bool bFind = false;
        for (;distIt  != findParam.distEnd; ++distIt)
        {
            bFind = findParam.funFind(distIt, findIt);
            if (bFind)
            {
                break;//�ҵ���
            }
        }

        //���ҵ���û�ҵ�������
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