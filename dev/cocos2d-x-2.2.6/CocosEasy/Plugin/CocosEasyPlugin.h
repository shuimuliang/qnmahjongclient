//
//  CocosEasyPlugin.h
//  client
//
//  Created by liulihua on 14-2-20.
//
//
#include "Common/CocosEasyDef.h"

#ifndef client_CocosEasyPlugin_h
#define client_CocosEasyPlugin_h
enum E_Noti_T{
	eDelayt = 0,
	eTimet	= 1,
};

static CCObject *pMsgBoxTarget;
static SEL_CallFuncO msgBoxCallback;

class CCEPlugin{
public:
    //device info
    static const std::string getDeviceID();
    static unsigned int isIOSSimulator();
    static const std::string getDeviceModel();  //ex: iPhone4S、iPad、GALAXY S5
    static const int getOSVersion();   //ex: 401 701
    static const int isJailBreak();    
    static const int getDeviceLV();   
    static const std::string getAndroidSDCardPath();
    
    // common method
    static void openURL(std::string url);
	static void messageBox(const char* title,
                           const char* msg,
                           const char* button1Title,
                           const char* button2Title,
                           int handler);
    static void messageBox2(const char* title,
                           const char* msg,
                           const char* button1Title,
                           const char* button2Title,
                           CCObject *pTarget,
                           SEL_CallFuncO callback);
    static void messageBox2Callback(int index);

    static void setClipboardTxt(const char* pTtxt);
    // send local notification
    static void startNotification(E_Noti_T notiType,int repeatDays ,int timeByMin , const char *title , const char *msg ,int tagId);

	static void closeAlarmNotification(int id = 0);
    
	// lock screen
    static void setIdleTimerDisabled(bool disabled);

    //play Vibrate
    static void playVibration();

    
	static E_Noti_T transNotiTFromInt(int t){ return t>=eDelayt&&t<=eTimet?(E_Noti_T)t:eDelayt;}
    
    static double getLatitude();
    static double getLongitude();
    static bool locationServicesEnabled();
    static void requestLocationService();

};


#endif
