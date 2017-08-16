#include "Facade.h"
#include "../interface/INotification.h"
#include "../pattern/obsever/Notification.h"
#include <iostream>
#include "../interface/ISend.h"

#include "cocos2d.h"

using namespace cocos2d;
namespace Engine
{
	Facade::Facade(void)
	{
		m_controller = new Controller(this);
		m_view = new View(this);
		m_model = new Modele();
		m_observer = new Observer();
		m_pModuleManager = CCModuleManager::getInstance();
	}

	Facade::~Facade(void)
	{
		delete(m_controller);
		m_controller = NULL;
		delete(m_view);
		m_view = NULL;
		delete(m_model);
		m_model = NULL;
		CC_SAFE_DELETE(m_observer);
		CCModuleManager::Release();
	}

	void Facade::registerCommand( const std::string & notificationName, ICommand * command) 
	{
		m_controller->registerCommand( notificationName, command );
		command->setFacade(this);
		m_pModuleManager->registerCommand(notificationName);
	}

	void Facade::removeCommand( const std::string & notificationName) 
	{
		if(m_controller!=NULL)
		{
			//CCLog("removeCommand by notificationName:%s",notificationName.c_str());
			m_controller->removeCommand( notificationName );
		}
	}

	bool Facade::hasCommand( const std::string & notificationName) 
	{
		return m_controller->hasCommand(notificationName);
	}

	void Facade::registerProxy ( IProxy * proxy)	
	{
		m_model->registerProxy ( proxy );
		proxy->setFacade(this);
		m_pModuleManager->registerProxy(proxy->getName());
	}

	IProxy * Facade::retrieveProxy ( const std::string & proxyName)
	{
		return m_model->retrieveProxy ( proxyName );	
	}

	IProxy * Facade::removeProxy ( const std::string & proxyName)
	{
		if(m_model!=NULL)
		{
			//CCLog("removeProxy by proxyName:%s",proxyName.c_str());
			return m_model->removeProxy ( proxyName );	
		}else{
			return NULL;
		}
	}

	bool Facade::hasProxy( const std::string & proxyName) 
	{
		return m_model->hasProxy( proxyName );
	}

	void Facade::registerMediator( IMediator * mediator) 
	{
		m_view->registerMediator( mediator );

		mediator->setFacade(this);
		m_pModuleManager->registerMediator(mediator->getName());
	}

	IMediator * Facade::retrieveMediator( const std::string & mediatorName)
	{
		return m_view->retrieveMediator( mediatorName ) ;
	}

	IMediator * Facade::removeMediator( const std::string & mediatorName) 
	{
		if(m_view!=NULL)
		{
			//CCLog("removeMediator by mediatorName:%s",mediatorName.c_str());
			return m_view->removeMediator( mediatorName );	
		}else{
			return NULL;
		}
	}

	bool Facade::hasMediator( const std::string & mediatorName) 
	{
		return m_view->hasMediator( mediatorName );
	}

	void Facade::sendNotification(INotification & noti ) 
	{
		if(noti.getName() != "MSG_OTHER_MAIN_UPDATE")
		{
			CCLOG("FacadeIn:%s",noti.getName().c_str());
		}
	
		noti.autoReleaseObject();
		m_observer->sendNotification(noti);
		if(noti.getName() != "MSG_OTHER_MAIN_UPDATE")
		{
			CCLOG("FacadeOut:%s",noti.getName().c_str());
		}
		
	}

	void Facade::sendNotification(const std::string & name)
	{
		Notification noti(name); 
		sendNotification(noti);
	}


	void Facade::addModule( CCIModule *module )
	{
		m_pModuleManager->addModule(module);
	}

	void Facade::removeModule( const string &name )
	{
		m_pModuleManager->removeModule(name);
	}

	void Facade::removeAllModule()
	{
		m_pModuleManager->removeAll();
	}

	void Facade::addObsever( const string &notifyName,ICommand *command )
	{
		m_observer->addObsever(notifyName,command);
	}

	void Facade::addObsever( const Interests &interests,ICommand *command )
	{
		m_observer->addObsever(interests,command);
	}

	void Facade::removeObsever( const string &notifyName,ICommand *command )
	{
		m_observer->removeObsever(notifyName,command);
	}

	int Facade::getNumOfModule()
	{
		return m_pModuleManager->getNumOfModule();
	}

	int Facade::getNumOfCommand()
	{
		return m_controller->getNumOfCommand();
	}

	int Facade::getNumOfMediator()
	{
		return m_view->getNumOfMediator();
	}

	int Facade::getNumOfProxy()
	{
		return m_model->getNumOfProxy();
	}

	int Facade::getNumOfObserver()
	{
		return m_observer->getNumOfObserver();
	}

	ICommand* Facade::retrieveCommand( const std::string & notiName )
	{
		return m_controller->retriveCommand(notiName);
	}

	void Facade::removeModuleExcept( const set<string> &setName )
	{
		m_pModuleManager->removeModuleExcept(setName);
	}
}