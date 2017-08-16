#include "Observer.h"
#include <iostream>
#include <algorithm>

namespace Engine
{

	Observer::Observer(void)
	{
		//Nothing
	}


	Observer::~Observer(void)
	{
		removeAll();
	}

	void Observer::addObsever( const string &notifyName,ICommand *command )
	{
		CC_ASSERT(!notifyName.empty());
		ObseverMapItr mapItr = m_obseverMap.find(notifyName);
		if(mapItr!=m_obseverMap.end()){
			mapItr->second.push_back(command);
		}else {
			CommandVec vec;
			vec.push_back(command);
			m_obseverMap[notifyName] = vec;
		}
	}

	void Observer::addObsever( const Interests &interests,ICommand *command )
	{
		Interests::const_iterator itr = interests.begin();
		while(itr!=interests.end()){
			addObsever(*itr,command);
			++itr;
		}
	}

	void Observer::removeObsever( const string &notifyName,ICommand *command )
	{
		ObseverMapItr mapItr = m_obseverMap.find(notifyName);
		if(mapItr!=m_obseverMap.end()){
			CommandVec &comVec = mapItr->second;
			CommandVecItr itr = find(comVec.begin(),comVec.end(),command);
			while(itr!=comVec.end()){
				comVec.erase(itr);
				itr = find(comVec.begin(),comVec.end(),command);
			}
			if(comVec.empty()){
				m_obseverMap.erase(mapItr);
			}
		}
	}

	void Observer::removeAll()
	{
		m_obseverMap.clear();
	}

	void Observer::sendNotification(INotification &notify )
	{
		string s = notify.getName();
		ObseverMapItr mapItr = m_obseverMap.find(s);
		if(mapItr!=m_obseverMap.end()){
			CommandVec &comVec = mapItr->second;
			for(CommandVecItr itr = comVec.begin();itr!=comVec.end();++itr){
				ICommand *pCmd = *itr;
				(*itr)->execute(notify);
			}
		}
		else{
			//CCLOG("warning!!!Notification:%s is not execute",notify.getName().c_str());
		}
	}

	int Observer::getNumOfObserver()
	{
		int sum = 0;
		ObseverMapItr mapItr = m_obseverMap.begin();
		while(mapItr!=m_obseverMap.end())
		{
			sum += mapItr->second.size();
			++mapItr;
		}
		return sum;
	}

}