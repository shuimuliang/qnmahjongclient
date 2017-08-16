//
//  CocosEasyPlugin.cpp
//  client
//
//  Created by liulihua on 14-2-20.
//
//

#include "Plugin/CocosEasyPlugin.h"
#include <winsock2.h>
#include <Iphlpapi.h>
#include <stdio.h>
#pragma comment(lib,"Iphlpapi.lib")
const std::string CCEPlugin::getDeviceID()
{
	char mac[1024]= {0};
	ULONG ulSize=0;
	PIP_ADAPTER_INFO pInfo=NULL;
	PIP_ADAPTER_INFO pHeadInfo=NULL;
	int temp=0;
	temp = GetAdaptersInfo(pInfo,&ulSize);//µ⁄“ª¥¶µ˜”√£¨ªÒ»°ª∫≥Â«¯¥Û–°
	pInfo=(PIP_ADAPTER_INFO)malloc(ulSize);
	temp = GetAdaptersInfo(pInfo,&ulSize);

	int iCount=0;
	pHeadInfo = pInfo;
	while(pInfo)//±È¿˙√ø“ª’≈Õ¯ø®
	{
		//  pInfo->Address MAC÷∑
		for(int i=0;i<(int)pInfo->AddressLength;i++)
		{
			int high = pInfo->Address[i]/16;
			int low = pInfo->Address[i]%16;
			mac[iCount] =  (high <10)?('0'+high):('A'+high-10);
			mac[iCount+1] =(low <10)?('0'+low):('A'+low-10);
			iCount+=2;
		}
		pInfo = pInfo->Next;
		break;
	}

	mac[--iCount]='\0';
	std::string deviceID;
	deviceID = mac;
	CC_SAFE_DELETE(pHeadInfo);
	return "hdydoooYYYYHHudddyyutt";//deviceID;
}



unsigned int CCEPlugin::isIOSSimulator()
{
    return 0;
}

void CCEPlugin::openURL(std::string url) {
	ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void CCEPlugin::messageBox(const char* title,const char* msg,const char* button1Title,const char* button2Title,int handler )
{
	UINT boxType = MB_OK;
	if (button1Title && button2Title)
	{
		boxType = MB_OKCANCEL;
	}
	int result = MessageBoxA(NULL,msg, title, boxType);

	CCArray* pArrayArgs = CCArray::createWithCapacity(1);
	pArrayArgs->addObject(CCInteger::create(result));

	if (handler)
		CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(handler,pArrayArgs);

}
void CCEPlugin::messageBox2(const char* title,
                            const char* msg,
                            const char* button1Title,
                            const char* button2Title,
                            CCObject *pTarget,
                            SEL_CallFuncO callback){
    UINT boxType = MB_OK;
    if (button1Title && button2Title)
    {
        boxType = MB_OKCANCEL;
    }
    int result = MessageBoxA(NULL,msg, title, boxType);
    
    if (pTarget && callback){
        (pTarget->*callback)(CCInteger::create(result));
    }
    
}
void CCEPlugin::messageBox2Callback(int index){
    
}
void CCEPlugin::setClipboardTxt(const char* pTtxt )
{
	int len=strlen(pTtxt)+1;
    OpenClipboard(NULL);
    HGLOBAL hMem=GlobalAlloc(GHND,len);
    void *pMem=GlobalLock(hMem);
    EmptyClipboard();
    memcpy(pMem,pTtxt,len);
    SetClipboardData(CF_TEXT,hMem);
    CloseClipboard();
    GlobalFree(hMem);
}


void CCEPlugin::startNotification(E_Noti_T notiType,int repeatDays ,int timeByMin , const char *title , const char *msg ,int tagId)
{

}

void CCEPlugin::closeAlarmNotification(int id)
{

}
void CCEPlugin::setIdleTimerDisabled(bool disabled){

}

const std::string CCEPlugin::getDeviceModel()
{
	return "win32";
}

const int CCEPlugin::getOSVersion()
{
	return 100;
}

 const int CCEPlugin::getDeviceLV(){
	 return 1;
 }

const int CCEPlugin::isJailBreak(){
    return 0;
}
void CCEPlugin::playVibration(){
}