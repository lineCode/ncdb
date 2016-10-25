#pragma once
#include "define_gnl.h"
#include "PiTypeDef.h"
#include "Stlmath.h"
#include "Math/mathUnit.h"
#include "logopr.h"

/************************************************************************
    fun: 日志类, 可以根据大小或时间等条件 自动分成多个文件
    remark: 
    author: liwanliang
    create time: 2013-7-31 20:13
    modify time: 2013-7-31 20:13
************************************************************************/
Pi_NameSpace_Begin
class CLogSpilit :
    public CLogOpr
{
public:
    struct INIT_PARAM
    {
        bool bSplitEveryDay;    //是否一天一个log
        UNLONG nMaxSize;        //一个文件大小上限
        INIT_PARAM()
        {
            bSplitEveryDay = true;
            nMaxSize        = NUM_FILE_SIZE_1M * 30;   //30M
        }
    };

public:
    CLogSpilit(void);
    ~CLogSpilit(void);
public:
    bool         Init();
    bool         UnInit();
    bool        SetParam(const INIT_PARAM& iParam);
	bool		OpenTodayLog(tcpchar szPath );

    /************************************************************************/
    /*  fun: 写log, 字符串之前有添加时间前缀
    /************************************************************************/
    DWORD	    WriteLog(tcpchar data);
    DWORD       WriteLog(StringCRef data);
    /************************************************************************/
    /*  fun: 写log, 不添加时间前缀
    /************************************************************************/
    DWORD       WriteLogPure(tcpchar data);
    DWORD       WriteLogPure(StringCRef data);
protected:
    void        PartLogFile();

private:
    CLogSpilit(CLogSpilit&);
    tstring			m_strDate;//保存date, 用来判断是否要开启一个新的Log
    INIT_PARAM      m_iParam;
};


Pi_NameSpace_End
