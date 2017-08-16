#include "Controller.h"
#include "Observer.h"
#include "Facade.h"

namespace Engine
{

	Controller::Controller(Facade* pFacade)
	{
		mFacade = pFacade;
	}


	Controller::~Controller(void)
	{
		ICommand *pCommand = NULL;
		for(CommandMapItr itr = m_commandMap.begin();itr!=m_commandMap.end();++itr){
			pCommand = itr->second;
			if(pCommand!=NULL){
				CCObject *objCommand = dynamic_cast<CCObject*>(pCommand);
				if(objCommand){
					//CCLOG("=================================mediator name = %s",mediator->getName().c_str());
					objCommand->release();
				}else{
					delete(pCommand);
				}
				pCommand = NULL;
			}
		}
		m_commandMap.clear();
		mFacade=NULL;
	}

	void Controller::registerCommand( const std::string & notificationName, ICommand *pCmd )
	{
		CommandMapItr itr = m_commandMap.find(notificationName); 
		if (itr == m_commandMap.end())
		{
			mFacade->addObsever(notificationName,pCmd);
		}else{
			CCAssert(false,"ÖØ¸´×¢²á");
		}
		m_commandMap[ notificationName ] = pCmd;
	}

	void Controller::removeCommand( const std::string & notificationName )
	{
		CommandMapItr itr = m_commandMap.find(notificationName);
		if(itr!=m_commandMap.end()){
			mFacade->removeObsever(notificationName,itr->second);
			delete itr->second;
			m_commandMap.erase(notificationName);
		}
	}

	bool Controller::hasCommand( const std::string & notificationName )
	{
		return m_commandMap.find(notificationName) != m_commandMap.end(); 
	}

	int Controller::getNumOfCommand()
	{
		CCLOG("========RestCommand==========");
		CommandMapItr itr = m_commandMap.begin();
		while(itr!=m_commandMap.end()){
			CCLOG("command:%s",itr->first.c_str());
			++itr;
		}
		return m_commandMap.size();
	}

	ICommand* Controller::retriveCommand(const std::string notificantionName )
	{
		CC_ASSERT(m_commandMap[notificantionName]);
		return m_commandMap[notificantionName];
	}

}