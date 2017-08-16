#ifndef __CCIModule_H_
#define __CCIModule_H_
#include<string>
#include<vector>

using std::string;
using std::vector;

namespace Engine
{
	class CCIModule
	{
	public:
		inline std::string getName(){return name;}
		virtual void onAdd()=0;
		void registerMediator(const string &name);
		void registerCommand(const string &name);
		void registerProxy(const string &name);
		void removeAll();
	protected:
		string name;
		vector<string> m_vecMediatorName;
		vector<string> m_vecCommandName;
		vector<string> m_vecProxyName;
		void removeMediator();
		void removeCommand();
		void removeProxy();
	};
}
#endif // __CCIModule_H_

