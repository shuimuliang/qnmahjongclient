#ifndef __CE_DICTIONARY_EXT_H__
#define __CE_DICTIONARY_EXT_H__


#include "Common/CocosEasyDef.h"

USING_NS_CC;

NS_CC_EASY_BEGIN

class CDictionaryExt
{
public:
	CDictionaryExt(CCDictionary* pDict){m_pDict = pDict; assert(m_pDict);}

#define KEY_PARSE(key,func,dft) \
	{\
		if(!m_pDict) return dft;\
		const CCString* pStrVal = m_pDict->valueForKey(key); \
		if(!pStrVal || pStrVal->length() == 0) return dft; \
		return pStrVal->func(); \
	}\


public:
	int intKeyParse(const char* key,int dft = 0) const {KEY_PARSE(key,intValue,dft);}
	unsigned int uintKeyParse(const char* key,unsigned int dft = 0) const {KEY_PARSE(key,uintValue,dft);}
	float floatKeyParse(const char* key,float dft = 0) const {KEY_PARSE(key,floatValue,dft);}
	double doubleKeyParse(const char* key,double dft = 0) const {KEY_PARSE(key,doubleValue,dft);}
	bool boolKeyParse(const char* key,bool dft = true) const {KEY_PARSE(key,boolValue,dft);}
	const char* strKeyParse(const char* key,const char* dft = NULL) const {KEY_PARSE(key,getCString,dft);}

private:
	CCDictionary* m_pDict;
};


NS_CC_EASY_END

#endif