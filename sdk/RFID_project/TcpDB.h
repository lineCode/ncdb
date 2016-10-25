#pragma once
#include "socktcp.h"

class CTcpDB :
	public CSockTcp
{
public:
	CTcpDB(SOCKET sock = 0, void* pAdd = 0);
	~CTcpDB(void);

    virtual bool	Listen();

	virtual SOCKET Accept(const char* ip  = 0 , u_short port  = 0 );
	virtual DWORD	Recv(char* &pBuf);
	virtual bool	DoDealDataRecv(void* pBuf, DWORD dwRecv);
	virtual CSock* Clone();
};
