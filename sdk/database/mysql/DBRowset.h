#pragma once
#include "./include/mysql.h"

#define MAX_RESULT			24				//

class CDBRowset
{
public:
	CDBRowset(void);
	virtual ~CDBRowset(void);
public:
	//��ӽ������������ã�
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

	//������ʱ �л�����һ�������
	BOOL NextResult(void);

	//ʹ���±��л������
	BOOL SeekResult(INT32 nIndex);

	//��������
	BOOL ClearResult(void);

	//��ȡִ��SQL����Ľ��������
	UINT32 GetResultNumber(void)
	{
		return m_nResultCount;
	}

	//��ȡ�������������
	UINT32 GetRowsNumber(void);

	//��ȡ�������������
	UINT32 GetFieldsNumber(void);

	//�����������
public:
	//�ж��Ƿ񵽽������β
	BOOL IsEOF(void)
	{
		if (m_CurRow == NULL)
		{
			return TRUE;
		}

		return FALSE;
	}

	//��һ��
	void NextRow(void)
	{
		if (m_pCurResult != NULL)
		{	
			m_CurRow = mysql_fetch_row(m_pCurResult);
		}
	}

	//��ȡ����
	const char* GetFieldName(int nCol)
	{
		mysql_field_seek(m_pCurResult, nCol);//��λ����0��
		MYSQL_FIELD* pField = mysql_fetch_field( m_pCurResult );
		if ( pField != NULL )
		{
			return pField->name;
		}
	}

	//�����ݲ���
public:
	//��ȡINT����
	int GetIntField(const char* szField, int nNullValue=0);
	//��ȡUINT����
	unsigned int GetUintField(const char* szField, int nNullValue=0);
	//��ȡFLOAT����
	double GetFloatField(const char* szField, double fNullValue=0.0);
	//��ȡ������
	time_t GetDateTimeField(const char* szField,time_t tNullValue = 0);
	//��ȡ���������ݣ�δʵ�֣�
	//const unsigned char* GetBlobField(const char* szField, int& nLen);
	//��ȡ�ַ�����
	const char* GetStringField(const char* szField, const char* szNullValue="");


	//	��ȡlong long����
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
