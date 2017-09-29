#pragma once
#include "PiTypeDef.h"
#include <Windows.h>
typedef DWORD (*ProcThreadFun)(void* paramR) ;	//�̻߳ص�����

Pi_NameSpace_Begin
//֪ͨ����
class CThreadLight;

enum NotifyType
{
    type_single_file			= 0,
    type_all_file_complete
};

struct ThreadParam
{
	CThreadLight* pThread;

	ProcThreadFun	pFun;
	void*			pOjb;
	void*			paramR;			//�Զ��庯���Ĳ���
	tstring			strThreadName;
	ThreadParam()
	{
		pThread = NULL;
		pFun		= NULL;
		pOjb		= NULL;
		paramR		= 0;
	}
};

class NotifyObj
{
public:
    NotifyType	nType;
    void*	pAdd;

    NotifyObj(NotifyType nTypeR, void* pAddR)
    {
        nType = nTypeR;
        pAdd = pAddR;
    }
};

Pi_NameSpace_End