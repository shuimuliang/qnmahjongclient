#include "DataParser.h"

NS_CC_EASY_BEGIN

CDataParser::CDataParser( const char* name )
{
	if(!name){assert(false); return;}
	m_strValue = name;
	parse();
}

CDataParser::CDataParser( const char* st,const char* ed )
{
	if(!st || !ed) {assert(false); return;}
	if(ed - st < 0) {assert(false); return;}
	char buf[DATA_PARSER_BUF_SIZE] = {0};
	const char* first = buf;
	char * mem = NULL;
	if(ed - st >= DATA_PARSER_BUF_SIZE)
	{
		mem = new char[ed-st+2];
		memset(mem,0,sizeof(char)*(ed-st+2));
		first = mem;
	}
	memcpy((void*)first,st,ed-st+1);
	m_strValue = first;
	if(mem){delete[] mem; mem = NULL;}
	//_CrtCheckMemory();
}


void CDataParser::parse()
{
	if(m_Data.size() != 0) return;
	unsigned int count = m_strValue.length();
	if(count <= 0) return;
	const char* st = m_strValue.c_str();
	const char* ed = st;
	const char* cur = st;
	const char* strEnd = &m_strValue[count-1];
	bool hasData = false;
	for (cur; cur <= strEnd; ++cur)
	{
		switch (*cur)
		{
		case ' ':
			{
				if(!hasData) st = cur + 1;
				continue; break;
			}
		case '(':
			if(ed-st > 0){assert(false);} // format error 123( 
		case ')':
		case '|':
			{
				if(hasData)
				{
					CDataParser data(st,ed);
					m_Data.push_back(data);
				}
				st = cur + 1;
				ed = st;
				hasData = false;
				continue;
			}break;
		default:
			{
				ed = cur;
				hasData = true;
				continue; 
			}break;
			
		}
		assert(false);
	}

	assert(cur-1 == strEnd);
	if(hasData)
	{
		// leaf node
		if(m_Data.size() == 0) return;
		assert(ed >= st);
		CDataParser data(st,ed);
		m_Data.push_back(data);
	}
}

NS_CC_EASY_END