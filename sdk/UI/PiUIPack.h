#pragma once
#include "PiTypeDef.h"

class CPiUIPack
{
public:
	CPiUIPack(void);
	~CPiUIPack(void);
public:
	static POINT GetCursorPos();
	static bool OpenUrl( tcpchar szUrl );
	static bool OpenBin( tcpchar szBin , tcpchar szParam);
	static bool MakeShortCut(tcpchar szLinkPath, tcpchar szFilePath, tcpchar szParam);
	static bool ChangeShortCutIcon( StringCRef strLinkPath, StringCRef strIconPath );
	static bool Jpg2Ico( tcpchar szPicPath, tcpchar szPathIcon );
	static bool IsAeroStyle();
	static bool SaveDesktopScreenshot( tcpchar szPath );
};
