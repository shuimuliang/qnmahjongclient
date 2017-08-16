#ifndef __SCRIPT_OBJECT_H__
#define __SCRIPT_OBJECT_H__

#include "cocos2d.h"

class CScriptObject : protected cocos2d::CCObject
{
public:
	CScriptObject() : _isValid(true)
	{

	}

	void destroy()
	{
		if(!_isValid)
		{
			CCAssert(false, "can not execute destroy repeated");
			return;
		}
		_isValid = true;
		onDestroy();
	}

protected:
	virtual void onDestroy()
	{
		cocos2d::CCObject::release();
	}


public:
	void release(void){cocos2d::CCObject::release();}

	void retain(void){cocos2d::CCObject::retain();}
	
	cocos2d::CCObject* autorelease(void)
	{
		CCAssert(false, "autorelease is baned");
	}


private:
	bool _isValid;
};

#endif