#ifndef __CCLSTRING_H__
#define __CCLSTRING_H__

#include "Common/CocosEasyDef.h"

class CCLString : public CCObject
{
public:
	CCLString()
	: m_Data(NULL)
	, m_Length(0)
	{
	}

	~CCLString()
	{
		CC_SAFE_DELETE(m_Data);
	}

	static CCLString* create(const unsigned char* pData, unsigned int length)
	{
		CCLString* pRet = new CCLString();
		if (pRet && pRet->init(pData, length))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_DELETE(pRet);                                     
		return NULL; 
	}

	static CCLString* create()
	{
		CCLString* pRet = new CCLString();
		return pRet;
	}

	void append(const unsigned char* pData, unsigned int length)
	{
		if(!pData) return;
		const unsigned char* preData = m_Data;
		unsigned int sum = m_Length + length + 1;
		m_Data = (unsigned char*) malloc(sizeof(unsigned char) * sum);
		memset((void*)m_Data, 0, sizeof(unsigned char) * sum);
		
		if(preData) memcpy((void*)m_Data, preData, m_Length);
		memcpy((void*)&m_Data[m_Length], pData, length);
		m_Length += length;

		if(preData) free((void*)preData);
	}

	const unsigned char* getString(){return m_Data;}
	unsigned int getLength(){ return m_Length; }

protected:
	bool init(const unsigned char* pData, unsigned int length)
	{
		if(!pData) return false;
		append(pData, length);
		return true;
	}

private:
	const unsigned char* m_Data;
	unsigned int m_Length;
};





#endif