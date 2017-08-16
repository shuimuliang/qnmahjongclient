//
//  APPSDKBase.m
//  client
//
//  Created by liulihua on 14-8-13.
//
//

#import "APPSDKBase.h"
#import "SDKManager.h"
#import "AppEventLogger.h"

@implementation APPSDKBase
-(void)logSdkEvent:(const char *)eventID errCode:(int)errCode{
    NSString* tag = [NSString stringWithFormat:@"%d_%d",CSDKManager::getInstance()->getChannelID(),errCode];
    NSString* nsEventID = eventID == nil?@"":[NSString stringWithFormat:@"%s",eventID];
    [AppEventLogger logEvent:nsEventID eventLabel:tag];
}
@end

void CSDKManager::createRoleAnayasis(unsigned int serverID){
}

void CSDKManager::enterMainSceneAnayasis(unsigned int serverID){
}
LanguageType CSDKManager::getDesignLanguage(){
#ifdef CASINO_LANGUAGE_ENGLISH
    return kLanguageEnglish;
#endif
    return kLanguageChinese;
}