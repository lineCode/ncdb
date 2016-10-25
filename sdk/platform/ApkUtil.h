#pragma once


class CApkUtil
{
public:
	struct tagAPK_INFO
	{
		tagAPK_INFO()
			: nCode(-1)
		{

		}
		int		nCode;
		tstring filename;
		tstring packname;
		tstring logoPath;
		tstring version;
		tstring acvivity;
	};

public:
	CApkUtil(void);
	~CApkUtil(void);
	static bool GetApkInfo( tcpchar szApkPath, tagAPK_INFO& tagAI );
	static bool GetApkInfoAndPic( tcpchar szApkPath, tagAPK_INFO& tagAI, tcpchar szPicDir);
};
