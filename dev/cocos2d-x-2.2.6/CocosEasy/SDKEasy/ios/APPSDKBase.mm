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
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <CoreTelephony/CTCarrier.h>
#ifdef CASINO_WXAPPID
#include "WXApi.h"
#include "WXApiObject.h"
#endif

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

void CSDKManager::share(const char* params){
#ifdef CASINO_WXAPPID
    rapidjson::Document doc;
    doc.Parse<0>(params);
    if(doc.HasParseError())
    {
        NSLog(@"offset: %lu error:%s", doc.GetErrorOffset(), doc.GetParseError());
        return;
    }
    
    std::string type = DICTOOL->getStringValue_json(doc, "type", "text");
    std::string scene = DICTOOL->getStringValue_json(doc, "scene", "session");
    NSLog(@"type = %s", type.c_str());
    NSLog(@"scene = %s", scene.c_str());

    WXScene eScene = WXSceneSession;
    if (0 == scene.compare("session")) {
        eScene = WXSceneSession;
    }else if (0 == scene.compare("timeline")) {
        eScene = WXSceneTimeline;
    }else {
        NSLog(@"share scene error %s", scene.c_str());
        return;
    }
    if (0 == type.compare("text")) {
        std::string text = DICTOOL->getStringValue_json(doc, "text", "");
        NSLog(@"text = %s", text.c_str());

        SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
        req.text = [NSString stringWithUTF8String : text.c_str()];
        req.bText = YES;
        req.scene = eScene;
        [WXApi sendReq:req];
    }else if (0 == type.compare("link")) {
        WXMediaMessage *message = [WXMediaMessage message];
        std::string title = DICTOOL->getStringValue_json(doc, "title", "");
        message.title = [NSString stringWithUTF8String : title.c_str()];
        std::string description = DICTOOL->getStringValue_json(doc, "description", "");
        message.description = [NSString stringWithUTF8String : description.c_str()];
        [message setThumbImage:[UIImage imageNamed:@"Icon.png"]];
        
        WXWebpageObject *webpageObject = [WXWebpageObject object];
        std::string webpageUrl = DICTOOL->getStringValue_json(doc, "webpageUrl", "");
        webpageObject.webpageUrl = [NSString stringWithUTF8String : webpageUrl.c_str()];
        message.mediaObject = webpageObject;
        
        SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
        req.bText = NO;
        req.message = message;
        req.scene = eScene;
        [WXApi sendReq:req];
    }else if (0 == type.compare("image")) {
        WXMediaMessage *message = [WXMediaMessage message];
        std::string imagePath = DICTOOL->getStringValue_json(doc, "imagePath", "");
        std::string imageType = DICTOOL->getStringValue_json(doc, "imageType", "");
        [message setThumbImage:[UIImage imageNamed:[NSString stringWithFormat:@"%s.%s", imagePath.c_str(), imageType.c_str()]]];
        // 缩略图
        WXImageObject *imageObject = [WXImageObject object];
        NSString *filePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String : imagePath.c_str()]
                                                             ofType:[NSString stringWithUTF8String : imageType.c_str()]];
        imageObject.imageData = [NSData dataWithContentsOfFile:filePath];
        message.mediaObject = imageObject;
        
        SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
        req.bText = NO;
        req.message = message;
        req.scene = eScene;
        [WXApi sendReq:req];
    }
#endif
}

void CSDKManager::thirdLogin(int loginType){ // 第三方登陆
    NSString *uuid = [NSUUID UUID].UUIDString;
    const char *state =[uuid UTF8String];
    setWchatState(state);
#ifdef CASINO_WXAPPID
    //构造SendAuthReq结构体
    SendAuthReq* req =[[[SendAuthReq alloc ] init ] autorelease ];
    req.scope = @"snsapi_userinfo" ;
    req.state = uuid;
    //第三方向微信终端发送一个SendAuthReq消息结构
    [WXApi sendReq:req];
#endif
}


const std::string CSDKManager::getSimCountryISO(){
    CTTelephonyNetworkInfo *networkInfo = [[CTTelephonyNetworkInfo alloc] init];
    CTCarrier *carrier = networkInfo.subscriberCellularProvider;
    if (carrier && carrier.isoCountryCode) {
        return std::string([carrier.isoCountryCode UTF8String]);
    }
    
    return "";
}

bool CSDKManager::isWXAppInstalled()
{
#ifdef CASINO_WXAPPID
    bool bRet = [WXApi isWXAppInstalled];
    if (bRet == YES) {
        NSLog(@"安装微信了");
    }else{
        NSLog(@"没安装微信");
    }
    return bRet;
#endif
    return false;
}

LanguageType CSDKManager::getDesignLanguage(){
#ifdef CASINO_LANGUAGE_ENGLISH
    return kLanguageEnglish;
#elif CASINO_LANGUAGE_ARAB
    return kLanguageArabic;
#elif CASINO_LANGUAGE_TRADITIONAL
    return kLanguageChineseTraditional;
#elif CASINO_LANGUAGE_MALAYSIA
    return kLanguageMalaysia;
#endif
    return kLanguageChinese;
}
