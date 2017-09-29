#pragma once
#include <string>
using namespace std;

typedef struct stMysqlConn
{
	int		nConnNum;
	char	szIp[20];
	int		nPort;
	char	szDataBase[20];
	char	szUser[20];
	char	szPWD[20];
	stMysqlConn()
	{
		memset(this, 0, sizeof(*this));
	}
} STMysqlConn;

