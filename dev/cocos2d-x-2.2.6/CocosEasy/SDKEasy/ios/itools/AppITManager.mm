//
//  AppITManager.m
//  zhajinhua
//
//  Created by LiuLihua on 13-3-27.
//
//

#import "AppITManager.h"
#import "SDKManager.h"
#include <stdio.h>
#import "JSONKit.h"

void CSDKManager::initPlatform(int code){
     m_ChannelID = SDK_CHANNEL_ITOOLS;
    [AppITManager sharedInstance];
}

void CSDKManager::preLoadGoods(const char* goodsList){
}

void CSDKManager::login(){
    [[AppITManager sharedInstance] loginIT:^(BOOL succ){
        this->loginCallBack(succ);
    }];
}
void CSDKManager::logout(bool iscallback){
    [[AppITManager sharedInstance] logout:iscallback];
}
void CSDKManager::userCenter(){
    [[AppITManager sharedInstance] enterIT];
}
bool CSDKManager::isLogined(){
    return [[AppITManager sharedInstance] isLogined];
}
bool CSDKManager::isInited(){
    return [[AppITManager sharedInstance] isInitIT];
}

const std::string CSDKManager::getAccountName(){
    return [[[AppITManager sharedInstance] nickName] UTF8String];
}
const std::string CSDKManager::getAccountID(){
    return [[[AppITManager sharedInstance] loginUin] UTF8String];
}
const std::string CSDKManager::getSessionID(){
    return [[[AppITManager sharedInstance] sessionId] UTF8String];
}
void CSDKManager::unInit(){
    
}


void CSDKManager::pay(unsigned int ID,
         const char* name,
         unsigned int money,
         const char* description,
         const char* extraData){
    
    NSString * nsname =  name?[NSString stringWithUTF8String : name]:nil;
    NSString * nsdescription = description?[NSString stringWithUTF8String:description]:nil;
    NSString * nsextraData = extraData?[NSString stringWithUTF8String:extraData]:nil;
    
    [[AppITManager sharedInstance] buyGood:[NSString stringWithFormat:@"%d",ID]
                                     price:money
                                      name:nsname
                                       des:nsdescription
                                       ext:nsextraData
                               finishBlock:^(BOOL succ ,NSString * ord){
                                   this->buyCallBack(succ,[ord UTF8String]);
                               }];
    
}

static AppITManager * __instance = nil;


@interface AppITManager ()
@property (nonatomic, copy) AppITLoginFinishBlock loginFinish;
@property (nonatomic, copy) AppITBuyGoodsFinishBlock buyFinishBlock;
@end

@implementation AppITManager{
    BOOL _isInit;
    bool _isLogin;
    NSString * _sessionID;
    NSString * _nickName;
    NSString * _userID;
    NSString *_currentSeriaNo;
    bool _needLogoutCallback;
}

- (void)configureITSDKWhenBootup{
    [self addITObserver];
    //设置充值平台分配的 appid 和 appkey
    [HXAppPlatformKitPro setAppId:253 appKey:@"03213CAB6BA7907439E3FFA6A3E13724"];
    //设置支持的方向,根据自己 App 支持的方向做设置。
    [HXAppPlatformKitPro setSupportOrientationPortrait:YES portraitUpsideDown:YES landscapeLeft:NO landscapeRight:NO];
    //设置是否开启自动更新,默认为 NO,不开启
    [HXAppPlatformKitPro setAutoCheckAppUpdateEnabled:NO];
    _isInit = YES;
}

- (void)addITObserver{
    //监听注册通知
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(registerNotification:) name:HX_NOTIFICATION_REGISTER object:nil];
    
    //监听登录通知
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loginNotification:) name:HX_NOTIFICATION_LOGIN object:nil];
    
    //视图关闭通知
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(closeViewNotification:) name:HX_NOTIFICATION_CLOSEVIEW object:nil];
    
    //注销通知
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(logoutNotification:) name:HX_NOTIFICATION_LOGOUT object:nil];
    
    //支付页面关闭通知
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(closePayViewNotification:) name:HX_NOTIFICATION_CLOSE_PAYVIEW object:nil];
}


- (BOOL)isInitIT{
    return _isInit;
}

- (BOOL)isLogined{
    return _isLogin;
}

- (void)loginIT:(AppITLoginFinishBlock)finishBlock{
    [HXAppPlatformKitPro showLoginView];
  self.loginFinish = finishBlock;
}

- (void)switchAccount{

}

- (NSString *)nickName{
    return _nickName;
}
- (NSString *)loginUin{
    return _userID;
}
- (NSString *)sessionId{
    return _sessionID;
}

- (void)enterIT{
    [HXAppPlatformKitPro showPlatformView];
}

- (void)logout:(BOOL)iscallback{
    _needLogoutCallback = iscallback;
    [HXAppPlatformKitPro logout];
}

- (void)buyGood:(NSString *)goodid
          price:(NSInteger)price
           name:(NSString *)productName
            des:(NSString *)describe
            ext:(NSString *)extraData
    finishBlock:(AppITBuyGoodsFinishBlock)buyFinishBlock{
    
    self.buyFinishBlock = buyFinishBlock;

    NSDictionary * dic = [extraData objectFromJSONString];
    NSString * accountName = [dic valueForKey:@"accountName"];
    NSString * accountID = [dic valueForKey:@"accountID"];
    NSString * token = [dic valueForKey:@"token"];
    NSString * serverID = [dic valueForKey:@"serverID"];
    
    int time = [[NSDate date] timeIntervalSince1970];
    
    NSString*billNO = [NSString stringWithFormat:@"%@-%@-%d-%@-%@",@"IT",accountID,time,goodid,serverID];
    _currentSeriaNo = [[NSString alloc] initWithString: billNO];
    
    [HXAppPlatformKitPro setPayViewAmount:[[[NSNumber alloc] initWithInteger:price] floatValue]
//      [HXAppPlatformKitPro setPayViewAmount:0.1
                               orderIdCom:billNO];

}


#pragma mark - 处理 SDK 通知
//注册通知处理
- (void)registerNotification:(NSNotification *)notification
{
    NSLog(@"userId: %@", [notification.object objectForKey:@"userId"]);
    NSLog(@"userName: %@", [notification.object objectForKey:@"userName"]);
    NSLog(@"sessionId: %@", [notification.object objectForKey:@"sessionId"]);
    _nickName = [notification.object objectForKey:@"userName"];
    _sessionID = [notification.object objectForKey:@"sessionId"];
    _userID = [notification.object objectForKey:@"userId"];
    _isLogin = YES;
    if (self.loginFinish) {
        self.loginFinish(YES);
    }
  }

//登录通知处理
- (void)loginNotification:(NSNotification *)notification
{
    NSLog(@"userId: %@", [notification.object objectForKey:@"userId"]);
    NSLog(@"userName: %@", [notification.object objectForKey:@"userName"]);
    NSLog(@"sessionId: %@", [notification.object objectForKey:@"sessionId"]);
    _nickName = [notification.object objectForKey:@"userName"];
    _sessionID = [notification.object objectForKey:@"sessionId"];
    _userID = [notification.object objectForKey:@"userId"];
    _isLogin = YES;
    if (self.loginFinish) {
        self.loginFinish(YES);
    }
}

//关闭窗口通知处理
- (void)closeViewNotification:(NSNotification *)notification
{
    NSLog(@"SDK View Closed");
}

//注销通知处理
- (void)logoutNotification:(NSNotification *)notification
{
    if (_needLogoutCallback){
        CSDKManager::getInstance()->logoutCallBack();
    }
    _isLogin = NO;
    _userID = nil;
    _sessionID = nil;
    _nickName = nil;

}

//支付视图关闭消息处理
- (void)closePayViewNotification:(NSNotification *)notification
{
    NSLog(@"PAY View Closed");
    if (self.buyFinishBlock) {
        self.buyFinishBlock(YES, _currentSeriaNo);
    }
}


#pragma mark - class init and singleton method

+ (AppITManager *)sharedInstance {
  
  if (__instance == nil) {
    __instance = [[AppITManager alloc] init];
  }
  return __instance;
}

+ (id)alloc {
  
  NSAssert(__instance == nil, @"Attempted to allocate a second instance of a singleton.");
  return [super alloc];
}

- (id)init {
  
  self = [super init];
  if (self) {
  _isInit = NO;
      [self configureITSDKWhenBootup];
  }
  return self;
}

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
  NSAssert(NO, @"Attempted to deallocate a singleton instance.");
    [super dealloc];
}


@end
