#pragma once
class CPiKeyBoard
{
public:
    CPiKeyBoard(void);
    virtual ~CPiKeyBoard(void);
public:
/************************************************************************
        fun:	判断指定的按键是否处于按下状态
        param:  
        ret:    true: 指定按键为按下状态
        remark:  虚拟按键, 如 VK_SHIFT
/************************************************************************/
    static bool IsKeyPush( int nKeyCode );
};

