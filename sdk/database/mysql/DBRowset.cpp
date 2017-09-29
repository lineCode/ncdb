#include "StdAfx.h"
#include "DBRowset.h"
#include <time.h>

CDBRowset::CDBRowset(void)
:m_pCurResult(NULL)
,m_CurRow(NULL)
,m_nFieldCount(0)
,m_nResultCount(0)
,m_CurIndex(0)
{
	for (int i=0; i<MAX_RESULT;i++)
	{
		m_arrayResult[i] = NULL;
	}

}

CDBRowset::~CDBRowset(void)
{
	ClearResult();
}

//CDBRowset::CDBRowset(CDBRowset& rRowset)
//:m_pCurResult(NULL)
//,m_CurRow(NULL)
//,m_nFieldCount(0)
//,m_nResultCount(0)
//{
//	for (int i=0; i<MAX_RESULT;i++)
//	{
//		m_arrayResult[i] = NULL;
//	}
//
//	*this = rRowset;
//}

//CDBRowset& CDBRowset::operator=(CDBRowset& rRowset)
//{
//	if (this == &rRowset)
//	{
//		return *this;
//	}
//
//	ClearResult();
//
//	return *this;
//}

BOOL CDBRowset::NextResult(void)
{
	if (m_CurIndex < m_nResultCount)
	{
		m_CurIndex++;
		return SeekResult(m_CurIndex);
	}

	return FALSE;
}

BOOL CDBRowset::SeekResult(INT32 nIndex)
{
	if (nIndex >= 0 && nIndex < m_nResultCount)
	{
		m_pCurResult = m_arrayResult[nIndex];
		if (m_pCurResult != NULL)
		{
			mysql_data_seek(m_pCurResult,0);
			m_CurRow = mysql_fetch_row(m_pCurResult);
			m_nFieldCount = mysql_num_fields(m_pCurResult);
		}
		return TRUE;
	}

	return FALSE;
}

UINT32 CDBRowset::GetRowsNumber(void)
{
	if (m_pCurResult != NULL)
	{
		return (UINT32)mysql_num_rows(m_pCurResult);
	}

	return 0;
}

UINT32 CDBRowset::GetFieldsNumber(void)
{
	if (m_pCurResult != NULL)
	{
		return (UINT32)mysql_num_fields(m_pCurResult);
	}

	return 0;
}

int CDBRowset::GetFieldIndex(const char * szField)
{
	if ( m_pCurResult == NULL || szField == NULL)
	{
		return -1;
	}

	//定位到第0列
	mysql_field_seek(m_pCurResult, 0);

	INT32 i = 0;
	while ( i < m_nFieldCount )
	{
		MYSQL_FIELD* pField = mysql_fetch_field( m_pCurResult );
		if ( pField != NULL && strcmp(pField->name, szField) == 0 )
		{
			return i;
		}
		i++;
	}

	return -1;
}

inline const char* CDBRowset::GetStringField(int nField, const char* szNullValue/*=""*/)
{
	return m_CurRow[nField];
}

const char* CDBRowset::GetStringField(const char* szField, const char* szNullValue/*=""*/)
{
	int nField = GetFieldIndex(szField);
	if (nField < 0)
	{
		//SQL_LOG_ERR("Error:[%s] is bad field\n",szField);
		//printf("Error:[%s] is bad field\n",szField);
		return szNullValue;
	}
	return GetStringField(nField,szNullValue);
}

__int64 CDBRowset::GetI64Field(int nField, __int64 i64NullValue)
{
	const char* pszValue = m_CurRow[nField];
	if (NULL == pszValue)
	{
		return i64NullValue;
	}

	return _atoi64(pszValue);
}
__int64 CDBRowset::GetI64Field(const char* szField, __int64 i64NullValue)
{
	int nField = GetFieldIndex(szField);
	if (nField < 0)
	{
		printf("Error:[%s] is bad field\n",szField);
		return i64NullValue;
	}
	return GetI64Field(nField,i64NullValue);
}

time_t CDBRowset::GetDateTimeField(const char* szField,time_t tNullValue /*= 0*/)
{
	int nField = GetFieldIndex(szField);
	if (nField < 0)
	{
		printf("Error:[%s] is bad field\n",szField);
		return tNullValue;
	}

	return GetDateTimeField(nField,tNullValue);
}

inline time_t CDBRowset::GetDateTimeField(int nField,time_t tNullValue /*= 0*/)
{
	const char* pszValue = m_CurRow[nField];
	if (NULL == pszValue)
	{
		return tNullValue;
	}

	tm tmObj = {0};
	sscanf_s(pszValue,"%4d-%2d-%2d %2d:%2d:%2d",
		&tmObj.tm_year,&tmObj.tm_mon,&tmObj.tm_mday,
		&tmObj.tm_hour,&tmObj.tm_min,&tmObj.tm_sec);
	tmObj.tm_year -= 1900;
	tmObj.tm_mon -= 1;
	tmObj.tm_isdst = -1;

	return mktime(&tmObj);
}

/*
inline __int64	CDBRowset::GetI64Field(int nField, int nNullValue=0)
{
	const char* pszValue = m_CurRow[nField];
	if (NULL == pszValue)
	{
		return nNullValue;
	}
	__int64 i64Ret = 0L;
	sscanf(pszValue,"%lld",&i64Ret);
	return i64Ret;
}
*/
inline int CDBRowset::GetIntField(int nField, int nNullValue/*=0*/)
{
	const char* pszValue = m_CurRow[nField];
	if (NULL == pszValue)
	{
		return nNullValue;
	}

	return strtol(pszValue,0,10);
}

int CDBRowset::GetIntField(const char* szField, int nNullValue/*=0*/)
{
	int nField = GetFieldIndex(szField);
	if (nField < 0)
	{
		printf("Error:[%s] is bad field\n",szField);
		return nNullValue;
	}
	return GetIntField(nField,nNullValue);
}

inline unsigned int CDBRowset::GetUintField(int nField, int nNullValue/*=0*/)
{
	const char* pszValue = m_CurRow[nField];
	if (NULL == pszValue)
	{
		return nNullValue;
	}

	return strtoul(pszValue,0,10);
}

unsigned int CDBRowset::GetUintField(const char* szField, int nNullValue/*=0*/)
{
	int nField = GetFieldIndex(szField);
	if (nField < 0)
	{
		printf("Error:[%s] is bad field\n",szField);
		return nNullValue;
	}
	return GetUintField(nField,nNullValue);
}

inline double CDBRowset::GetFloatField(int nField, double fNullValue/*=0.0*/)
{
	const char* pszValue = m_CurRow[nField];
	if (NULL == pszValue)
	{
		return fNullValue;
	}

	return atof(pszValue);
}

double CDBRowset::GetFloatField(const char* szField, double fNullValue/*=0.0*/)
{
	int nField = GetFieldIndex(szField);
	if (nField < 0)
	{	
		printf("Error:[%s] is bad field\n",szField);
		return fNullValue;
	}
	return GetFloatField(nField,fNullValue);
}

/*
inline const unsigned char* CDBRowset::GetBlobField(int nField, int& nLen)
{
	return 0;
}

const unsigned char* CDBRowset::GetBlobField(const char* szField, int& nLen)
{
	int nField = GetFieldIndex(szField);
	if (nField < 0)
	{
		return 0;
	}

	return 0;
}
*/

BOOL CDBRowset::ClearResult(void)
{
	for (int i=0; i<MAX_RESULT;i++)
	{
		MYSQL_RES* pResult = NULL;	
		pResult = m_arrayResult[i];
		if (pResult != NULL)
		{
			mysql_free_result(pResult);
			pResult =  NULL;
		}

		m_arrayResult[i] = NULL;
	}

	return TRUE;
}

