
#include "PiAES.h"
#include "AESHeader.h"
#include "StrCoding.h"

USING_NAMESPACE(CryptoPP)
CPiAES::CPiAES(void)
{
}

CPiAES::~CPiAES(void)
{
}

string CPiAES::AESEncryptString(const string& str_in, const string& key, const string& iv)
{
	string str_out;
	if (key.length() <= 0 
		|| iv.length() <= 0)
	{
		return str_out;
	}

	CFB_Mode<AES>::Encryption encryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());

	StringSource encryptor(str_in, true, 
		new StreamTransformationFilter(
		encryption, 
		new Base64Encoder(
		new StringSink(str_out),false)));
	return str_out;
}

wstring CPiAES::AESEncryptString( const wstring& str_in, const wstring& key, const wstring& iv )
{
	return s2w(AESEncryptString(w2s(str_in), w2s(key), w2s(iv)));
}
