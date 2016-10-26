#pragma once
#include "PiTypeDef.h"
class CDataMng
{
public:
    CDataMng(void);


    virtual ~CDataMng(void);
public:
    /************************************************************************
        fun:	追加内存到数据队列里
        param:  
        ret:    
        remark:  TODO:每次追加都会分配新的内存, 后期需要改成自动增长备用缓存, 避免每次分配内存的问题
    /************************************************************************/
    bool Append(void* pMem, UNLONG nDataSize);

    /************************************************************************
        fun:	清除队列里所有数据
        param:  
        ret:    
        remark: 
    /************************************************************************/
    bool ClearData();

    void* GetData();
    UNLONG GetDataSize();
	void* Alloc( UNINT dwSize );

	operator void*();
	operator tpchar();
private:
    void InitVariable();
private:
    void*       m_pData;
    UNLONG      m_nDataSizeTotal;
};

