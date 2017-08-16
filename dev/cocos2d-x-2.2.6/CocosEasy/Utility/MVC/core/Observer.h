#ifndef __I_OBSERVER_H__
#define __I_OBSERVER_H__

#include "../interface/ICommand.h"
#include "../interface/INotification.h"
#include "../interface/IMediator.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

namespace Engine
{

	class Observer
	{
	public:
		Observer(void);
		~Observer(void);
		void addObsever( const string &notifyName,ICommand *command);
		void addObsever(const Interests &interests,ICommand *command);
		void removeObsever(const string &notifyName,ICommand *command);
		void removeAll();
		void sendNotification(INotification &notify);
		int getNumOfObserver();
	private:
		typedef vector<ICommand *> CommandVec;
		typedef CommandVec::iterator CommandVecItr;

		typedef std::map<std::string,CommandVec>  ObseverMap; 
		typedef ObseverMap::iterator ObseverMapItr; 
		ObseverMap m_obseverMap;

	};

}
#endif //__I_OBSERVER_H__


