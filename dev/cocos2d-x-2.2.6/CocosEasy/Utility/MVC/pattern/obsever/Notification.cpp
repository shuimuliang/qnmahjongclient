#include "Notification.h"

namespace Engine
{

	Notification::Notification(const char * name)
	{
		resetAll();
		this->m_name = name;
	}

	Notification::Notification(const std::string & name)
	{
		resetAll();
		this->m_name = name; 
	}

	Notification::Notification( const std::string & name, void* body, const std::string & type="")
	{
		resetAll();
		this->m_name = name;
		this->m_type = type;
		this->m_body = body;
	}

	Notification::Notification(const Notification& noti){
		this->m_name = noti.m_name;
		this->m_body = noti.m_body;
		this->m_type = noti.m_type;
		this->m_ccObjectType= noti.m_ccObjectType;
		this->mIntNum = noti.mIntNum;
		this->mDoubelNum = noti.mDoubelNum;
	}

	std::string Notification::toString()
	{
		std::string msg= std::string("Notification Name: ")+getName();
		//msg += "\nBody:"+(( m_body == NULL )?"NULL":m_body.toString());
		msg += "\nType:"+(( m_type == "")?"":m_type);
		return msg;
	}

	Notification& Notification::operator=( const Notification& noti )
	{
		this->m_name = noti.m_name;
		this->m_body = noti.m_body;
		this->m_type = noti.m_type;
		this->m_ccObjectType= noti.m_ccObjectType;
		this->mIntNum = noti.mIntNum;
		this->mDoubelNum = noti.mDoubelNum;
		return *this;
	}

	void Notification::autoReleaseObject()
	{
		if(m_ccObjectType!=NULL){
			m_ccObjectType->autorelease();
		}
	}

	void Notification::resetAll()
	{
		m_name = "";
		m_type="";
		m_body=NULL;
		m_ccObjectType=NULL;
	}

	void Notification::addCCObjectType( CCObject* obj )
	{
		CC_SAFE_RELEASE(m_ccObjectType);
		m_ccObjectType = obj;
		m_ccObjectType->retain();
	}

	void Notification::setSelector( SEL_MenuHandler pfnSelector )
	{
		m_pfnSelector=pfnSelector;
	}

	cocos2d::SEL_MenuHandler Notification::getSelector()
	{
		return m_pfnSelector;
	}

}