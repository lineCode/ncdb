#pragma once
#include <string>

//定义MaterialModifier表的结构体
typedef struct _MaterialModifierInfo
{
	std::string materialName;
	std::string catalogUuid;
	int         catalogContentId;
	int         catalogUserId;
	std::string materialUuid;
	int         materialContentId;
	int         materialUserId;
	std::string parentUuid;
	//以下两个字段在1.3.5才有
	std::string createTime;//20140731164551
	std::string updateTime;//20140806111546
	//end
	int         pathType;
	std::string path;
	_MaterialModifierInfo()
	{
		materialName = "";
		catalogUuid = "";
		catalogContentId = 0;
		catalogUserId = 0;
		materialUuid = "";
		materialContentId = 0;
		materialUserId = 0;
		parentUuid = "";
		//以下两个字段在1.3.5才有
		createTime = "";//20140731164551
		updateTime = "";//20140806111546
		//end
		pathType = 0;
		path = "";
	}
}MaterialModifierInfo;


//定义表CatalogMetaData的结构体
typedef struct _CatalogMetaDataInfo
{
	std::string createTool;
	std::string materialKind;
	std::string lead;
	std::string useTool;
	int         readOnly;
	//以下一个字段在135才有
	int         growth;
	//end
	int         uploadable;
	int         antisocial;
	int         ideology;
	int         sexual;
	int         violence;
	int         clippy;
	int         gold;
	int         materialAlias;
	int         uploaded;
	int         downloaded;
	int         created;
	int         inCatalog;
	int         downloadNotYet;
	int         updated;
	int         editing;
	_CatalogMetaDataInfo()
	{
		createTool = "";
		materialKind = "";
		lead = "";
		useTool = "";
		readOnly = 0;;
		//以下一个字段在135才有
		growth = 0;;
		//end
		uploadable = 0;
		antisocial = 0;
		ideology = 0;
		sexual = 0;
		violence = 0;
		clippy = 0;
		gold = 0;
		materialAlias = 0;
		uploaded = 0;
		downloaded = 0;
		created = 0;
		inCatalog = 0;
		downloadNotYet = 0;
		updated = 0;
		editing = 0;
	}
}CatalogMetaDataInfo;


enum INSERTTAGTYPE 
{
	MAINTAG = 1,  //Tag表
	FOLDERTAG,    //FolderTag表
	SYSTEMTAG,    //SystemTag表
};

