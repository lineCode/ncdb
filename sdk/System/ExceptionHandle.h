#pragma once
#define SEH_BEGIN __try {
#define SEH_EXCEPT_BEGIN(stage) } __except(stage){
#define SEH_EXCEPT_END }
#define SEH_FINAL_BEGIN } __finally {
#define SEH_FINAL_END } 

