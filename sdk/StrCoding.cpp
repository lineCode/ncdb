
#include "StdAfx.h"
#include "StrCoding.h"
#include <atlenc.h>
#include <assert.h>
#include "ResCommon.h"
#include "doString.h"

//这是个类strCoding (strCoding.cpp文件)

#define		SAFERELEASE(p)			if((p)){delete[] (p);(p) = NULL;}

CStrCoding::CStrCoding(void)
{
}

CStrCoding::~CStrCoding(void)
{
}
void CStrCoding::Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer)
{
    ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
    return;
}
void CStrCoding::UTF_8ToUnicode(WCHAR* pOut,char *pText)
{
    char* uchar = (char *)pOut;

    uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
    uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);

    return;
}

void CStrCoding::UnicodeToUTF_8(char* pOut,WCHAR* pText)
{
    // 注意 WCHAR高低字的顺序,低字节在前，高字节在后
    char* pchar = (char *)pText;

    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));

    return;
}
void CStrCoding::UnicodeToGB2312(char* pOut,WCHAR uData)
{
    WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(WCHAR),NULL,NULL);
    return;
}

//做为解Url使用
char CStrCoding:: CharToInt(char ch){
    if(ch>='0' && ch<='9')return (char)(ch-'0');
    if(ch>='a' && ch<='f')return (char)(ch-'a'+10);
    if(ch>='A' && ch<='F')return (char)(ch-'A'+10);
    return -1;
}
char CStrCoding::StrToBin(char *str){
    char tempWord[2];
    char chn;

    tempWord[0] = CharToInt(str[0]);                         //make the B to 11 -- 00001011
    tempWord[1] = CharToInt(str[1]);                         //make the 0 to 0 -- 00000000

    chn = (tempWord[0] << 4) | tempWord[1];                //to change the BO to 10110000

    return chn;
}


//UTF_8 转gb2312
void CStrCoding::UTF_8ToGB2312(string &pOut, char *pText, int pLen)
{
    char buf[4];
    char* rst = new char[pLen + (pLen >> 2) + 2];
    memset(buf,0,4);
    memset(rst,0,pLen + (pLen >> 2) + 2);

    int i =0;
    int j = 0;

    while(i < pLen)
    {
        if(*(pText + i) >= 0)
        {

            rst[j++] = pText[i++];
        }
        else                
        {
            WCHAR Wtemp;


            UTF_8ToUnicode(&Wtemp,pText + i);

            UnicodeToGB2312(buf,Wtemp);

            unsigned short int tmp = 0;
            tmp = rst[j] = buf[0];
            tmp = rst[j+1] = buf[1];
            tmp = rst[j+2] = buf[2];

            //newBuf[j] = Ctemp[0];
            //newBuf[j + 1] = Ctemp[1];

            i += 3;   
            j += 2;  
        }

    }
    rst[j]='\0';
    pOut = rst;
    delete []rst;
}

//GB2312 转为 UTF-8
void CStrCoding::GB2312ToUTF_8(string& pOut,char *pText, int pLen)
{
    char buf[4];
    memset(buf,0,4);

    pOut.clear();

    int i = 0;
    while(i < pLen)
    {
        //如果是英文直接复制就可以
        if( pText[i] >= 0)
        {
            char asciistr[2]={0};
            asciistr[0] = (pText[i++]);
            pOut.append(asciistr);
        }
        else
        {
            WCHAR pbuffer;
            Gb2312ToUnicode(&pbuffer,pText+i);

            UnicodeToUTF_8(buf,&pbuffer);

            pOut.append(buf);

            i += 2;
        }
    }

    return;
}
//把str编码为网页中的 GB2312 url encode ,英文不变，汉字双字节 如%3D%AE%88
string CStrCoding::UrlGB2312(char * str)
{
    string dd;
    size_t len = strlen(str);
    for (size_t i=0;i<len;i++)
    {
        if(isalnum((BYTE)str[i]))
        {
            char tempbuff[2];
            sprintf_s(tempbuff,2,"%c",str[i]);
            dd.append(tempbuff);
        }
        else if (isspace((BYTE)str[i]))
        {
            dd.append("+");
        }
        else
        {
            char tempbuff[4];
            sprintf_s(tempbuff,4,"%%%X%X",((BYTE*)str)[i] >>4,((BYTE*)str)[i] %16);
            dd.append(tempbuff);
        }

    }
    return dd;
}

//把str编码为网页中的 UTF-8 url encode ,英文不变，汉字三字节 如%3D%AE%88

string CStrCoding::UrlUtf8(char * str)
{
    string tt;
    string dd;
    GB2312ToUTF_8(tt,str,(int)strlen(str));

    size_t len=tt.length();
    for (size_t i=0;i<len;i++)
    {
        if(isalnum((BYTE)tt.at(i)))
        {
            char tempbuff[2]={0};
            sprintf_s(tempbuff,2,"%c",(BYTE)tt.at(i));
            dd.append(tempbuff);
        }
        else if (isspace((BYTE)tt.at(i)))
        {
            dd.append("+");
        }
        else
        {
            char tempbuff[4];
            sprintf_s(tempbuff,4,"%%%X%X",((BYTE)tt.at(i)) >>4,((BYTE)tt.at(i)) %16);
            dd.append(tempbuff);
        }

    }
    return dd;
}
//把url GB2312解码
string CStrCoding::UrlGB2312Decode(string str)
{
    string output="";
    char tmp[2];
    int i=0,idx=0,len=str.length();

    while(i<len){
        if(str[i]=='%'){
            tmp[0]=str[i+1];
            tmp[1]=str[i+2];
            output += StrToBin(tmp);
            i=i+3;
        }
        else if(str[i]=='+'){
            output+=' ';
            i++;
        }
        else{
            output+=str[i];
            i++;
        }
    }

    return output;
}
wstring CStrCoding::UrlUnicodeDecode( wstring str )
{
    string strA = CStrCoding::UnicodeToAnsi(str.c_str());
    strA = UrlGB2312Decode(strA);
    //Gb2312ToUnicode()
    return CStrCoding::AnsiToUnicode(strA.c_str());
}

//把url utf8解码
string CStrCoding::UrlUtf8Decode(string str)
{
    string output="";

    string temp =UrlGB2312Decode(str);//

    UTF_8ToGB2312(output,(char *)temp.data(),strlen(temp.data()));

    return output;

}

tstring CStrCoding::UrlDecode( tstring strUrl )
{
#ifdef UNICODE
    return UrlUnicodeDecode(strUrl);
#else
    return UrlGB2312Decode(strUrl);
#endif
}

tstring CStrCoding::EncodeBase64( PCVOID szData, UNINT nSize )
{
    int nSizeDist = Base64EncodeGetRequiredLength(nSize);
    NewMemAndInit(pMem, nSizeDist + 1);
    if(! Base64Encode((const BYTE*)szData, nSize, (LPSTR)pMem, &nSizeDist
		, ATL_BASE64_FLAG_NOPAD | ATL_BASE64_FLAG_NOCRLF))
    {
        return _T("");
    }
    string strDist((LPSTR)pMem);
    ClearMemArr(pMem);
    return s2t(strDist);
}

void* CStrCoding::DecodeBase64( PCSTR szData, UNINT* pnSize /*= NULL*/ )
{
    int nLenSrc = strlen(szData);
    int nSizeDist = Base64DecodeGetRequiredLength(nLenSrc);
    NewMemAndInit(pMem, nSizeDist);
    if(! Base64Decode(szData, nLenSrc, (BYTE*)pMem, &nSizeDist))
    {
        pnSize = 0;
        ClearMemArr(pMem);
        return NULL;
    }
    if (pnSize)
    {
        *pnSize = nSizeDist;
    }
    return pMem;
}

void* CStrCoding::DecodeBase64_T( tcpchar szData, UNINT* pnSize /*= NULL*/ )
{
    string strData;
#ifdef _UNICODE
    strData = UnicodeToAnsi(szData);
#else
    strData = szData;
#endif
    return DecodeBase64(strData.c_str(), pnSize);
}


tstring CStrCoding::DecodeBase64Str_T( tcpchar szData )
{
    string strData;
#ifdef _UNICODE
    strData = UnicodeToAnsi(szData);
#else
    strData = szData;
#endif
    tstring strDist;

    char* szDecode = (char*)DecodeBase64(strData.c_str(), NULL);
#ifdef _UNICODE
    strDist = AnsiToUnicode(szDecode);
#else
    strDist = szData;
#endif
    return strDist;
}




wstring CStrCoding::Utf8Base64W2Unicode( tcpchar szData )
{
    string strUtf8 = (char*)DecodeBase64_T(szData);
    wstring strUnicode = Utf8ToUnicode(strUtf8.c_str());
    return strUnicode;
}


wstring CStrCoding::Utf8ToUnicode( LPCSTR pSour )
{
    wstring strDist;
    wchar_t* pBuf = NULL;
    int nSize = MultiByteToWideChar(CP_UTF8, 0, pSour, -1, pBuf, 0);
    pBuf = new wchar_t[nSize + 1];
    memset(pBuf, 0, nSize + 1);
    if(!MultiByteToWideChar(CP_UTF8, 0, pSour, -1, pBuf, nSize))
    {
        ClearMemArr(pBuf);
        return strDist;
    }
    strDist = pBuf;
    ClearMemArr(pBuf);
    return strDist;
}


wstring CStrCoding::AnsiToUnicode( PCSTR szData )
{
    int nLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szData, -1, NULL, 0 );
    if (nLen == 0)
    {
        return NULL;
    }
    wchar_t* pResult = new wchar_t[nLen];
    MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szData, -1, pResult, nLen );
    wstring strData(pResult);
    delete [] pResult;
    return strData;
}


string CStrCoding::UnicodeToAnsi( const wchar_t* wszString )
{
    int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
    char* szAnsi = new char[ansiLen + 1];
    ::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), szAnsi, ansiLen, NULL, NULL);
    szAnsi[ansiLen] = 0;
    string strRet(szAnsi);
    delete [] szAnsi;
    return strRet;
}

string CStrCoding::Utf8ToAnsi( LPCSTR pSour )
{
    //utf8->Unicode->Ansi
    string strDist;
    wchar_t* pBuf = NULL;
    int nSize = MultiByteToWideChar(CP_UTF8, 0, pSour, -1, pBuf, 0);
    pBuf = new wchar_t[nSize];
    memset(pBuf, 0, nSize);
    if(!MultiByteToWideChar(CP_UTF8, 0, pSour, -1, pBuf, nSize))
    {
        return strDist;
    }


    int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, pBuf, wcslen(pBuf), NULL, 0, NULL, NULL);
    char* szAnsi = new char[ansiLen + 1];
    ::WideCharToMultiByte(CP_ACP, NULL, pBuf, wcslen(pBuf), szAnsi, ansiLen, NULL, NULL);
    szAnsi[ansiLen] = 0;

    strDist = szAnsi;
	ClearMemArr(pBuf);
	ClearMemArr(szAnsi);
    return strDist;
}

string CStrCoding::AnsiToUtf8( LPCSTR pSour)
{
	string strDist;
    //Ansi->Unicode->utf8
    wchar_t* pBuf = NULL;
    int nSize = MultiByteToWideChar(CP_ACP, 0, pSour, -1, pBuf, 0);
    pBuf = new wchar_t[nSize];
    memset(pBuf, 0, nSize);
    if(!MultiByteToWideChar(CP_ACP, 0, pSour, -1, pBuf, nSize))
    {
        return strDist;
    }


    int ansiLen = ::WideCharToMultiByte(CP_UTF8, NULL, pBuf, wcslen(pBuf), NULL, 0, NULL, NULL);
    char* szAnsi = new char[ansiLen + 1];
    ::WideCharToMultiByte(CP_UTF8, NULL, pBuf, wcslen(pBuf), szAnsi, ansiLen, NULL, NULL);
    szAnsi[ansiLen] = 0;

    strDist = szAnsi;
	ClearMemArr(pBuf);
	ClearMemArr(szAnsi);

    return strDist;
}
/*

static unsigned char char_to_hex( unsigned char x )
{
    return (unsigned char)(x > 9 ? x + 55: x + 48);
}

static int is_alpha_number_char( unsigned char c )
{
    if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') )
        return 1;
    return 0;
}

//url编码实现

void urlencode( unsigned char * src, int  src_len, unsigned char * dest, int  dest_len )
{
    unsigned char ch;
    int  len = 0;

    while (len < (dest_len - 4) && *src)
    {
        ch = (unsigned char)*src;
        if (*src == ' ')
        {
            *dest++ = '+';
        }
        else if (is_alpha_number_char(ch) || strchr("-_.!~*'()", ch))
        {
            *dest++ = *src;
        }
        else
        {
            *dest++ = '%';
            *dest++ = char_to_hex( (unsigned char)(ch >> 4) );
            *dest++ = char_to_hex( (unsigned char)(ch % 16) );
        } 
        ++src;
        ++len;
    }
    *dest = 0;
    return ;
}
*/

static int is_alpha_number_char( unsigned char c )
{
    if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') )
        return 1;
    return 0;
}


unsigned char ToHex(unsigned char x) 
{ 
    return  x > 9 ? x + 55 : x + 48; 
}

unsigned char FromHex(unsigned char x) 
{ 
    unsigned char y;
    if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
    else if (x >= '0' && x <= '9') y = x - '0';
    else assert(0);
    return y;
}


string CStrCoding::EncodeUrl( const string& str )
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((unsigned char)str[i]) || 
            (str[i] == '-') ||
            (str[i] == '_') || 
            (str[i] == '.') || 
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += ToHex((unsigned char)str[i] >> 4);
            strTemp += ToHex((unsigned char)str[i] % 16);
        }
    }
    return strTemp;
}


std::string UrlEncode(const std::string& str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((unsigned char)str[i]) || 
            (str[i] == '-') ||
            (str[i] == '_') || 
            (str[i] == '.') || 
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += ToHex((unsigned char)str[i] >> 4);
            strTemp += ToHex((unsigned char)str[i] % 16);
        }
    }
    return strTemp;
}

std::string UrlDecode(const std::string& str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (str[i] == '+') strTemp += ' ';
        else if (str[i] == '%')
        {
            assert(i + 2 < length);
            unsigned char high = FromHex((unsigned char)str[++i]);
            unsigned char low = FromHex((unsigned char)str[++i]);
            strTemp += high*16 + low;
        }
        else strTemp += str[i];
    }
    return strTemp;
}

string CStrCoding::GB2GBK(const char* data)													//简体转繁体
{
	assert(data != NULL);
	string strDist;
	//无内容，不用转换
	if(!strcmp(data, ""))
		return strDist;

	int dataLen = strlen(data);
	DWORD wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
	int nReturn = LCMapStringA(wLCID, LCMAP_TRADITIONAL_CHINESE, data, dataLen, NULL, 0);

	if(!nReturn)
		return strDist;

	char *pcBuf = new char[nReturn + 1];
	
	wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
	LCMapStringA(wLCID, LCMAP_TRADITIONAL_CHINESE, data, nReturn, pcBuf, nReturn + 1);
	strDist.assign(pcBuf, nReturn);
	//strncpy(&strDist.at(0), pcBuf, nReturn);
	
	SAFERELEASE(pcBuf);
	return strDist;
}

string CStrCoding::GBK2GB(const char* data)
{
	assert(data != NULL);
	string strDist;
	if(!strcmp(data, ""))
		return strDist;

	int dataLen = strlen(data);
	DWORD wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
	int nReturn = LCMapStringA(wLCID, LCMAP_SIMPLIFIED_CHINESE, data, dataLen, NULL, 0);

	if(!nReturn)
		return strDist;

	char *pcBuf = new char[nReturn + 1];

	wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
	LCMapStringA(wLCID, LCMAP_SIMPLIFIED_CHINESE, data, nReturn, pcBuf, nReturn + 1);
	strDist.assign(pcBuf, nReturn);
	//strncpy(data, pcBuf, nReturn);

	SAFERELEASE(pcBuf);

	return strDist;
}

tstring CStrCoding::HttpDecode( StringCRef strData )
{
	tstring strL(strData);
	StrReplace(strL, _T("&quot;"), _T("\""));
	StrReplace(strL, _T("&amp;"), _T("&"));
	StrReplace(strL, _T("&lt;"), _T("<"));
	StrReplace(strL, _T("&gt;"), _T(">"));
	StrReplace(strL, _T("&nbsp;"), _T(" "));
	StrReplace(strL, _T("&#39;"), _T("'"));
	return strL;
}

string CStrCoding::UnicodeToUtf8( LPCWSTR szData )
{	
	//Unicode->utf8
	
	int ansiLen = ::WideCharToMultiByte(CP_UTF8, NULL, szData, wcslen(szData), NULL, 0, NULL, NULL);
	char* szAnsi = new char[ansiLen + 1];
	::WideCharToMultiByte(CP_UTF8, NULL, szData, wcslen(szData), szAnsi, ansiLen, NULL, NULL);
	szAnsi[ansiLen] = 0;

	string strDist = szAnsi;
	ClearMemArr(szAnsi);

	return strDist;
}

tstring CStrCoding::Utf8ToTStr( LPCSTR pSour )
{
#ifdef _UNICODE
	return Utf8ToUnicode(pSour);
#else
	return Utf8ToAnsi(pSour);
#endif
}

string CStrCoding::EncodeUrlToUtf8( const tstring& strParam )
{
	//to utf8, encode
#ifdef _UNICODE
	string strUtf8 = UnicodeToUtf8(strParam.c_str());
#else
	string strUtf8 = AnsiToUtf8(strParam.c_str());
#endif
	return EncodeUrl(strUtf8);
}

