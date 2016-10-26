#ifndef _MD5_H_ 
#define _MD5_H_ 
#ifndef _WINDOWS_
#include <Windows.h>
#endif
#ifndef _INC_TCHAR
#include <tchar.h>
#endif
#include "PiTypeDef.h"

void MD5String( unsigned char *pData, unsigned long dwlen, TCHAR *outString );
tstring MD5(StringCRef strSrc);
#endif //_MD5_H_ 
