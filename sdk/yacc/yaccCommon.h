#pragma once
#include <stddef.h>
/************************************************************************
    fun:    
    author: 
    create time:   
    update time:   
    remark: 影响的宏 
        DEBUG_INFO: 输出win32调试信息
************************************************************************/
void DebugInfo(const char* s);

void DebugInfo(const char* s, int nValue);

void DebugInfo(const wchar_t* s);

void DebugInfo(const wchar_t* s, int nValue);
