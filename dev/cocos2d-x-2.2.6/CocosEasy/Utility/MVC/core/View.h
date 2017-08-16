#ifndef __VIEW_H__
#define __VIEW_H__

#include <string>
#include <map>
#include "../interface/IMediator.h"

namespace Engine
{

	class Facade;
	class View
	{

	public:
		View(Facade *pFacade);

		~View();

		void registerMediator( IMediator *mediator);

		IMediator * retrieveMediator(const std::string &  mediatorName);

		IMediator * removeMediator( const std::string & mediatorName);

		bool hasMediator( const std::string & mediatorName);

		int getNumOfMediator();
	private:
		typedef std::map<std::string,IMediator * >  MediatorMap; 
		typedef MediatorMap::iterator MediatorMapItr; 
		MediatorMap m_mediatorMap;
		Facade* mFacade;
	};

}

#endif //__VIEW_H__