/***********************************************************************
Copyright (c), 2012, Unicorn Tech. Co., Ltd.

File name   : k_filepath.cpp
Description : 文件路径类
Other       : 
Version     : 1.0
Author      : xuemingzhi
Date        : 2014/06/23
-----------------------------------------------------------------------
History 1 
Date   : 2014/07/18
Author : xuemingzhi
Modification : 运算符重载+=时，如果是后缀名，无需斜杠分隔符；
***********************************************************************/

#include "stdafx.h"
#include "k_filepath.h"
#include "MD5.h"

// --------------------------------------------------------------------------
// Function : 构造函数&析构函数
// Thinking : 
// --------------------------------------------------------------------------
KFilePath::KFilePath(LPCTSTR inSzPath /*= NULL*/)
{
  if(inSzPath)
    path_ = inSzPath;
}
KFilePath::KFilePath(ConstSTLStringRef inStrPath)
{
  path_ = inStrPath;
}

KFilePath::~KFilePath()
{

}

// --------------------------------------------------------------------------
// Function : GetFileDirectory
// Thinking : 
// --------------------------------------------------------------------------
STLString KFilePath::GetFileDirectory(ConstSTLStringRef inPath)
{
  STLString legalPath = inPath;
  _unified_format(legalPath);
  size_t pos = legalPath.find_last_of(_T("\\"));
  if(-1 == pos)
    return STLString();
  return legalPath.substr(0,pos+1);
}
// --------------------------------------------------------------------------
// Function : GetFileName
// Thinking : 
// --------------------------------------------------------------------------
STLString KFilePath::GetFileName(ConstSTLStringRef inPath,bool inHasExtend)
{
  STLString legalPath = inPath;
  _unified_format(legalPath);
  size_t pos = legalPath.find_last_of(_T("\\"));
  if(-1 == pos)
    return legalPath;
  STLString filename = legalPath.substr(pos+1,inPath.size());
  if( !inHasExtend ) {
    pos = filename.find_last_of(_T("."));
    if(-1 == pos)
      return filename;
    else
      return filename.substr(0,pos);
  }
  return filename;
}
// --------------------------------------------------------------------------
// Function : GetFileExten
// Thinking : 
// --------------------------------------------------------------------------
STLString KFilePath::GetFileExtend(ConstSTLStringRef inPath)
{
  STLString legalPath = inPath;
  _unified_format(legalPath);
  STLString filename = GetFileName(legalPath,true);
  size_t pos = filename.find_last_of(_T("."));
  if(-1 == pos)
    return STLString();
  return filename.substr(pos,filename.size());
}
// --------------------------------------------------------------------------
// Function : IsExist
// Thinking : 
// --------------------------------------------------------------------------
bool KFilePath::IsExist(ConstSTLStringRef inPath)
{
  return (-1 == GetFileAttributes(inPath.c_str()) ? false : true);
}

bool KFilePath::IsDirectory(ConstSTLStringRef inPath)
{
  STLString file_extend = GetFileExtend(inPath);
  return file_extend.empty();
}
// --------------------------------------------------------------------------
// Function : GetFileDirectory、GetFileName、GetFileExtend、IsExist
// Thinking : 
// --------------------------------------------------------------------------
STLString KFilePath::GetFileDirectory()
{
  return GetFileDirectory(path_);
}
STLString KFilePath::GetFileName(bool inHasExtend)
{
  return GetFileName(path_,inHasExtend);
}
STLString KFilePath::GetFileExtend()
{
  return GetFileExtend(path_);
}
bool KFilePath::IsExist()
{
  return IsExist(path_);
}
bool KFilePath::IsDirectory()
{
  return IsDirectory(path_);
}
// --------------------------------------------------------------------------
// Function : GetFileDirectory、GetFileName、GetFileExtend、IsExist
// Thinking : 
// --------------------------------------------------------------------------
KFilePath& KFilePath::operator=(ConstSTLStringRef inPath)
{
  path_ = inPath;
  return *this;
}
KFilePath  KFilePath::operator+(ConstSTLStringRef inPath)
{
  KFilePath newObj;
  bool isNeedAddSip = true;

  // 判断当前路径是否以斜杠结尾
  if( !path_.empty() ) {
    TCHAR endc = path_.at(path_.size()-1);
    if(endc == _T('\\')) {
      isNeedAddSip = false;
    }
  }

  if(isNeedAddSip)
    newObj = path_ + _T("\\") + inPath;
  else
    newObj = path_ + inPath;

  return newObj;
}
KFilePath& KFilePath::operator+=(ConstSTLStringRef inPath)
{
  // 后缀名特殊处理
  if(!inPath.empty()) {
    if(inPath.at(0) == _T('.')) {
      path_ += inPath;
    }
    else {
      bool isNeedAddSip = true;

      // 判断当前路径是否以斜杠结尾
      if( !path_.empty() ) {
        TCHAR endc = path_.at(path_.size()-1);
        if(endc == _T('\\')) {
          isNeedAddSip = false;
        }
      }

      if(isNeedAddSip)
        path_ = path_ + _T("\\") + inPath;
      else
        path_ = path_ + inPath;
    }
  }
  return *this;
}

// --------------------------------------------------------------------------
// Function : _unified_format
// Purpose  : 将路径里所有'/'转换为'\'，统一格式（内部函数）
// --------------------------------------------------------------------------
void KFilePath::_unified_format(STLString& inPath)
{
  for(size_t i = 0; i < inPath.size(); ++i) {
    TCHAR& c = inPath.at(0);
    if(_T('/') == c) {
      c = _T('\\');
    }
  }
}
// --------------------------------------------------------------------------
// Function : GetMD5
// Purpose  : 获取MD5
// --------------------------------------------------------------------------
STLString KFilePath::GetMD5()
{
  string path = path_.ToAnscii();
  FILE* stream = fopen(path.c_str(),"rb");
  if(NULL == stream) return STLString();
  int iRet = fseek(stream,0,SEEK_END);
  if(-1 == iRet) {
    fclose(stream);
    return STLString();
  }
  long filesize = ftell(stream);
  iRet = fseek(stream,0,SEEK_SET);
  if(-1 == iRet) {
    fclose(stream);
    return STLString();
  }
  unsigned char* buff = new unsigned char[filesize+1];
  memset(buff,0,(filesize+1) * sizeof(unsigned char));
  fread(buff,sizeof(unsigned char),filesize * sizeof(unsigned char),stream);
  fclose(stream);
  TCHAR md5buff[70] = {0};
  MD5String((unsigned char*)buff,filesize,md5buff);
  STLString md5 = md5buff;
  delete[] buff;
  return md5;
}
// --------------------------------------------------------------------------
// Function : GetCurrentDir等
// Purpose  : 快捷路径
// --------------------------------------------------------------------------
STLString KFilePath::GetCurrentDir()
{
  TCHAR buff[MAX_PATH] = {0};
  ::GetCurrentDirectory(MAX_PATH,buff);
  return buff;
}

STLString KFilePath::GetTempDir()
{
  TCHAR buff[MAX_PATH] = {0};
  GetTempPath(MAX_PATH,buff);
  return buff;
}
// --------------------------------------------------------------------------
// Function : IsValidFileName
// Purpose  : 检测文件名是否符合window命名规范
// Input    ：filename
// Output   ：无
// Return   ：bool -- true:有效；false:无效；
// --------------------------------------------------------------------------
bool KFilePath::IsValidFileName(ConstSTLStringRef inFileName)
{
  if(inFileName.empty())
    return false;
  size_t count = 0;
  for(size_t i = 0; i < inFileName.size(); i++) {
    TCHAR _tc = inFileName.at(i);

	if( _T(' ') == _tc )
		++count;
	
    if(  _T('\\') == _tc 
      || _T('/') == _tc 
      || _T(':') == _tc
      || _T('*') == _tc 
      || _T('?') == _tc 
      || _T('\"') == _tc 
      || _T('<') == _tc
      || _T('>') == _tc 
      || _T('|') == _tc) 
    {
      return false;
    }
  }
  if( count == inFileName.size() )
	  return false;
  return true;
}