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


void CAnalysis::initAnalysis(){
    [AppEventLogger initEventLogger];
}

void CAnalysis::logEvent(const char *eventID,const char *eventTag){
    if (eventID == NULL) return;
    NSString *eventName = eventID ? [NSString stringWithUTF8String:eventID]: NULL;
    NSString *eventLabel = eventTag ? [NSString stringWithUTF8String:eventTag]: NULL;
    [AppEventLogger logEvent:eventName eventLabel:eventLabel];
}
void CAnalysis::logPurchase(const char* parmas){
}
void CAnalysis::logRegister(const char* parmas){
}
void CAnalysis::logLogin(const char* parmas){
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

const char*  CAnalysis::getAdID(){
    return "";
}

static NSString * const kAppGlobalTalkingDataKey = @"A7E05A017D3A23112759005B80ECED75";

@implementation AppEventLogger

static AsyncUdpSocket * __udpSocket;

+ (void)initEventLogger{
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



@end
