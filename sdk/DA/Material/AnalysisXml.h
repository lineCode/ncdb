#pragma once
#include "DBDefined.h"
#include <vector>
class CAnalysisXml
{
public:
	CAnalysisXml();
	virtual ~CAnalysisXml();
	BOOL AnalysisAndInstall(const CString dbPath, const CString materialPath);
protected:
private:
	void AnalysisCatalog(MaterialModifierInfo &mtInfo, CatalogMetaDataInfo &cmInfo, const CString materialPath);
	void AnalysisInfo(std::vector <std::string> &vectorTag, const CString materialPath);
	void AnalysisLayerData(std::vector <std::string> &vecorSystemTag, std::string &systemFolder, const CString materialPath);
};