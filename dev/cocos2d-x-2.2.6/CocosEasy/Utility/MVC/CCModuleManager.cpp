#include "CCModuleManager.h"
#include "cocos2d.h"
USING_NS_CC;

namespace Engine
{

	CCModuleManager::CCModuleManager():m_pCurModuleRet(NULL)
	{

	}

	CCModuleManager::~CCModuleManager()
	{
		removeAll();
	}

	void CCModuleManager::addModule( CCIModule *module )
	{
		ModuleMapItr itr = mModuleMap.find(module->getName());
		if(itr==mModuleMap.end())
		{
			m_pCurModuleRet = module;
			module->onAdd();
			m_pCurModuleRet = NULL;
		}
		mModuleMap[ module->getName() ] = module;
	}

	void CCModuleManager::removeModule( const string &name )
	{
		ModuleMapItr itr = mModuleMap.find(name);
		if(itr!=mModuleMap.end())
		{
			CCIModule *pModule = itr->second;
			pModule->removeAll();
			mModuleMap.erase(itr);
			delete pModule;
		}
	}

	void CCModuleManager::removeAll()
	{
		ModuleMapItr itr = mModuleMap.begin();
		while(itr!=mModuleMap.end())
		{
			CCIModule *pModule = itr->second;
			if(pModule!=NULL)
			{
				CCLOG("removeModuleBegin:%s",pModule->getName().c_str());
				pModule->removeAll();
				CCLOG("removeModuleEnd:%s",pModule->getName().c_str());
				delete pModule;
			}
			++itr;
		}
		mModuleMap.clear();
	}

	int CCModuleManager::getNumOfModule()
	{
		CCLOG("========RestModule==========");
		ModuleMapItr itr = mModuleMap.begin();
		while(itr!=mModuleMap.end())
		{
			CCLOG("module:%s",itr->first.c_str());
			++itr;
		}
		return mModuleMap.size();
	}

	void CCModuleManager::removeModuleExcept(const set<string> &setName)
	{
		ModuleMapItr itr = mModuleMap.begin();
		while(itr!=mModuleMap.end())
		{
			CCIModule *pModule = itr->second;
			if(pModule!=NULL)
			{
				if(setName.find(pModule->getName())!=setName.end())
				{
					pModule->removeAll();
					mModuleMap.erase(itr);
					delete pModule;
				}
			}
			++itr;
		}
	}


	void CCModuleManager::registerMediator( const string &name )
	{
		if(m_pCurModuleRet){
			m_pCurModuleRet->registerMediator(name);
		}
	}

	void CCModuleManager::registerCommand( const string &name )
	{
		if(m_pCurModuleRet){
			m_pCurModuleRet->registerCommand(name);
		}
	}

	void CCModuleManager::registerProxy( const string &name )
	{
		if(m_pCurModuleRet){
			m_pCurModuleRet->registerProxy(name);
		}
	}
}