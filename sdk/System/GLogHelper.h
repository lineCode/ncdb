#pragma once
class CGLogHelper
{
public:
	CGLogHelper(void);
	~CGLogHelper(void);
public:
	bool Init();
	bool SetInstallFailureWriter();
	virtual bool InitParam();
	/********************************************************************
		fun:	进行初始化日志操作,
		param:	
		ret:	成功初始化返回true, 不初始化则返回false
	********************************************************************/
	virtual bool IsWriteLog();
	bool		m_bHasInit;		//日志是否已初始化
};


void FuncFailureWriter(const char* data, int size);
void FuncFa33();
