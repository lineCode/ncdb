#pragma once
#include "FileOperAssistDef.h"
#include "PiTypeDef.h"
class CFileOperAssist
{
public:
    CFileOperAssist(void);
    ~CFileOperAssist(void);
public:
    bool BeginTrans();
    bool EndTransDo();

    bool AddOperator(ENUM_OPERATOR op, const tstring& strPath);
    bool AddOperator(ENUM_OPERATOR op, const LST_STRING& strPath);
    bool AddOperator(const LST_OPER& lstOper);
    bool DeleteOperator(ENUM_OPERATOR op);
    bool AddRobackTask( tagFILE_OPER* pOper );
    bool RoBack();
private:
    LST_OPER            m_lstOper;
    LST_OPER            m_lstRollBack;
};

