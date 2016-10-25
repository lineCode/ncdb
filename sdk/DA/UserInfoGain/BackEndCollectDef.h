#pragma once
#include <map>
#include "PiTypeDef.h"
using namespace std;
typedef time_t     TimeType;

//用户信息数据职责
 enum ENUM_UD_DATA_DUTY
 {
     unKnown = 0,
     da_btn_use_my_assist,               
     da_btn_use_my_assist_protect_works, //作品保护按钮
     ps_use_times,
     ps_use_time_duration,
     sai_use_times,
     sai_use_time_duration
 };
// 
// enum ENUM_UD_TIMES_TYPE
// {
//     tt_ps = 0,
//     tt_csa,
//     tt_DA,
//     tt_da_btn_record,
//     tt_sai,
// };
// 
// enum ENUM_UD_TIME_DURATION_TYPE
// {
//     td_ps = 0,
//     td_csa,
//     td_DA
// };

struct TimeDuration
{
    bool        bExistPre;  //上次是否存在
    TimeType timeTotal;     //总存在时间
    TimeType timeLast;      //上一次的时间, 
    
};



typedef map<ENUM_UD_DATA_DUTY, UNINT> LST_USE_COUNT;
typedef map<ENUM_UD_DATA_DUTY, TimeDuration> LST_TIME_DURATION;
