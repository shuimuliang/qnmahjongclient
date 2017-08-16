#include "CCIModule.h"
#include "Mvc.h"

namespace Engine
{

	void CCIModule::registerMediator( const string &name )
	{
		m_vecMediatorName.push_back(name);
	}

	void CCIModule::registerCommand( const string &name )
	{
		m_vecCommandName.push_back(name);
	}

	void CCIModule::registerProxy( const string &name )
	{
		m_vecProxyName.push_back(name);
	}

	void CCIModule::removeAll()
	{
		removeCommand();
		removeMediator();
		removeProxy();
	}

	void CCIModule::removeMediator()
	{
		vector<string>::iterator itr = m_vecMediatorName.begin();
		CCFacade *pMainFacade = CCFacade::getInstance();
		CCNetFacade *pNetFacade = CCNetFacade::getInstance();
		while(itr!=m_vecMediatorName.end()){
			pMainFacade->removeMediator(*itr);
			pNetFacade->removeMediator(*itr);
			++itr;
		}
	}

	void CCIModule::removeCommand()
	{
		vector<string>::iterator itr = m_vecCommandName.begin();
		CCFacade *pMainFacade = CCFacade::getInstance();
		CCNetFacade *pNetFacade = CCNetFacade::getInstance();
		while(itr!=m_vecCommandName.end()){
			pMainFacade->removeCommand(*itr);
			pNetFacade->removeCommand(*itr);
			++itr;
		}
	}

	void CCIModule::removeProxy()
	{
		vector<string>::iterator itr = m_vecProxyName.begin();
		CCFacade *pMainFacade = CCFacade::getInstance();
		CCNetFacade *pNetFacade = CCNetFacade::getInstance();
		while(itr!=m_vecProxyName.end()){
			pMainFacade->removeProxy(*itr);
			pNetFacade->removeProxy(*itr);
			++itr;
		}
	}

}