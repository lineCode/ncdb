#include "StdAfx.h"
#include "PiKeyBoard.h"


CPiKeyBoard::CPiKeyBoard(void)
{
}


CPiKeyBoard::~CPiKeyBoard(void)
{
}

bool CPiKeyBoard::IsKeyPush( int nKeyCode )
{
    SHORT nRet = ::GetKeyState(nKeyCode);
    //UINT nHighBit = nRet >> ( 2<<(16-2) );
    UINT nHighBit = nRet & ( 2<<(16-2) );
    return nHighBit > 0;
}
