#include "CCNetFacade.h"
#include "CCFacade.h"

namespace Engine
{

	static CCNetFacade *s_ince = NULL;
	CCNetFacade::QueueNotification CCNetFacade::m_queueNotification;

	CCNetFacade::QueueNotification CCNetFacade::m_queueNotificationFromLoading;

	pthread_mutex_t CCNetFacade::m_mutex_receive_queue;

	CCNetFacade * CCNetFacade::getInstance()
	{
		if(s_ince == NULL){
			pthread_mutex_init(&m_mutex_receive_queue,NULL);
			s_ince = new CCNetFacade();
		}
		return s_ince;
	}


	CCNetFacade::CCNetFacade():m_bIsRunUpdate(true)
	{

	}

	CCNetFacade::~CCNetFacade()
	{

	}

	void CCNetFacade::release()
	{
		CC_SAFE_DELETE(s_ince);
	}

	void CCNetFacade::sendNotificationToMainThread(Notification &noti)
	{
		pthread_mutex_lock(&m_mutex_receive_queue);
		m_queueNotification.push(noti);
		pthread_mutex_unlock(&m_mutex_receive_queue);
	}

	void CCNetFacade::sendNotificationToMainThread( const string& name )
	{
		Notification noti(name);
		sendNotificationToMainThread(noti);
	}

	void CCNetFacade::update()
	{
		pthread_mutex_lock(&m_mutex_receive_queue);
		updateNet();
		pthread_mutex_unlock(&m_mutex_receive_queue);

		pthread_mutex_lock(&m_mutex_receive_queue);
		updateLoading();
		pthread_mutex_unlock(&m_mutex_receive_queue);
	}

	void CCNetFacade::sendNotificationToMainThreadFromLoading( const string& name )
	{
		Notification noti(name);
		sendNotificationToMainThreadFromLoading(noti);
	}

	void CCNetFacade::sendNotificationToMainThreadFromLoading( Notification &noti )
	{
		pthread_mutex_lock(&m_mutex_receive_queue);
		m_queueNotificationFromLoading.push(noti);
		pthread_mutex_unlock(&m_mutex_receive_queue);
	}

	void CCNetFacade::updateNet()
	{
		while(!m_queueNotification.empty()&&m_bIsRunUpdate){
			Notification noti = m_queueNotification.front();
			CCLOG("NetFacadeIn:%s",noti.getName().c_str());
			m_queueNotification.pop();
			CCFacade::getInstance()->sendNotification(noti);
			CCLOG("NetFacadeOut:%s",noti.getName().c_str());
		}	
	}

	void CCNetFacade::updateLoading()
	{
		while(!m_queueNotificationFromLoading.empty()){
			Notification noti = m_queueNotificationFromLoading.front();
			m_queueNotificationFromLoading.pop();
			CCFacade::getInstance()->sendNotification(noti);
		}	
	}

}