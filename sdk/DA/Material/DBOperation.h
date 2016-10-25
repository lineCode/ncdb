#pragma once
#include "DBDefined.h"

class CDBOperation
{
public:
	CDBOperation(std::string pathStr);
	virtual ~CDBOperation();
	BOOL InsertDataIntoTagTable(INSERTTAGTYPE tableType, std::string titleStr);//将数据添加到Tag表以及Tag的关联表中
	BOOL DeleteMaterialFromDB(const std::string uuidStr, const std::string dbPath); //将素材从数据库移除
	BOOL InitCatalogMaterialDB();//初始化数据库表
    BOOL InsertDataIntoMTModifier(MaterialModifierInfo mtInfo);//插入数据到MaterialModifier表中
	BOOL InsertDataIntoMTInterpret(std::string freeWorldStr);//插入数据到MaterialInterpret表中
	BOOL InsertDataIntoCAMMetaData(CatalogMetaDataInfo cmInfo);//插入数据到CatalogMetaData表中
protected:
private:
	std::string m_dbPath;
};