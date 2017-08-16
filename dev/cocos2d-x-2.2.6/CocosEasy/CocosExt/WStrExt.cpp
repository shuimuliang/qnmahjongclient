#include "WStrExt.h"
#include <stdlib.h>

NS_CC_EASY_BEGIN

#define DELETEIF(p) { if (p) { delete (p); (p) = NULL; }}

	//字符串拷贝
	void wstrcpy(wchar * s1, wchar * s2)
	{
		while(*s2 != 0)
		{
			*s1 = *s2;
			++s1;
			++s2;
		}
	}
	void wstrncpy(wchar * s1, const wchar * s2, int len)
	{
		for(int i = 0; i <len; ++i)
			*s1++ = *s2++;
	}

	//查找字符在字符串中出现的位置
	wchar* wstrchr(const wchar * s, wchar c)
	{
		while(*s != 0)
		{
			if(*s == c) return (wchar*)s;
			++s;
		}
		return 0;
	}
	int wstrlen(const wchar *s)
	{
		int r = 0;
		while(*s != 0)
		{
			++r;
			++s;
		}
		return r;
	}
	int wstrnicmp(wchar *dst, wchar *src, int count)
	{
		wchar ch1, ch2;
		do 
		{
			ch1 = *dst;
			ch2 = *src;
			if(ch1 >= 'A' && ch1 <= 'Z') ch1 += 0x20;
			if(ch2 >= 'A' && ch2 <= 'Z') ch2 += 0x20;
			--count;
			++dst;
			++src;
		} while(count >= 0 && ch1 && (ch1 == ch2));
		return (ch1 - ch2);
	}
	int wstrcmp(const wchar *src, const wchar *dst)
	{
		wchar ch1, ch2;
		do 
		{
			ch1 = *dst;
			ch2 = *src;
			++dst;
			++src;
		} while(ch1 && ch2 && (ch1 == ch2));
		return (ch1 - ch2);
	}

	int wstrcat(wchar *dst, wchar *src)
	{
		int dst_len = wstrlen(dst);
		int src_len = wstrlen(src);
		memcpy(dst + dst_len, src, src_len * sizeof(wchar));
		dst[dst_len + src_len] = 0;
		return 0;
	}
	wchar* wstrdup(const wchar *src)
	{
		int len = wstrlen(src);
		if(len == 0)
			return NULL;
		wchar * r = (wchar*)malloc((len + 1) * sizeof(wchar));
		memcpy(r, src, (len + 1) * sizeof(wchar));
		r[len] = 0;
		return r;
	}

	bool wstrtoutf8( const wchar* pSrc, int nLen, byte* pDest, int nSize )
	{
		// 	wcstombs();
		// 		mbstowcs();
		int i = 0;
		int count = 0;
		for(i = 0; i < nLen; i++)
		{
			if(pSrc[i] < 0x7f)
			{
				if(count >= nSize)
					break;
				pDest[count++] = pSrc[i];
			}
			else if(pSrc[i] < 0x07ff)
			{
				if(count + 1 >= nSize)
					break;
				pDest[count++] = (0x06 << 5) + (pSrc[i] >> 6);
				pDest[count++] = (0x02 << 6) + (pSrc[i] & 0x3f);
			}
			else
			{
				if(count + 2 >= nSize)
					break;
				pDest[count++] = (0xfe << 4) + (pSrc[i] >> 12);
				pDest[count++] = (0x02 << 6) + ((pSrc[i] & 0x0fff) >> 6);
				pDest[count++] = (0x02 << 6) + (pSrc[i] & 0x3f);
			}
		}
		if(count<=nSize){
			pDest[count] = 0;
		}
		return true;
	}

	bool utf8towstr( const byte* pSrc, int nLen, wchar* pDest, int nSize )
	{
		if(nLen < 0 || nSize < 0)
			return false;
		int i = 0;
		int count = 0;
		while(i < nLen)
		{
			if(count >= nSize)
			{
				break;
			}
			if(pSrc[i] >> 7 == 0)
			{
				pDest[count++] = pSrc[i++];
			}
			else if(pSrc[i] >> 5 == 0x06)
			{
				if(i + 1 >= nLen)
					break;
				pDest[count++] = ((pSrc[i] & 0x1f) << 6) + (pSrc[i+1] & 0x3f);
				i += 2;
			}
			else if(pSrc[i] >> 4 == 0x0e)
			{
				if(i + 2 >= nLen)
					break;
				pDest[count++] = ((pSrc[i] & 0x0f) << 12) + ((pSrc[i+1] & 0x3f) << 6) + (pSrc[i+2] & 0x3f);
				i += 3;
			}
			else
			{
				return false;
			}
		}
		if(count<nSize){
			pDest[count] = 0;
		}
		return true;
	}

	wchar* wstrstr( const wchar * src1, const wchar* src2 )
	{
		int len1 = wstrlen(src1);
		int len2 = wstrlen(src2);
		wchar * r = (wchar*)malloc((len1 + len2 + 1) * sizeof(wchar));
		memcpy(r, src1, len1 * sizeof(wchar));
		memcpy(r + len1, src2, len2 * sizeof(wchar));
		r[len1 + len2] = 0;
		return r;
	}
	WStringExt::WStringExt()
	{
		str = NULL;
	}

	WStringExt::WStringExt( const WStringExt & ws )
	{
		_from_wchar_p(ws.str);
	}

	WStringExt::WStringExt( wchar * ws )
	{
		_from_wchar_p(ws);
	}

	WStringExt::WStringExt( const char * s )
	{
		_from_char_p_decode(s);
	}

	WStringExt::WStringExt( std::string s )
	{
		_from_char_p_decode(s.c_str());
	}
	WStringExt::~WStringExt()
	{
		DELETEIF(str);
	}


	WStringExt & WStringExt::operator = (const WStringExt & ws)
	{
		DELETEIF(str);
		_from_wchar_p(ws.str);
		return *this;
	}
	WStringExt & WStringExt::operator = (wchar * ws)
	{
		DELETEIF(str);
		_from_wchar_p(ws);
		return *this;
	}
	WStringExt & WStringExt::operator = (std::string s)
	{
		DELETEIF(str);
		_from_char_p_decode(s.c_str());
		return *this;
	}
	WStringExt & WStringExt::operator = (const char * s)
	{
		DELETEIF(str);
		_from_char_p_decode(s);
		return *this;
	}

	void WStringExt::_from_wchar_p( wchar * p )
	{
		int l = 0;
		if(p) l = WSTRLEN(p);
		if(l > 0)
		{
			str = new wchar[l + 1];
			memcpy(str, p, l * 2);
			str[l] = 0;
		}
		else str = 0;
	}

	void WStringExt::_from_char_p_decode( const char * p )
	{
		if(NULL == p)
		{
			str = 0;
			return;
		}

		int nLen = strlen(p);
		str = new wchar[nLen * 3 + 3];
		if(UTF8TOWSTR((byte*)p, nLen, str, nLen * 3 + 3) != true)
		{
		}
	}

	wchar * WStringExt::c_str()
	{
		return str;
	}

	unsigned char WStringExt::operator==( WStringExt s )
	{
		return WSTRCMP(str, s.str) == 0;
	}

	unsigned char WStringExt::operator!=( WStringExt s )
	{
		return !(WSTRCMP(str, s.str) == 0);
	}

	int WStringExt::length()
	{
		return wstrlen(c_str());
	}

	void WStringExt::substr( int begin,int num ,WStringExt& des)
	{
		if(num>0){
			wchar *pDesChar = new wchar[num+1];
			const wchar* pSrc = this->c_str()+begin;
			wstrncpy(pDesChar,pSrc,num);
			pDesChar[num]=0;
			des = pDesChar;
			delete[] pDesChar;
		}else{
			des="";
		}
	}

	void WStringExt::getUtf8(char* pChar)
	{
		const int leng = length()*3+3;
		wstrtoutf8(c_str(),length(),(byte*)pChar,leng);
	}

NS_CC_EASY_END
