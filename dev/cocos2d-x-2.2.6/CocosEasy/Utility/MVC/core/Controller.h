#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <string>
#include <map>
#include "../interface/ICommand.h"

namespace Engine
{

	class Facade;
	class Controller
	{
	public:
		Controller(Facade* pFacade);

		~Controller(void);


		void registerCommand(const std::string & notificationName, ICommand * ); 

		void removeCommand(const std::string & notificationName); 
		ICommand* retriveCommand(const std::string notificantionName);
		bool hasCommand(const std::string & notificationName); 

		int getNumOfCommand();
	private:
		typedef std::map<std::string,ICommand * >  CommandMap; 
		typedef CommandMap::iterator CommandMapItr; 
		CommandMap m_commandMap;
		Facade* mFacade;
	};

}

#endif //__CONTROLLER_H__

