#include "Modele.h"
#include "Observer.h"

namespace Engine
{

	Modele::Modele(void)
	{
	}


	Modele::~Modele(void)
	{
		IProxy *pProxy = NULL;
		for(ProxyMapItr itr = m_proxyMap.begin();itr!=m_proxyMap.end();++itr){
			pProxy = itr->second;
			if(pProxy !=NULL){
				CCObject *objProxy = dynamic_cast<CCObject*>(pProxy);
				if(objProxy){
					//CCLOG("=================================mediator name = %s",mediator->getName().c_str());
					objProxy->release();
				}else{
					delete(pProxy);
				}
				pProxy = NULL;
			}
		}
		m_proxyMap.clear();
	}

	void Modele::registerProxy( IProxy * proxy )
	{
		CC_ASSERT(!proxy->getName().empty());
		m_proxyMap[ proxy->getName() ] = proxy;
	}

	IProxy * Modele::retrieveProxy( const std::string & proxyName )
	{
		ProxyMapItr itr = m_proxyMap.find(proxyName); 
		if (itr != m_proxyMap.end())
		{
			return itr->second; 
		}
		return NULL; 
	}

	IProxy * Modele::removeProxy( const std::string & proxyName )
	{
		ProxyMapItr itr = m_proxyMap.find(proxyName); 
		IProxy * pProxy = NULL; 
		if (itr != m_proxyMap.end())
		{
			pProxy = itr->second; 
			CCObject *objProxy = dynamic_cast<CCObject*>(pProxy);
			if(objProxy){
				//CCLOG("=================================mediator name = %s",mediator->getName().c_str());
				objProxy->release();
			}else{
				delete(pProxy);
			}
			pProxy = NULL;
			m_proxyMap.erase(itr); 
		}
		return pProxy;
	}

	bool Modele::hasProxy( const std::string & proxyName )
	{
		return m_proxyMap.end() !=  m_proxyMap.find(proxyName); 
	}

	int Modele::getNumOfProxy()
	{
		CCLOG("========RestProxy==========");
		ProxyMapItr itr = m_proxyMap.begin();
		while(itr!=m_proxyMap.end()){
			CCLOG("proxy:%s",itr->first.c_str());
			++itr;
		}
		return m_proxyMap.size();
	}

}