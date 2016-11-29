
#include "UIlib.h"
#include "PiTypeDef.h"
/*
#include <list>
#include <vector>*/

using namespace DuiLib;

#ifndef M_LINK_DUILIB
#ifdef _DEBUG
#   ifdef _UNICODE
//#       pragma comment(lib, "DuiLib_ud.lib")
#       pragma comment(lib, "DuiLib_d.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
//#       pragma comment(lib, "DuiLib_u.lib")
#		pragma comment(lib, "DuiLib.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif
#endif

#define MAX max
#define MIN min

//消息定义
#define UM_CHILD_MOUSE_ENTER	WM_USER + 200


typedef std::list<DuiLib::CControlUI*> LST_CONTROL;
typedef std::vector<DuiLib::CControlUI*> ARR_CONTROL;
typedef std::vector<DuiLib::CDuiString> ARR_CONTROL_NAME;
