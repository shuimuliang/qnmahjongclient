#include "CELuaEngine.h"
#include "CocosExt/CCLString.h"
#include "Utility/Log.h"

extern "C" {
#include "lua.h"
#include "tolua++.h"
#include "lualib.h"
#include "lauxlib.h"
#include "tolua_fix.h"
}

//CCELuaEngine* CCELuaEngine::m_defaultEngine = NULL;
#define LUA_FILE_SUFFIX ".lua"

extern "C"
{
	int cocoseasy_lua_loader(lua_State *L)
	{
		std::string filename(luaL_checkstring(L, 1));
		size_t pos = filename.rfind(LUA_FILE_SUFFIX);
		if (pos == std::string::npos)
		{
			filename = filename + LUA_FILE_SUFFIX;
		}
		
		//search file in package.path
		unsigned char* chunk = NULL;
		unsigned long chunkSize = 0;
		std::string chunkName = filename;
		CCFileUtils* utils = CCFileUtils::sharedFileUtils();

		std::string path = utils->fullPathForFilename(filename.c_str());
		if(path == filename)
		{
			CEFatal("can not find file %s",filename.c_str());
			return 1;
		}

		chunk = utils->getFileData(chunkName.c_str(), "rb", &chunkSize);

		if (NULL != chunk)
		{
			CCLuaStack* stack = CCLuaEngine::defaultEngine()->getLuaStack();
			stack->luaLoadBuffer(L, (char*)chunk, (int)chunkSize, chunkName.c_str());
			free(chunk);
		}
		else
		{
			CEFatal("can not get file data of %s", filename.c_str());
		}

		return 1;
	}
}


static void luaErrorCatcher(const char* error)
{
	if(error) CEFatal(error);
}

CCELuaEngine* CCELuaEngine::defaultEngine( void )
{
	if (!m_defaultEngine)
	{
		m_defaultEngine = new CCELuaEngine();
		static_cast<CCELuaEngine*>(m_defaultEngine)->init();
	}
	return static_cast<CCELuaEngine*>(m_defaultEngine);
}

void CCELuaEngine::end()
{
	CC_SAFE_DELETE(m_defaultEngine);
	CCLuaEngine::m_defaultEngine = NULL;
}


CCELuaEngine::~CCELuaEngine()
{
	CCLuaStack* pStack = m_defaultEngine->getLuaStack();
	lua_State* L = pStack->getLuaState();
	lua_close(L);
}



bool CCELuaEngine::init()
{
	if(!CCLuaEngine::init()) return false;
	CCLuaStack* pStack = m_defaultEngine->getLuaStack();
	if(!pStack) return false;
	pStack->setErrorCathcer( (CCLuaStack::ErrorCatch)&(luaErrorCatcher) );
	pStack->addLuaLoader(cocoseasy_lua_loader);
	return true;
}

int CCELuaEngine::executeEventWithArgs( int nHandler, CCArray* pArgs )
{
    if (NULL == pArgs)
		return 0;
    
    CCObject*   pObject = NULL;
    
    CCInteger*  pIntVal = NULL;
    CCString*   pStrVal = NULL;
    CCLString*	pLStrVal = NULL;
    CCDouble*   pDoubleVal = NULL;
    CCFloat*    pFloatVal = NULL;
    CCBool*     pBoolVal = NULL;
    
    
    int nArgNums = 0;
    for (unsigned int i = 0; i < pArgs->count(); i++)
    {
        pObject = pArgs->objectAtIndex(i);
        if (NULL != (pIntVal = dynamic_cast<CCInteger*>(pObject)))
        {
            m_stack->pushInt(pIntVal->getValue());
            nArgNums++;
        }
        else if (NULL != (pStrVal = dynamic_cast<CCString*>(pObject)))
        {
            m_stack->pushString(pStrVal->getCString(),pStrVal->length());
            nArgNums++;
        }
        else if (NULL != (pDoubleVal = dynamic_cast<CCDouble*>(pObject)))
        {
            m_stack->pushFloat(pDoubleVal->getValue());
            nArgNums++;
        }
        else if (NULL != (pFloatVal = dynamic_cast<CCFloat*>(pObject)))
        {
            m_stack->pushFloat(pFloatVal->getValue());
            nArgNums++;
        }
        else if (NULL != (pBoolVal = dynamic_cast<CCBool*>(pObject)))
        {
            m_stack->pushBoolean(pBoolVal->getValue());
            nArgNums++;
        }
        else if (NULL != (pLStrVal = dynamic_cast<CCLString*>(pObject)))
        {
            m_stack->pushString((const char*)pLStrVal->getString(),pLStrVal->getLength());
            nArgNums++;
        }
        else if(NULL != pObject)
        {
            m_stack->pushCCObject(pObject, "CCObject");
            nArgNums++;
        }
    }
    
    return  m_stack->executeFunctionByHandler(nHandler, nArgNums);
}



