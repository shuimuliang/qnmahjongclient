//
//  AppEventLogger.m
//  client
//
//  Created by liulihua on 14-4-10.
//
//

#import "AppEventLogger.h"
#import "SDKManager.h"
#import "Analysis.h"
#import "AsyncUdpSocket.h"
#import "JSONKit.h"
#import "TalkingDataGA.h"
#import "AppFlyerManager.h"
#import <AdSupport/AdSupport.h>
#import "AdTrakingManager.h"

void CAnalysis::initAnalysis(){
    [AppEventLogger initEventLogger];
}

void CAnalysis::logEvent(const char *eventID,const char *eventTag){
    if (eventID == NULL) return;
    NSString *eventName = eventID ? [NSString stringWithUTF8String:eventID]: NULL;
    NSString *eventLabel = eventTag ? [NSString stringWithUTF8String:eventTag]: NULL;
    [AppEventLogger logEvent:eventName eventLabel:eventLabel];
}
void CAnalysis::pageviewStart(const char *pageName){
    if (pageName == NULL) return;
    NSString* name = [NSString stringWithUTF8String:pageName];
    [AppEventLogger pageviewStartWithName:name];
}
void CAnalysis::pageviewEnd(const char *pageName){
    if (pageName == NULL) return;
    NSString* name = [NSString stringWithUTF8String:pageName];
    [AppEventLogger pageviewEndWithName:name];
}

//void CAnalysis::logPurchase(const char* orderId, const char* iapId, double currencyAmount, const char* currencyType, double virtualCurrencyAmount, const char* paymentType) {
//    if (NULL == orderId || strlen(orderId) == 0) {
//        return;
//    }
//    NSString* id = [NSString stringWithUTF8String:orderId];
//    NSString* iap = (iapId) ? [NSString stringWithUTF8String : iapId] : @"";
//    NSString* ctype = (currencyType) ? [NSString stringWithUTF8String : currencyType] : @"";
//    NSString* ptype = (paymentType) ? [NSString stringWithUTF8String : paymentType] : @"";
//    
//    NSMutableDictionary *dic = [NSMutableDictionary dictionaryWithCapacity:1];
//    [dic setObject:iap forKey:@"iapID"];
//    [dic setObject:ptype forKey:@"paymentType"];
//    [dic setObject:ctype forKey: @"currencyType"];
//    [dic setObject:[NSNumber numberWithDouble:currencyAmount] forKey:@"currencyAmount" ];
//    [AppFlyerManager trackPurchaseEvent:dic];
//}

void CAnalysis::logRegister(const char* parmas){
    NSString * playerData =  parmas?[NSString stringWithUTF8String : parmas]:nil;
    NSDictionary *dataDic = [playerData objectFromJSONString];
    NSString * playerID = [dataDic objectForKey:@"playerID"];
    [AdTrakingManager onRegister:playerID];
}
void CAnalysis::logLogin(const char* parmas){
    NSString * playerData =  parmas?[NSString stringWithUTF8String : parmas]:nil;
    NSDictionary *dataDic = [playerData objectFromJSONString];
    NSString * playerID = [dataDic objectForKey:@"playerID"];
    [AdTrakingManager onLogin:playerID];
}

void CAnalysis::logPurchase(const char* parmas){
    NSString * goodData =  parmas?[NSString stringWithUTF8String : parmas]:nil;
    NSDictionary *dataDic = [goodData objectFromJSONString];
    
    NSString * playerID = [dataDic objectForKey:@"playerID"];
    NSString * orderID = [dataDic objectForKey:@"orderID"];
    NSString * iapID = [dataDic objectForKey:@"iapID"];
    NSString * currencyType = [dataDic objectForKey:@"currencyType"];
    NSString * paymentType = [dataDic objectForKey:@"paymentType"];
    NSNumber * currencyAmount = [dataDic objectForKey:@"currencyAmount"];   //单位是  元
    NSNumber * virtualCurrencyAmount = [dataDic objectForKey:@"virtualCurrencyAmount"];
    
    if(!currencyAmount) currencyAmount = [NSNumber numberWithInt:0];
    if(!virtualCurrencyAmount) virtualCurrencyAmount = [NSNumber numberWithInt:0];
    
    // app flyer log
    [AppFlyerManager trackPurchaseEvent:iapID
                            paymentType:paymentType
                           currencyType:currencyType
                         currencyAmount:currencyAmount];
    
    int intAmount = (int)[currencyAmount doubleValue] * 100;
    //adtraking
    [AdTrakingManager onPay:playerID
                withOrderId:orderID
                 withAmount:intAmount
           withCurrencyType:currencyType
                withPayType:paymentType];
}

const char*  CAnalysis::getAdID(){
    NSString * idfa = [AppEventLogger getAdvertisingIdentifier];
    idfa = idfa ? idfa : @"";
    return [idfa UTF8String];
}

@implementation AppEventLogger

static AsyncUdpSocket * __udpSocket;

+ (void)initEventLogger{
    NSInteger platFrom = CSDKManager::getInstance()->getChannelID();
    
    NSString *channelName = @"unknown";
    
    channelName = [NSString stringWithFormat:@"channel_%ld",(long)platFrom];
    
#ifdef CASINO_TALKING_GAME_CHANNEL_NAME
    
    channelName = CASINO_TALKING_GAME_CHANNEL_NAME;
    
#endif
    
    [TalkingDataGA onStart:CASINO_TALKINGDATA_KEY withChannelId:channelName];

    /*
    __udpSocket = [[AsyncUdpSocket alloc] initWithDelegate:nil];
    [TalkingData setSignalReportEnabled:YES];
    [TalkingData setExceptionReportEnabled:YES];
    [TalkingData openDebugLog:YES];
    NSInteger platFrom = CSDKManager::getInstance()->getChannelID();

    NSString *channelName = @"unknown";
#ifdef DEBUG
    channelName = [NSString stringWithFormat:@"debug_%d",platFrom];
#else
    channelName = [NSString stringWithFormat:@"channel_%d",platFrom];
#endif
    [TalkingData sessionStarted:kAppGlobalTalkingDataKey withChannelId:channelName];
     */
}

+ (void)logEvent:(NSString*) eventId eventLabel:(NSString*)eventLabel {
#if 0
    if (eventId == nil || [eventId isEqualToString:@""]) return;
    if (eventLabel == nil) {
        [TalkingData trackEvent:eventId];
    } else {
        [TalkingData trackEvent:eventId label:eventLabel];
    }
    
    //send to our server
    const int channelID = CSDKManager::getInstance()->getChannelID();
    const char * playerID = CAnalysis::getInstance()->getPlayerID().c_str();
    int serverID = CAnalysis::getInstance()->getServerID();

    NSString* nsPlayerID = [NSString stringWithUTF8String:playerID == NULL?"":playerID];
    NSMutableDictionary *jsonDic = [NSMutableDictionary dictionaryWithCapacity:0];
    [jsonDic setObject:eventId forKey:@"eID"];
    [jsonDic setObject:[NSNumber numberWithInt:channelID] forKey:@"ch"];
    [jsonDic setObject:[NSNumber numberWithInt:serverID] forKey:@"sID"];

    if (nsPlayerID != NULL && ![nsPlayerID isEqualToString:@""]){
        [jsonDic setObject:nsPlayerID forKey:@"uID"];
    }
    if (eventLabel != NULL && ![eventLabel isEqualToString:@""]){
        //avoid length > 15
        if (eventLabel.length > 15){
            eventLabel = [eventLabel substringToIndex:15];
        }
        [jsonDic setObject:eventLabel forKey:@"eTag"];
    }
    if(__udpSocket == nil) return;
#ifdef DEBUG
    NSLog(@"send to our server : \n %@",jsonDic);
#endif
    [__udpSocket sendData:[jsonDic JSONData] toHost:[NSString stringWithUTF8String:ANALYSIS_SERVER_IP] port:ANALYSIS_SERVER_PORT withTimeout:20 tag:0];
#endif
}

+ (void)pageviewStartWithName:(NSString*) name {
//    [TalkingData trackPageBegin:name];
}

+ (void)pageviewEndWithName:(NSString*) name {
    
//    [TalkingData trackPageEnd:name];
}

+ (NSString *)getAdvertisingIdentifier{
     return [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
}

@end
