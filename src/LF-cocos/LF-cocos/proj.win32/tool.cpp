#include "tool.h"  
#ifdef MAKE_ANDRIOD
#include "iconv.h"
#else
#include "iconv\iconv.h"
#endif

#include "stdio.h"



//字符转换，使cocos2d-x在win32平台支持中文显示  
int GBKToUTF8(std::string &gbkStr,char* toCode,char* formCode)  
{  
	iconv_t iconvH;  
	iconvH = iconv_open(formCode,toCode);  
	if(iconvH == 0)  
	{  
		return -1;  
	}  

	char* strChar = (char *)gbkStr.c_str();  
	char** pin = &strChar;

	size_t strLength = gbkStr.length();  
	char* outbuf = (char*)malloc(strLength*4);  
	char* pBuff = outbuf;  
	memset(outbuf,0,strLength*4);  
	size_t outLength = strLength*4;  
#ifdef MAKE_ANDRIOD
	if(-1 == iconv(iconvH,pin,&strLength,&outbuf,&outLength))  
	{  
		iconv_close(iconvH);  
		return -1;  
	}
#else
	if(-1 == iconv(iconvH,(const char **)pin,&strLength,&outbuf,&outLength))  
	{  
		iconv_close(iconvH);  
		return -1;  
	}
#endif // MAKE_ANDRIOD

	  

	gbkStr = pBuff;  
	iconv_close(iconvH);  
	return 0;  
}  
/** 
**在封装一层，直接传入一个string，转换后还回对应的编码给你 
*/  
const char* GBKToUTF(std::string &gbkStr)  
{  
	//WStrToUTF8(c2w(gbkStr.c_str()));
	GBKToUTF8(gbkStr,"gbk","utf-8"); //后面两个参数就默认了,免得后面再传参麻烦  
	return gbkStr.c_str();  
}  

string getStrFromInt(int i)
{
	char content[20] = {0};
	sprintf(content,"%d",i);
	string str(content);
	return str;
}


inline void WStrToUTF8(std::string& dest, const wstring& src)
{
	dest.clear();
	for (size_t i = 0; i < src.size(); i++){
		wchar_t w = src[i];
		if (w <= 0x7f)
			dest.push_back((char)w);
		else if (w <= 0x7ff){
			dest.push_back(0xc0 | ((w >> 6)& 0x1f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (w <= 0xffff){
			dest.push_back(0xe0 | ((w >> 12)& 0x0f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (sizeof(wchar_t) > 2 && w <= 0x10ffff){
			dest.push_back(0xf0 | ((w >> 18)& 0x07)); // wchar_t 4-bytes situation
			dest.push_back(0x80| ((w >> 12) & 0x3f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else
			dest.push_back('?');
	}
}

inline std::string WStrToUTF8(const std::wstring& str)
{
	std::string result;
	WStrToUTF8(result, str);
	return result;
}

/* 
    string 转换为 wstring  
*/  
std::wstring c2w(const char *pc)  
{  
    std::wstring val = L"";  
  
    if(NULL == pc)  
    {  
        return val;  
    }  
    //size_t size_of_ch = strlen(pc)*sizeof(char);  
    //size_t size_of_wc = get_wchar_size(pc);  
    size_t size_of_wc;  
    size_t destlen = mbstowcs(0,pc,0);  
    if (destlen ==(size_t)(-1))  
    {  
        return val;  
    }  
    size_of_wc = destlen+1;  
    wchar_t * pw  = new wchar_t[size_of_wc];  
    mbstowcs(pw,pc,size_of_wc);  
    val = pw;  
    delete pw;  
    return val;  
}  
/* 
    wstring 转换为 string 
*/  
std::string w2c(const wchar_t * pw)  
{  
    std::string val = "";  
    if(!pw)  
    {  
        return val;  
    }  
    size_t size= wcslen(pw)*sizeof(wchar_t);  
    char *pc = NULL;  
    if(!(pc = (char*)malloc(size)))  
    {  
        return val;  
    }  
    size_t destlen = wcstombs(pc,pw,size);  
    /*转换不为空时，返回值为-1。如果为空，返回值0*/  
    if (destlen ==(size_t)(0))  
    {  
        return val;  
    }  
    val = pc;  
    delete pc;  
    return val;  
}  