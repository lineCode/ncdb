#include "StdAfx.h"
#include "FileOperAssist.h"
#include "..\..\..\..\sdk\UCommon.h"
#include <assert.h>


CFileOperAssist::CFileOperAssist(void)
{
}


CFileOperAssist::~CFileOperAssist(void)
{
}

bool CFileOperAssist::AddOperator( ENUM_OPERATOR op, const tstring& strPath )
{
    //操作加入列表
    return true;
}

bool CFileOperAssist::AddOperator( ENUM_OPERATOR op, const LST_STRING& strPath )
{
    return true;
}

bool CFileOperAssist::AddOperator( const LST_OPER& lstOper )
{
    m_lstOper = lstOper;
    return true;
}

bool CFileOperAssist::BeginTrans()
{
    //清除列表, 
    m_lstOper.clear();
    m_lstRollBack.clear();
    return true;
}

bool CFileOperAssist::EndTransDo()
{
    bool bDoSuc = true;
    bool bRobackSuc = true;
    //事务执行所有操作
    FOR_EACH_FILE_OPER(m_lstOper, it)
    {
        //一个操作完成后添加到回滚列表, 当有一个失败时, 执行回滚列表(失败处理), 所有都完成后, 清空回滚列表
        tagFILE_OPER* pOper = *it;
        switch (pOper->oper)
        {
        case operator_rename:
            if(CFileUtil::Rename(pOper->strSrc.c_str(), pOper->strDist.c_str()))
            {
                break;
            }
            bDoSuc = false;
            if (!RoBack())
            {
                /*TODO:标识 操作失败,回滚成功 */
                bRobackSuc = false;
                return bDoSuc;
            }
        	return false;
            break;
        case operator_delete:
            if(CFileUtil::DeleteFile(CString(pOper->strSrc.c_str())))
            {
                break;
            }
            bDoSuc = false;
            if (!RoBack())
            {
                /*TODO:标识 操作失败,回滚成功 */
                bRobackSuc = false;
                return bDoSuc;
            }
            return false;
            break;
        default:
            break;
        }
        AddRobackTask(pOper);
    }
    return bDoSuc;
}

bool CFileOperAssist::RoBack()
{
    FOR_EACH_FILE_OPER(m_lstRollBack, it)
    {
        //一个操作完成后添加到回滚列表, 当有一个失败时, 执行回滚列表(失败处理), 所有都完成后, 清空回滚列表
        tagFILE_OPER* pOper = *it;
        switch (pOper->oper)
        {
        case operator_rename:
            if(!CFileUtil::Rename(pOper->strSrc.c_str(), pOper->strDist.c_str()))
            {
                return false;
            }
            break;
        case operator_delete:
            assert(0);
            break;
        default:
            break;
        }
    }
    return true;
}

bool CFileOperAssist::AddRobackTask( tagFILE_OPER* pOper )
{
    if (operator_delete == pOper->oper)
    {
        //暂时不支持删除操作的回滚
        return false;
    }
    tagFILE_OPER* pFile = new tagFILE_OPER;
    *pFile = *pOper;
    pFile->GetRobackOper();
    m_lstRollBack.push_front(pFile);
    return true;
}
