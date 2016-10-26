#pragma once
#include "PAMessageDef.h"
enum ENUM_MSG_TYPE
{
    msg_normal = 0,
    msg_news            ,
    msg_tasks_diary            ,
};


#ifndef TEST_UNITS
    #define  NUM_CIRCLE_NEXT               5 * 60 * 1000    //单位, 分钟
#else
    #define  NUM_CIRCLE_NEXT               10 * 1000    //单位, 秒
#endif

