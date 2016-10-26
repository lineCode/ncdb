#pragma once
#include <Windows.h>
#include <map>
#include <string>
class CMapLight;
using namespace std;

/************************************************************************/
/*	fun: 图像列表,用于查找 指定后缀名的图标,是否已存在,                                                                   
	ret: 如果存在,返回在CImageList里的位置>=0,反则返回-1
/************************************************************************/
typedef map<string, DWORD> ilMap;
class CImageMap
{
public:
	CImageMap(void);
	~CImageMap(void);

	bool Init();
	/************************************************************************/
	/*	fun: 获取指定文件类型对应的图标所在的索引,不存在则会添加并返回添加后的index                                                                     */
	/************************************************************************/
	DWORD GetIndex(const char*);
	ilMap::iterator Find(const char* suffix);
	bool Insert(const char* key, DWORD idxImage );
	DWORD GetNewIndex();
	CImageList* GetImageList();
	void SetImageList(CImageList* );
private:
	CImageList* m_pIL;
	CMapLight* m_pMap;
};
