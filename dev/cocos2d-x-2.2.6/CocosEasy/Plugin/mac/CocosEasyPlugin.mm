//
//  CocosEasyPlugin.cpp
//  client
//
//  Created by liulihua on 14-2-20.
//
//

#import "CocosEasyPlugin.h"
#import "AppPushNotificationManager.h"
#import "NSDate+Additions.h"
#import "NSString+Additions.h"
#import "NSAlert+Blocks.h"
#import "EAGLView.h"

const std::string CCEPlugin::getDeviceID(){
    NSString* deviceId = [NSString uniqueDeviceIdentifier];
    return [deviceId UTF8String];
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
    return [@"mac" UTF8String];
}
const int CCEPlugin::getOSVersion(){
    return 108;

}
const int CCEPlugin::isJailBreak(){    //是否越狱  【0:未越狱，1：越狱】
    return 0;
}
void CCEPlugin::openURL(std::string url) {
    NSString *msg = [NSString stringWithUTF8String:url.c_str()] ;
    NSURL *nsUrl = [NSURL URLWithString:msg];
    [[NSWorkspace sharedWorkspace] openURL:nsUrl];
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
    
    NSArray *otherButtonTitles = [NSArray arrayWithObjects:nsBtn2Title, nil];

    NSWindow *window = [[EAGLView sharedEGLView] window];
    [NSAlert showSheetModalForWindow:window message:nsTitle informativeText:nsMsg alertStyle:NSInformationalAlertStyle cancelButtonTitle:nsBtn1Title otherButtonTitles:otherButtonTitles onDismiss:^(int btnIndex){
        if (handler){
            CCArray* pArrayArgs = CCArray::createWithCapacity(1);
            pArrayArgs->addObject(CCInteger::create(btnIndex));
            CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(handler,pArrayArgs);
        }
    }onCancel:^(void){
        
    }];
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
    
    NSArray *otherButtonTitles = [NSArray arrayWithObjects:nsBtn2Title, nil];
    
    NSWindow *window = [[EAGLView sharedEGLView] window];
    [NSAlert showSheetModalForWindow:window message:nsTitle informativeText:nsMsg alertStyle:NSInformationalAlertStyle cancelButtonTitle:nsBtn1Title otherButtonTitles:otherButtonTitles onDismiss:^(int btnIndex){
        if (pTarget && callback){
            (pTarget->*callback)(CCInteger::create(btnIndex));
        }
    }onCancel:^(void){
        if (pTarget && callback){
            (pTarget->*callback)(CCInteger::create(1));
        }
    }];
}

void CCEPlugin::messageBox2Callback(int index){
    
}

void CCEPlugin::setClipboardTxt(const char* pTtxt){
    
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
}
void CCEPlugin::playVibration(){
}

