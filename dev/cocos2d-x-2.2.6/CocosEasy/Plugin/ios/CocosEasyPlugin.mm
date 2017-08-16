//
//  CocosEasyPlugin.cpp
//  client
//
//  Created by liulihua on 14-2-20.
//
//

#import "CocosEasyPlugin.h"
#import "UIDevice+IdentifierAddition.h"
#import "PSAlertView.h"
#import "AppPushNotificationManager.h"
#import "UIDevice+Additions.h"
#import "NSDate+Additions.h"
#import <AVFoundation/AVFoundation.h>
#import "LocationManager.h"

const std::string CCEPlugin::getDeviceID(){
    NSString* deviceId = [[UIDevice currentDevice] casinoUniqueDeviceIdentifier];
    if(deviceId == nil) return "";
    return [deviceId UTF8String];
}

const std::string CCEPlugin::getAndroidSDCardPath(){
    return "";
}

unsigned int CCEPlugin::isIOSSimulator()
{
#ifdef DEBUG
    return 1;
#endif

    
#if TARGET_IPHONE_SIMULATOR
    return 1;
#endif
    return 0;
}

const std::string CCEPlugin::getDeviceModel(){
    NSString *modelString = [UIDevice deviceModelString];
    if (modelString == NULL) return "";
    return [[UIDevice deviceModelString] UTF8String];
}
const int CCEPlugin::getOSVersion(){
    return (int)[UIDevice deviceOSVersion] ;

}
const int CCEPlugin::isJailBreak(){    //是否越狱  【0:未越狱，1：越狱】
    if ([[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"cydia://"]]) {
        return 1;
    }
    return 0;
}

//设备高，低配置  【0:低配置设备，1:高配置设备】
const int CCEPlugin::getDeviceLV(){
    return 1;
}

void CCEPlugin::openURL(std::string url) {
    NSString *msg = [NSString stringWithUTF8String:url.c_str()] ;
    NSURL *nsUrl = [NSURL URLWithString:msg];
    [[UIApplication sharedApplication] openURL:nsUrl];
}

void CCEPlugin::messageBox(const char* title,
                           const char* msg,
                           const char* button1Title,
                           const char* button2Title,
                           int handler){
    NSString * nsTitle =  title ? [NSString stringWithUTF8String :title]:nil;
    NSString * nsMsg =  msg?[NSString stringWithUTF8String : msg]:nil;
    NSString *nsBtn1Title = button1Title?[NSString stringWithUTF8String:button1Title]:nil;
    NSString *nsBtn2Title = button2Title?[NSString stringWithUTF8String:button2Title]:nil;

    PSAlertView *alert = [PSAlertView alertWithTitle:nsTitle message:nsMsg];
    
    if (nsBtn1Title) {
        [alert addButtonWithTitle:nsBtn1Title block:^(void){
            if (handler){
                CCArray* pArrayArgs = CCArray::createWithCapacity(1);
                pArrayArgs->addObject(CCInteger::create(1));
                CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(handler,pArrayArgs);
            }
        }];
    }
    if (nsBtn2Title){
        [alert addButtonWithTitle:nsBtn2Title block:^(void){
            if (handler){
                CCArray* pArrayArgs = CCArray::createWithCapacity(1);
                pArrayArgs->addObject(CCInteger::create(2));
                CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(handler,pArrayArgs);
            }
        }];
    }
    [alert show];
}

void CCEPlugin::messageBox2(const char* title,
                       const char* msg,
                       const char* button1Title,
                       const char* button2Title,
                       CCObject *pTarget,
                    SEL_CallFuncO callback){
    NSString * nsTitle =  title ? [NSString stringWithUTF8String :title]:nil;
    NSString * nsMsg =  msg?[NSString stringWithUTF8String : msg]:nil;
    NSString *nsBtn1Title = button1Title?[NSString stringWithUTF8String:button1Title]:nil;
    NSString *nsBtn2Title = button2Title?[NSString stringWithUTF8String:button2Title]:nil;
    
    PSAlertView *alert = [PSAlertView alertWithTitle:nsTitle message:nsMsg];
    if (nsBtn1Title) {
        [alert addButtonWithTitle:nsBtn1Title block:^(void){
            if (pTarget && callback){
                (pTarget->*callback)(CCInteger::create(1));
            }
        }];
    }
    if (nsBtn2Title){
        [alert addButtonWithTitle:nsBtn2Title block:^(void){
            if (pTarget && callback){
                (pTarget->*callback)(CCInteger::create(2));
            }        }];
    }
    [alert show];
}

void CCEPlugin::messageBox2Callback(int index){
    
}

void CCEPlugin::setClipboardTxt(const char* pTtxt){
    UIPasteboard *pb = [UIPasteboard generalPasteboard];
    [pb setString:[NSString stringWithUTF8String:pTtxt]];
}

void CCEPlugin::startNotification(E_Noti_T notiType,int repeatDays ,int timeByMin , const char *title , const char *msg ,int tagId){
    
    if(notiType == eTimet){
        NSDate *date = [NSDate dateWithTimeIntervalSinceNow:timeByMin*60];
        NSString *message = [NSString stringWithUTF8String:msg == NULL ? "null msg":msg];
        [[AppPushNotificationManager sharedManager] scheduleLocalNotification:date message:message notificationID:tagId];
    }else if(notiType == eTimet){
        for (int i = 0; i<repeatDays; i++) {
            NSInteger hour = timeByMin/60;
            NSInteger minute = timeByMin%60;
            NSDate *date = [NSDate dateByHour:hour minute:minute daysLater:i];
            NSString *message = [NSString stringWithUTF8String:msg == NULL ? "null msg":msg];
            [[AppPushNotificationManager sharedManager] scheduleLocalNotification:date message:message notificationID:tagId];
        }
    }
}



void CCEPlugin::closeAlarmNotification(int tagID){
    [[AppPushNotificationManager sharedManager] cancelLocalNotification:tagID];
}
void CCEPlugin::setIdleTimerDisabled(bool disabled){
    [[UIApplication sharedApplication] setIdleTimerDisabled: disabled];
}
void CCEPlugin::playVibration(){
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}

double CCEPlugin::getLatitude() {
    return [[LocationManager sharedInstance] getLatitude];
}

double CCEPlugin::getLongitude() {
    return [[LocationManager sharedInstance] getLongitude];
}

bool CCEPlugin::locationServicesEnabled() {
    return [[LocationManager sharedInstance] locationServicesEnabled];
}

void CCEPlugin::requestLocationService() {
    return [[LocationManager sharedInstance] requestLocationService];
}
