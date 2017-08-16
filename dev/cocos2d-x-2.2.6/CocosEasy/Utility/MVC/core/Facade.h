#ifndef _FACADE_H_
#define _FACADE_H_
#include "Controller.h"
#include "View.h"
#include "Modele.h"
#include "../interface/IMediator.h"
#include "../CCIModule.h"
#include "../CCModuleManager.h"
#include "Observer.h"

namespace Engine
{
	class Facade
	{
	public:
		Facade(void);

		~Facade(void);

		void registerProxy(IProxy * proxy) ;

		IProxy * retrieveProxy(const std::string & proxyName); 

		IProxy * removeProxy(const std::string & proxyName); 

		bool hasProxy (const std::string & proxyName); 

		void registerCommand(const std::string & noteName, ICommand *); 

		void removeCommand(const std::string & notificationName);  
		ICommand* retrieveCommand(const std::string & notiName);

		bool hasCommand(const std::string & notificationName); 

		void registerMediator( IMediator * mediator); 

		IMediator *retrieveMediator(const std::string & mediatorName); 

		IMediator *removeMediator(const std::string & mediatorName); 

		bool hasMediator(const std::string & mediatorName); 

		void sendNotification(INotification &  ); 

		void sendNotification(const std::string & name);
		//addModule时触发onAdd事件
		void addModule(CCIModule *module);
		//移除时进行内存delete
		void removeModule(const string &name);
		void removeAllModule();
		void removeModuleExcept(const set<string> &setName);


		void addObsever( const string &notifyName,ICommand *command);
		void addObsever(const Interests &interests,ICommand *command);
		void removeObsever(const string &notifyName,ICommand *command);

		int getNumOfModule();
		int getNumOfCommand();
		int getNumOfMediator();
		int getNumOfProxy();
		int getNumOfObserver();
	protected :
		Controller * m_controller ;

		Modele *m_model;

		View *m_view;

		CCModuleManager *m_pModuleManager;

		Observer* m_observer;
	};
}
#endif //_FACADE_H_
