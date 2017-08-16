#ifndef __CE_LUA_ENGINE_H__
#define __CE_LUA_ENGINE_H__

#include "Common/CocosEasyDef.h"
#include "CCLuaEngine.h"

class CCELuaEngine : public CCLuaEngine
{
public:
	~CCELuaEngine();
	static CCELuaEngine* defaultEngine(void);
	static void end();

public:
	virtual int executeEventWithArgs(int nHandler, CCArray* pArgs);

public:
	bool init();
};
#endif