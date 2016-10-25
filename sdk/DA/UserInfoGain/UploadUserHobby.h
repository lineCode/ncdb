#pragma once
#include "PiTypeDef.h"
#include "ResCommon.h"


class CUploadUtil;

class CUploadUserHobby
{
public:
    CUploadUserHobby(void);
    ~CUploadUserHobby(void);
public:
    bool Init();
    bool UploadUserHobbyData();
private:
    bool IsCanDo();
    bool IsTimeUp();
    bool UploadDataFile(tcpchar szFile);
    static bool DealFileFunc(tcpchar lpFileOrPath, void* pUserData );
    bool UploadAllHobbyData();
    bool IsFileHobby( tcpchar lpFileOrPath );
    bool DeleteFile( tcpchar lpFileOrPath );
    /****************************************  Ù–‘ ****************************************/
private:
    auto_ptr<CUploadUtil>                m_pUploadUtil;
    tstring                     m_strHobbyFileDir;
};



class CUploadUtil
{
public:
  bool Upload(tcpchar szFile);
};