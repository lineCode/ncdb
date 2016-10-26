#pragma once
#include "..\PiTypeDef.h"
using std::string;
/************************************************************************/
/*	fun: 获取一个拖放文件路径, 窗口需要支持拖放:通过DragAcceptFiles设置
/************************************************************************/
tstring GetOneDragFilePath(const HDROP& hd);