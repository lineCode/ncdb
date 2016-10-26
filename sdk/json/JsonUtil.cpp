#include "StdAfx.h"
#include "JsonUtil.h"

using namespace Json;
CJsonUtil::CJsonUtil(void)
{
}


CJsonUtil::~CJsonUtil(void)
{
}

bool CJsonUtil::GetChild( const tstring& strJson, Json::Value& root )
{
    string strWebA = w2s(strJson);
    Json::Reader json;
    if( ! json.parse(strWebA.c_str(), root, false))
    {
        return false;
    }
    return true;
}

bool CJsonUtil::GetChild( const tstring& strJson, tcpchar szObj, tstring& strValue )
{
    Json::Value root;
    if(!GetChild(strJson, root))
    {
        return false;
    }

    strValue = s2w(root[w2s(tstring(szObj)).c_str()].asString());
    return true;
}

bool CJsonUtil::GetChild( const tstring& strJson, tcpchar szObj, int& nValue )
{
    Json::Value root;
    if(!GetChild(strJson, root))
    {
        return false;
    }

    nValue = root[w2s(tstring(szObj)).c_str()].asInt();
    return true;
}

bool CJsonUtil::GetChild( const tstring& strJson, tcpchar szObj, Json::Value& body )
{
    Json::Value root;
    if(!GetChild(strJson, root))
    {
        return false;
    }
    body = root[w2s(tstring(szObj)).c_str()];
    return body.empty() == false;
}

bool CJsonUtil::GetChild( Json::Value& root, tcpchar szObj, tstring& strValue )
{
	if (root.isNull())
	{
		return false;
	}
	Json::Value& vT = root[tp2s(szObj)];
	if(vT.isNull())
	{
		return false;
	}
	strValue = s2t(vT.asString());
	return true;
}

bool CJsonUtil::GetChild( Json::Value& root, tcpchar szObj, int& strValue )
{
	strValue = root[pw2psSmart(szObj)].asInt();
	return true;
}

bool CJsonUtil::GetChild( Json::Value& root, tcpchar szObj, float& strValue )
{
	strValue = (float)root[pw2psSmart(szObj)].asDouble();
	return true;	
}

bool CJsonUtil::GetChild( Json::Value& root, tcpchar szObj, double& strValue )
{
	strValue = root[pw2psSmart(szObj)].asDouble();
	return true;	
}

bool CJsonUtil::GetChildFirst( StringCRef strJson, Json::Value& jsDeal )
{
	GetChild(strJson, jsDeal);
	Value::Members mem;
	mem = jsDeal.getMemberNames();
	if (!mem.size())
	{
		return false;
	}
	jsDeal = jsDeal[mem[0]];
	return true;
}

bool CJsonUtil::GetChildFirst( Json::Value& root, Json::Value& jsDeal )
{
	Value::Members mem;
	mem = root.getMemberNames();
	if (!mem.size())
	{
		return false;
	}
	jsDeal = root[mem[0]];
	return true;
}

tstring CJsonUtil::GetChildKeyFirst( Json::Value& root )
{
	Value::Members mem;
	mem = root.getMemberNames();
	if (!mem.size())
	{
		return false;
	}
	return s2w(mem[0]);
}

bool CJsonUtil::GetArrayFirst( Json::Value& vCt, Json::Value& vTemp )
{
	if(vCt.isNull() 
		|| !vCt.isArray()
		|| !vCt.size())
	{
		return false;
	}
	vTemp = vCt[(UInt)0];
	return true;
}


