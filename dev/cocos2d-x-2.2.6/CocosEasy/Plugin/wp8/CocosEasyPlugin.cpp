//
//  CocosEasyPlugin.cpp
//  client
//
//  Created by liulihua on 14-2-20.
//
//

#include "Plugin/CocosEasyPlugin.h"

namespace PhoneDirect3DXamlAppComponent 
{
	public delegate Platform::String^ GetUniqueIDDelegate();
	public delegate int GetOSVersionDelegate();
	public delegate void ShowWebBroswerDelegate(Platform::String^ url); 
	public delegate void SetClipboardTextDelegate(Platform::String^ text);


	public ref class WP8Delegate sealed
	{
	public:
		WP8Delegate(void){}
		void SetGetUniqueIDDelegate(GetUniqueIDDelegate^ del)
		{
			m_getUniqueIDDelegate = del;
		}
		void SetGetOSVersionDelegate(GetOSVersionDelegate^ del)
		{
			m_getOSVersionDelegate = del;
		}
		void SetShowWebBroswerDelegate(ShowWebBroswerDelegate^ del) 
		{ 
			m_broswerDelegate = del; 
		} 
		void SetSetClipboardTextDelegate(SetClipboardTextDelegate^ del) 
		{ 
			m_clipboardTextDelegate = del; 
		} 

		Platform::String^ GetUniqueID()
		{            
			if(m_getUniqueIDDelegate)
			{
				return m_getUniqueIDDelegate->Invoke();
			}
			return "";
		}

		int GetOSVersion()
		{            
			if(m_getOSVersionDelegate)
			{
				return m_getOSVersionDelegate->Invoke();
			}
			return 0;
		}
		void ShowWebBroswer(Platform::String^ url) 
		{ 
			if (m_broswerDelegate) 
			{ 
				m_broswerDelegate->Invoke(url); 
			} 
		} 
		void SetClipboardText(Platform::String^ text) 
		{ 
			if (m_broswerDelegate) 
			{ 
				m_clipboardTextDelegate->Invoke(text); 
			} 
		} 
		///DeviceInfo end
	private:
		property static GetUniqueIDDelegate^ m_getUniqueIDDelegate;
		property static GetOSVersionDelegate^ m_getOSVersionDelegate;
		property static ShowWebBroswerDelegate^ m_broswerDelegate; 
		property static SetClipboardTextDelegate^ m_clipboardTextDelegate; 
	};

}
PhoneDirect3DXamlAppComponent::WP8Delegate^ manager;

Platform::String^ GetPlatformString(std::string stdstr){
	std::wstring ws;
	ws.assign(stdstr.begin(), stdstr.end());
	return ref new Platform::String(ws.c_str());
}
Platform::String^ GetPlatformString(const char* charpoint){
	std::string str = charpoint;
	return GetPlatformString(str);
}
std::string GetStdString(Platform::String^ pstr){
	std::wstring wstr(pstr->Data());
	std::string str(wstr.length(),' ');
	std::copy(wstr.begin(),wstr.end(),str.begin());
	return str;
}

const std::string CCEPlugin::getDeviceID(){
	if (!manager){	manager = ref new PhoneDirect3DXamlAppComponent::WP8Delegate();	}
	return GetStdString(manager->GetUniqueID());
}

unsigned int CCEPlugin::isIOSSimulator(){
    return 0;
}

const std::string CCEPlugin::getDeviceModel(){
    return "windows phone";
}
const int CCEPlugin::isJailBreak(){    //是否越狱  【0:未越狱，1：越狱】
    return 0;
}

const int CCEPlugin::getOSVersion(){
	if (!manager){	manager = ref new PhoneDirect3DXamlAppComponent::WP8Delegate();	}
    return manager->GetOSVersion();
}

void CCEPlugin::openURL(std::string url) {
	if (!manager){	manager = ref new PhoneDirect3DXamlAppComponent::WP8Delegate();	}
	manager->ShowWebBroswer(GetPlatformString(url)); 	
}

void CCEPlugin::setClipboardTxt(const char* pTtxt ){
	if (!manager){	manager = ref new PhoneDirect3DXamlAppComponent::WP8Delegate();	}
	manager->SetClipboardText(GetPlatformString(pTtxt)); 
}

void CCEPlugin::messageBox(const char* title,const char* msg,const char* button1Title,const char* button2Title,int handler )
{
	
}

// lock screen
static void setIdleTimerDisabled(bool disabled)
{

}

void CCEPlugin::startNotification(E_Noti_T notiType,int repeatDays ,int timeByMin , const char *title , const char *msg ,int tagId)
{

}

void CCEPlugin::closeAlarmNotification(int id)
{

}
void CCEPlugin::setIdleTimerDisabled(bool disabled){
    
}
void CCEPlugin::playVibration(){
}