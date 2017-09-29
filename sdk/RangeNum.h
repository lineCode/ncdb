#pragma once
#include <string>
#include <assert.h>
/*------------------------------------------------------------
	file:范围数字模块	
------------------------------------------------------------*/
using std::wstring;
enum ENUM_COMPARE
{
	ec_begin			= 1,
	ec_big				= 1,
	ec_big_equal			,
	ec_equal				,
	ec_small_equal			,
	ec_small				,
	ec_end				= ec_small
};

template<class T>
class CRangeNum
{
	typedef CRangeNum<T>	ClassType;
public:
	
	CRangeNum();
	CRangeNum(const T& val, ENUM_COMPARE ec);
	~CRangeNum(void);
public:
	bool operator ==(const T& val) const;
	bool operator ==(const ClassType& ct) const;
	bool operator >(const T& val) const;
	bool operator >(const CRangeNum<T>& val) const; 
	bool operator <(const CRangeNum<T>& val) const; 
	wstring Str() const; 
	ClassType operator +=(const T& val)
	{
		m_nVal += val;
		return *this;
	}
	T GetVal()const {return m_nVal;}
	void SetData(const T& val, ENUM_COMPARE ec);
private:
	T						m_nVal;
	ENUM_COMPARE			m_ec;
};

template<class T>
wstring CRangeNum<T>::Str() const
{
	wstring strOut;
	strOut.resize(10);
	switch (m_ec)
	{
	case ec_big:
		swprintf(&strOut.at(0), 10, _T(">%d"), m_nVal);
		break;
	case ec_big_equal:
		swprintf(&strOut.at(0), 10, _T(">=%d"), m_nVal);
		break;
	case ec_equal:
		swprintf(&strOut.at(0), 10, _T("=%d"), m_nVal);
		break;
	case ec_small_equal:
		swprintf(&strOut.at(0), 10, _T("<=%d"), m_nVal);
		break;
	case ec_small:
		swprintf(&strOut.at(0), 10, _T("<%d"), m_nVal);
		break;
	default:
		break;
	}
	return strOut;
}

template<class T>
bool CRangeNum<T>::operator==(const  ClassType& ct ) const
{
	return m_nVal == ct.m_nVal 
		&& m_ec	== ct.m_ec;
}

template<class T>
bool CRangeNum<T>::operator==(const T& val) const
{
	bool bEqual = false;
	switch (m_ec)
	{
	case ec_big:
		bEqual = (val > m_nVal);
		break;
	case ec_big_equal:
		bEqual = (val >= m_nVal);
		break;
	case ec_equal:
		bEqual = (val == m_nVal);
		break;
	case ec_small_equal:
		bEqual = (val <= m_nVal);
		break;
	case ec_small:
		bEqual = (val < m_nVal);
		break;
	default:
		break;
	}
	return bEqual;
}

template<class T>
bool CRangeNum<T>::operator>( const CRangeNum<T>& rNum ) const
{
	if (m_ec != rNum.m_ec)
	{
		assert(0 && "暂不支持不同范围的比较");
	}
	if (m_nVal > rNum.m_nVal)
	{
		return true;
	}
	else if(m_nVal == rNum.m_nVal
		&& m_ec > rNum.m_ec)
	{
		return true;
	}
	return false;
}

template<class T>
bool CRangeNum<T>::operator>( const T& val ) const
{
	return val > m_nVal;
}


template<class T>
bool CRangeNum<T>::operator<( const CRangeNum<T>& rNum ) const
{
	//TODO:能表示的范围里最小的数的大小比较
	if (m_nVal < rNum.m_nVal)
	{
		return true;
	}
	else if(m_nVal == rNum.m_nVal
		&& m_ec < rNum.m_ec)
	{
		return true;
	}
	return false;
}


template<class T>
CRangeNum<T>::CRangeNum()
	:m_nVal(0)
	,m_ec(ec_big)
{

}

template<class T>
CRangeNum<T>::CRangeNum( const T& val, ENUM_COMPARE ec )
	:m_nVal(val)
	,m_ec(ec)
{

}


template<class T>
CRangeNum<T>::~CRangeNum( void )
{

}

template<class T>
void CRangeNum<T>::SetData( const T& val, ENUM_COMPARE ec )
{
	m_nVal = val;
	m_ec = ec;
}
