#ifdef UNICODE

#define tifstream wifstream
#define tchar wchar_t
#define tstring wstring
#define tcout wcout
#else 

#define tifstream ifstream
#define tchar char
#define tstring string
#define tcout cout
#endif