#ifndef __MODELE_H__
#define __MODELE_H__

#include "../interface/IProxy.h"
#include <string>
#include <map>

namespace Engine
{

	class Modele
	{
	public:
		Modele();

		~Modele();

		void registerProxy( IProxy * proxy );

		IProxy * retrieveProxy( const std::string & proxyName);

		IProxy * removeProxy( const std::string & proxyName);

		bool hasProxy(const std::string &  proxyName);

		int getNumOfProxy();
	private:
		typedef std::map<std::string,IProxy * >  ProxyMap; 
		typedef ProxyMap::iterator ProxyMapItr; 
		ProxyMap m_proxyMap;
	};

}

#endif //__MODELE_H__