#ifndef __WSTR_EXT__H__
#define __WSTR_EXT__H__

#include "Common/CocosEasyDef.h"

NS_CC_EASY_BEGIN
#ifndef wchar
	typedef unsigned short wchar;
#endif
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
#ifndef byte
typedef unsigned char byte;
#endif
#endif
#define WSTRNICMP(a, b, c) wstrnicmp((wchar *)a, (wchar *)b, c)
#define WSTRLEN(a) wstrlen((wchar *)a)
#define WSTRCMP(a, b) wstrcmp((wchar *)a, (wchar *)b)
#define WSTRCPY(a, b) wstrcpy((wchar *)a, (wchar *)b)
#define WSTRCHR(a, b) wstrchr((wchar *)a, (wchar)b)

#define WSTRDUP(a) wstrdup((wchar *)a)
#define WSTRCAT wstrcat
#define STRLEN	strlen
#define STRCHR  strchr
#define WSTRTOUTF8 wstrtoutf8
#define UTF8TOWSTR utf8towstr

	void wstrcpy(wchar * s1, wchar * s2);
	void wstrncpy(wchar * s1,const wchar * s2, int len);
	wchar* wstrchr(const wchar * s, wchar c);
	int wstrlen(const wchar *s);
	int wstrnicmp(wchar *dst, wchar *src, int count);
	int wstrcmp(const wchar *src, const wchar *dst);
	wchar* wstrdup(const wchar *src);
	int wstrcat(wchar *dst, wchar *src);
	bool wstrtoutf8(const wchar* pSrc, int nLen, byte* pDest, int nSize);
	bool utf8towstr(const byte* pSrc, int nLen, wchar* pDest, int nSize);
	wchar* wstrstr(const wchar * src1, const wchar* src2);

	class WStringExt
	{
	public:
		WStringExt();
		WStringExt(const WStringExt & ws);
		WStringExt(wchar * ws);
		WStringExt(std::string s);//UTF-8解码
		WStringExt(const char * s);//UTF-8解码
		WStringExt & operator = (const WStringExt & ws);
		WStringExt & operator = (wchar * ws);
		WStringExt & operator = (std::string s);
		WStringExt & operator = (const char * s);
		unsigned char operator == (WStringExt s);
		unsigned char operator != (WStringExt s);
		~WStringExt();
		wchar * c_str();
		int length();
		void substr(int begin,int num,WStringExt& des);
		void getUtf8(char* pChar);
	private:
		wchar * str;
		void _from_wchar_p(wchar * p);
		void _from_char_p_decode(const char * p);
	};

NS_CC_EASY_END
#endif //