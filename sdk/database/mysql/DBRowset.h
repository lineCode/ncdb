#pragma once
#include "./include/mysql.h"

#define MAX_RESULT			24				//

class CDBRowset
{
public:
	CDBRowset(void);
	virtual ~CDBRowset(void);
public:
	//添加结果集（引擎调用）
	void AddResult(MYSQL_RES* pRetsult)
	{
		if (m_nResultCount < MAX_RESULT)
		{
			m_arrayResult[m_nResultCount++] = pRetsult;
		}
		else
		{
			printf("more than result limit counts\n");
		}
	}

	//多结果集时 切换到下一个结果集
	BOOL NextResult(void);

	//使用下标切换结果集
	BOOL SeekResult(INT32 nIndex);

	//清理结果集
	BOOL ClearResult(void);

	//获取执行SQL语句后的结果集数量
	UINT32 GetResultNumber(void)
	{
		return m_nResultCount;
	}

	//获取单结果集的行数
	UINT32 GetRowsNumber(void);

	//获取单结果集的列数
	UINT32 GetFieldsNumber(void);

	//单结果集操作
public:
	//判断是否到结果集行尾
	BOOL IsEOF(void)
	{
		if (m_CurRow == NULL)
		{
			return TRUE;
		}

		return FALSE;
	}

	//下一行
	void NextRow(void)
	{
		if (m_pCurResult != NULL)
		{	
			m_CurRow = mysql_fetch_row(m_pCurResult);
		}
	}

	//获取列名
	const char* GetFieldName(int nCol)
	{
		mysql_field_seek(m_pCurResult, nCol);//定位到第0列
		MYSQL_FIELD* pField = mysql_fetch_field( m_pCurResult );
		if ( pField != NULL )
		{
			return pField->name;
		}
	}

	//行数据操作
public:
	//读取INT数据
	int GetIntField(const char* szField, int nNullValue=0);
	//读取UINT数据
	unsigned int GetUintField(const char* szField, int nNullValue=0);
	//读取FLOAT数据
	double GetFloatField(const char* szField, double fNullValue=0.0);
	//读取日期型
	time_t GetDateTimeField(const char* szField,time_t tNullValue = 0);
	//读取二进制数据（未实现）
	//const unsigned char* GetBlobField(const char* szField, int& nLen);
	//读取字符数据
	const char* GetStringField(const char* szField, const char* szNullValue="");


	//	读取long long数据
	__int64 GetI64Field(int nField, __int64 i64NullValue=0);
	__int64 GetI64Field(const char* szField, __int64 i64NullValue=0);

public:
	int GetFieldIndex(const char * szField);
	int GetIntField(int nField, int nNullValue=0);
	unsigned int GetUintField(int nField, int nNullValue=0);
	double GetFloatField(int nField, double fNullValue=0.0);
	//const unsigned char* GetBlobField(int nField, int& nLen);
	time_t GetDateTimeField(int nField,time_t tNullValue = 0);
	const char* GetStringField(int nField, const char* szNullValue="");

private:
	MYSQL_RES*				m_arrayResult[MAX_RESULT];
	int								m_nResultCount;
	int								m_CurIndex;

private:
	MYSQL_RES*				m_pCurResult;
	MYSQL_ROW				m_CurRow;
	INT32							m_nFieldCount;
};
