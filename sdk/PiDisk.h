#pragma once
#include "PiTypeDef.h"
class CPiDisk
{
public:
    CPiDisk(void);
    ~CPiDisk(void);
    static LST_STRING GetValidDrive();
    static MAP_STRING GetDriveMapping( LST_STRING& lstDrive );
    bool GetValidDriveMapping();
    tstring GetDosPathFromMapping( tcpchar szMapping );
    bool Init();
	/*
	* Function:GetAppVersion
	* Description:??®®?®Æ|®Æ?3®¨D®∞¶Ã?°„?°¿?o?
	* Input:strPath ???°ß3®¨D®∞¶Ã??°§??
	* Return:?°‰DD3®¶1|°§¶Ã??3®¨D®∞¶Ã?°„?°¿?o?°Í?°§°‰??°§¶Ã?????¶Ã
	*/
	static tstring GetFileVersion( tcpchar path );
private:
    MAP_STRING          m_mapDrive;
};

