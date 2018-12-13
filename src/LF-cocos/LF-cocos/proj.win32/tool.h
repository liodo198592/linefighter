#ifndef _TOOLS_H_
#define  _TOOLS_H_
#include "global_cocos.h"
#include "cocos2d.h" 

	#ifdef MAKE_ANDRIOD
	#include "iconv.h"
	#include "string"
	#else
	#include "iconv\iconv.h"
	#endif


int GBKToUTF8(std::string &gbkStr,char* toCode,char* formCode);  
string getStrFromInt(int i);
inline void WStrToUTF8(std::string& dest, const wstring& src);
inline std::string WStrToUTF8(const std::wstring& str);
std::string w2c(const wchar_t * pw);
std::wstring c2w(const char *pc);

#endif  
