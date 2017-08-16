#ifndef __G_STRING_H__
#define __G_STRING_H__

#include <stdio.h>
//#include <malloc.h>
#include <string.h>

class GString
{
public:
	size_t size() const
	{
		return _size;
	}

	const char* c_str() const
	{
		return _str;
	}

	~GString()
	{
		if(_str) free(_str);
	}

	GString(const char* str) : _str(NULL), _size(0)
	{
		_save(str, strlen(str));
	}

	GString(const char* str, unsigned int size) : _str(NULL), _size(0)
	{
		_save(str, size);
	}

	GString(const unsigned char* str, unsigned int size) : _str(NULL), _size(0)
	{
		_save((const char*)str, size);
	}

	GString& operator = (const char* str)
	{
		_save(str, strlen(str));
		return *this;
	}

	GString& operator = (const unsigned char* str)
	{
		_save((const char*)str, strlen((const char*)str));
		return *this;
	}

	GString& operator = (const GString &gstr)
	{
		_save(gstr.c_str(), gstr.size());
		return *this;
	}

protected:
	void _save(const char* str, size_t size)
	{
		if(_str)
		{
			free(_str);
		}
		_str = (char*)malloc( (size+1) * sizeof(char));
		memcpy(_str, str, size);
		_str[size] = '\0';
		_size = size + 1;
	}

protected:
	char* _str;
	size_t _size;
};

#endif