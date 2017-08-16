#include "View.h"
#include "Observer.h"
#include "Facade.h"
#include "cocos2d.h"

USING_NS_CC;

namespace Engine
{

	View::View(Facade *pFacade)
	{
		mFacade = pFacade;
	}

	View::~View(void)
	{
		IMediator *pMediator = NULL;
		for(MediatorMapItr itr = m_mediatorMap.begin();itr!=m_mediatorMap.end();++itr){
			pMediator = itr->second; 
			if(pMediator!=NULL){
				CCObject *objMediator = dynamic_cast<CCObject*>(pMediator);
				if(objMediator){
					//CCLOG("=================================mediator name = %s",mediator->getName().c_str());
					objMediator->release();
				}else{
					delete(pMediator);
				}
				pMediator = NULL;
			}
		}
		m_mediatorMap.clear();
		mFacade = NULL;
	}

	void View::registerMediator( IMediator *mediator )
	{
		CC_ASSERT(!mediator->getName().empty());
		MediatorMapItr itr = m_mediatorMap.find(mediator->getName()); 
		CCObject *objMediator = dynamic_cast<CCObject*>(mediator);
		if(objMediator){
			//CCLOG("=================================mediator name = %s",mediator->getName().c_str());
			objMediator->retain();
		}
		if (itr == m_mediatorMap.end())
		{
			mFacade->addObsever(mediator->listNotificationInterests(),mediator);
		}

		m_mediatorMap[ mediator->getName() ] = mediator;
	}

	IMediator * View::retrieveMediator( const std::string & mediatorName )
	{
		CC_ASSERT(mediatorName != "");
		MediatorMapItr itr = m_mediatorMap.find(mediatorName);
		if(itr!=m_mediatorMap.end()){
			return itr->second;
		}
		return NULL;
	}

	IMediator * View::removeMediator( const std::string & mediatorName )
	{
		// Retrieve the named mediator
		MediatorMapItr itr = m_mediatorMap.find(mediatorName); 

		IMediator * mediator = NULL;
		if ( itr != m_mediatorMap.end()) 
		{
			mediator = itr->second; 
			// for every notification this mediator is interested in...
			Interests interests= mediator->listNotificationInterests();
			for ( size_t i =0; i<interests.size(); i++ ) 
			{
				mFacade->removeObsever(interests.at(i),mediator);
			}	
			// remove the mediator from the map		
			m_mediatorMap.erase(mediatorName);
			CCObject *objMediator = dynamic_cast<CCObject*>(mediator);
			if(objMediator){
				//CCLOG("=================================mediator name = %s",mediator->getName().c_str());
				objMediator->release();
			}else{
				delete(mediator);
			}
			mediator = NULL;
		}
		return mediator;
	}

	bool View::hasMediator( const std::string & mediatorName )
	{
		return m_mediatorMap.find(mediatorName)!=m_mediatorMap.end();
	}

	int View::getNumOfMediator()
	{
		CCLOG("========RestMediator==========");
		MediatorMapItr itr = m_mediatorMap.begin();
		while(itr!=m_mediatorMap.end()){
			CCLOG("mediator:%s",itr->first.c_str());
			++itr;
		}
		return m_mediatorMap.size();
	}

}