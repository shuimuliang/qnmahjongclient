#ifndef _NOTIFICATION_H_
#define _NOTIFICATION_H_

#include <string>
#include "../../interface/INotification.h"

namespace Engine
{

	class Notification:public INotification
	{

	public:
		Notification(const Notification& noti);
		Notification& operator=(const Notification& noti);
		Notification(const char * name);

		Notification(const std::string & name);

		Notification( const std::string & name, void* body, const std::string & type);

		inline const std::string& getName() const { return m_name;}
		inline void setBody( void* body ){this->m_body = body;}
		inline void* getBody() { return m_body;}
		inline void setType( const std::string & type){this->m_type = type;}
		inline const std::string& getType(){return m_type;}
		void autoReleaseObject() ;
		inline CCObject* getCCObjectType(){return m_ccObjectType;}
		void addCCObjectType(CCObject* obj);
		std::string toString();
		void setSelector(SEL_MenuHandler pfnSelector);
		SEL_MenuHandler getSelector(); 
	private:
		// the name of the notification instance
		std::string m_name;
		// the type of the notification instance
		std::string m_type;
		// the body of the notification instance
		void*  m_body;
		SEL_MenuHandler m_pfnSelector; 
		CCObject* m_ccObjectType;
		void resetAll();
	};

}
#endif //_NOTIFICATION_H_


