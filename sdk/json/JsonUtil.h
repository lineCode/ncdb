#pragma once
#include "piTypeDef.h"
#include "JsonHeader.h"
#include "StrCoding.h"

class CJsonUtil
{
public:
    CJsonUtil(void);
    ~CJsonUtil(void);
public:
    /************************************************************************
        fun:    获取跟对象的子节点
        remark:
        ret:   
    ************************************************************************/
    static bool GetChild(const tstring& strJson, tcpchar szObj, int& nValue);
    static bool GetChild(const tstring& strJson, tcpchar szObj, tstring& strValue);
    /************************************************************************
        fun:    获取子对象
        remark:
        ret:   
    ************************************************************************/
    static bool GetChild(const tstring& strJson, tcpchar szObj, Json::Value& root);
    static bool GetChild(const tstring& strJson, Json::Value& root);

	static bool GetChild(Json::Value& root, tcpchar szObj, tstring& strValue);
	static bool GetChild(Json::Value& root, tcpchar szObj, int& strValue);
	static bool GetChild(Json::Value& root, tcpchar szObj, float& strValue);
	static bool GetChild(Json::Value& root, tcpchar szObj, double& strValue);
	static bool GetChildFirst( StringCRef strJson, Json::Value& jsDeal );
	static bool GetChildFirst( Json::Value& root, Json::Value& jsDeal );
	static tstring GetChildKeyFirst( Json::Value& root);
	static bool GetArrayFirst( Json::Value& vCt, Json::Value& vTemp );
	template<class T>
	static bool GetArrayString( Json::Value& jsArray, T& lstOut );
protected:
    
};

template<class T>
bool CJsonUtil::GetArrayString( Json::Value& jsArray, T& lstOut )
{
	for (UInt i = 0; i < jsArray.size(); ++i)
	{
		lstOut.push_back(s2t(jsArray[i].asString()));
	}
	return true;

}

