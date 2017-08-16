#ifndef __CE_DATA_PARSER_H__
#define __CE_DATA_PARSER_H__

#include "Common/CocosEasyDef.h"

NS_CC_EASY_BEGIN


#define  DATA_PARSER_BUF_SIZE 256

class CDataParser
{
public:
	CDataParser(const char* name);
	CDataParser(const char* st,const char* ed);
	bool isLeaf() const {return m_Data.size() == 0;}
	unsigned int count() const {return m_Data.size();}
	const CDataParser& at(unsigned int i) const {assert(i<count()); return m_Data[i];}
	int _int() const {return atoi(m_strValue.c_str());}
	unsigned int _uint() const {return (unsigned int)_int();}
	float _float() const {return atof(m_strValue.c_str());}
	double _double() const {return _float();}
	bool _bool() const {return (_int() != 0);}
	const char* _str() const {return m_strValue.c_str();}

protected:
	void parse();

private:
	CC_SYNTHESIZE_READONLY(std::string,m_strValue,StrValue);
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<CDataParser>,m_Data,Data);
};


NS_CC_EASY_END


#endif