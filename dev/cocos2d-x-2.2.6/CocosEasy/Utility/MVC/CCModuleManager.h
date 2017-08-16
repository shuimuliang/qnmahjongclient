#ifndef __CCMODULE_MANAGER_H_
#define __CCMODULE_MANAGER_H_

#include "CCIModule.h"
#include <string>
#include <map>
#include <set>
#include "../Single.h"
using namespace std;

namespace Engine
{

	//将游戏中的功能以模块的形式进行组织
	class CCModuleManager:public Singleton<CCModuleManager>
	{
	public:
		CCModuleManager();
		~CCModuleManager();
		//addModule时触发onAdd事件
		void addModule(CCIModule *module);
		//移除时进行内存delete
		void removeModule(const string &name);
		//移除全部Module();
		void removeAll();
		//除了以下模块都移除
		void removeModuleExcept(const set<string> &setName );
		//获得module数量
		int getNumOfModule();

		void registerMediator(const string &name);
		void registerCommand(const string &name);
		void registerProxy(const string &name);
	private:
		typedef map<string,CCIModule *> ModuleMap;
		typedef ModuleMap::iterator ModuleMapItr;
		ModuleMap mModuleMap;
		//是一个指向，有时是NULL
		CCIModule *m_pCurModuleRet;
	};
}

#endif // __CCMODULE_MANAGER_H_

