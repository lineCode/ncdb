#pragma once
#include "Cryptopp/aes.h"
#include "Cryptopp/modes.h"
#include "Cryptopp/filters.h"
#include "Cryptopp/hex.h"
#include "Cryptopp/md5.h"
#include "Cryptopp/files.h"
#include "Cryptopp/base64.h"

#ifdef _DEBUG
	//_MT && _DLL æ≤Ã¨c‘À––ø‚
#if defined(_MT) && defined(_DLL)
	#pragma comment(lib, "cryptlib_d_md.lib")	
#else 
	#pragma comment(lib, "cryptlib_d_mt.lib")	
#endif

#else

#if defined(_MT) && defined(_DLL)
	#pragma comment(lib, "cryptlib_md.lib")
#else 
	#pragma comment(lib, "cryptlib_mt.lib")
#endif

#endif