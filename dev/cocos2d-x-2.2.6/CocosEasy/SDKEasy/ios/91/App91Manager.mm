//
//  App91Manager.m
//  client
//
//  Created by liulihua on 14-4-8.
//
//

#import "App91Manager.h"
#import "SDKManager.h"
#include <stdio.h>
#import "PSAlertView.h"
#import "JSONKit.h"
#import "Analysis.h"
using namespace std;

//SDKManager interface
void CSDKManager::initPlatform(int code)
{
#warning 发布前必须修改
#if DEBUG
    m_ChannelID = SDK_CHANNEL_91;
#else
    m_ChannelID = SDK_CHANNEL_DEBUG;
#endif
    [App91Manager sharedInstance];
}

void CSDKManager::preLoadGoods(const char* goodsList){
}

void CSDKManager::login()
{
    [[App91Manager sharedInstance] login91:^(BOOL succ,NSError *err){
        this->loginCallBack(succ);
    }];
}
void CSDKManager::logout(bool iscallback)
{
    [[App91Manager sharedInstance] logout:iscallback];
}
void CSDKManager::userCenter()
{
    [[App91Manager sharedInstance] enter91];
}
bool CSDKManager::isLogined()
{
    return [[App91Manager sharedInstance] isLogined];
}
bool CSDKManager::isInited()
{
    return [[App91Manager sharedInstance] isInit91];
}

const std::string CSDKManager::getAccountName(){
    return [[[App91Manager sharedInstance] nickName] UTF8String];
}
const std::string CSDKManager::getAccountID(){
    return [[[App91Manager sharedInstance] loginUin] UTF8String];
}
const std::string CSDKManager::getSessionID(){
    return [[[App91Manager sharedInstance] sessionId] UTF8String];
}

void CSDKManager::pay(unsigned int ID,
                      const char* name,
                      unsigned int money,
                      const char* description,
                      const char* extraData){

    NSString * nsname =  name?[NSString stringWithUTF8String : name]:nil;
    NSString * nsdescription = description?[NSString stringWithUTF8String:description]:nil;
    NSString * nsextraData = extraData?[NSString stringWithUTF8String:extraData]:nil;
    
    [[App91Manager sharedInstance] buyGood:[NSString stringWithFormat:@"%d",ID]
                                     price:money
                                      name:nsname
                                       des:nsdescription
                                       ext:nsextraData
                               finishBlock:^(BOOL succ ,NdBuyInfo *buyInfo){
        this->buyCallBack(succ,[buyInfo.cooOrderSerial UTF8String]);
    }];
}

void CSDKManager::unInit(){
    
}

#if DEBUG
#define APP_DEBUG_91_PURCHASE 1
#endif
//*******************  分割线  ******************************************//

NSString * const kApp91ErrorDomain = @"App91ErrorDomain";
static App91Manager * __instance = nil;

@interface App91Manager ()
@property (nonatomic, copy) App91LoginFinishBlock loginFinishBlock;
@property (nonatomic, copy) App91BuyGoodsFinishBlock buyFinishBlock;
@end


@implementation App91Manager {
    BOOL _isInit;
    NSString * _login91Uin;
}

- (void)configure91SDKWhenBootup {
    NdInitConfigure *cfg = [[NdInitConfigure alloc] init];
    cfg.versionCheckLevel = ND_VERSION_CHECK_LEVEL_STRICT;
    
    cfg.appid = 114290;
    cfg.appKey = @"aa0354d79876d240829b7329d8c1fdce2a32b69d85bc1e80";
    [[NdComPlatform defaultPlatform] NdInit:cfg];
    [[NdComPlatform defaultPlatform] NdSetScreenOrientation:UIDeviceOrientationPortrait];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(SNSInitResult:) name:(NSString *)kNdCPInitDidFinishNotification object:nil];
    _isInit = NO;
}

- (BOOL)isInit91{
    return _isInit;
}
- (void)SNSInitResult:(NSNotification *)notify {
    //检查软件的版本更新
    //  [[NdComPlatform defaultPlatform] NdAppVersionUpdate:0 delegate:self];
    [[NdComPlatform defaultPlatform] NdShowToolBar:NdToolBarAtTopLeft];
    [[NdComPlatform defaultPlatform] NdSetAutoRotation:YES];
//    [[NdComPlatform defaultPlatform] NdHideToolBar];
    [self registerLoginNotification];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(SNSSessionInvalid:)
                                                 name:(NSString *)kNdCPSessionInvalidNotification
                                               object:nil];
    _isInit = YES;
#if APP_DEBUG_91_PURCHASE
    [[NdComPlatform defaultPlatform] NdSetDebugMode:1];
#endif
    
}
- (void)SNSSessionInvalid:(NSNotification *)notify {
    [self logoutAndBackToLoginView];
}


/*91
 * 登录和充值
 *
 *
 */

- (BOOL)isLogined {
    
    return [[NdComPlatform defaultPlatform] isLogined];
}

- (void)login91:(App91LoginFinishBlock)finishBlock {
    UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
    [[NdComPlatform defaultPlatform] NdSetScreenOrientation:orientation];
    [[NdComPlatform defaultPlatform] NdLogin:0];
    
    self.loginFinishBlock = finishBlock;
}

- (void)show91ToolBar{
    [[NdComPlatform defaultPlatform] NdShowToolBar:NdToolBarAtMiddleRight];
}

- (void)enter91 {
    [[NdComPlatform defaultPlatform] NdEnterPlatform:NDCP_SETTING_DEFAULT];
}
- (void)logout:(BOOL)iscallback{
    [[NdComPlatform defaultPlatform] NdLogout:1];
    if (_login91Uin != nil) {
        if (iscallback){
            CSDKManager::getInstance()->logoutCallBack();
        }
    }
    _login91Uin = nil;
}

- (void)logoutAndBackToLoginView {
    [self logout];
}
- (NSString *)nickName{
    return [NdComPlatform defaultPlatform].nickName;
}
- (NSString *)loginUin{
    return [NdComPlatform defaultPlatform].loginUin;
}
- (NSString *)sessionId{
    return [NdComPlatform defaultPlatform].sessionId;
}

- (void)switchUser {
    _login91Uin = [NdComPlatform defaultPlatform].loginUin;
}

- (void)show91Profile {
    
    [[NdComPlatform defaultPlatform] NdEnterUserSetting:NDCP_SETTING_CONSUME_RECORD];
}

- (NSString *)encryptedUUIDFor91 {
    
    NSString * uin = [NdComPlatform defaultPlatform].loginUin;
    return uin;
}
- (void)showPause{
    [[NdComPlatform defaultPlatform] NdPause];
}
- (void)userFeedback{
    [[NdComPlatform defaultPlatform] NdUserFeedBack];
}
- (void)enterAppBBS{
    [[NdComPlatform defaultPlatform] NdEnterAppBBS:0];
}

- (void)checkMissingOrder {
    //  NdPagination * aPage = [[NdPagination alloc] init];
    //  aPage.pageSize = 10;
    //  [[NdComPlatform defaultPlatform] NdGetUserProductsList:aPage delegate:self];
}


//实现响应方法
- (void)SNSLoginProcess:(NSNotification *)notify {
    
    NSDictionary *dict = [notify userInfo];
    BOOL success = [[dict objectForKey:@"result"] boolValue];
    int error = [[dict objectForKey:@"error"] intValue];
    if (!success){
        [[App91Manager sharedInstance] logSdkEvent:kAppEventSDKLoginFailed errCode:error];
    }
    
    NSError *err = nil;
    BOOL loginSucceed = NO;
    //  if (error == ND_COM_PLATFORM_ERROR_USER_CANCEL){ //用户取消登录和和相应处理
    //  }
    
    if([[NdComPlatform defaultPlatform] isLogined] && success){
        NSLog(@"   user info : %@",[[NdComPlatform defaultPlatform] nickName]);

        loginSucceed = YES;
    } else if (error != ND_COM_PLATFORM_ERROR_USER_CANCEL) { //用户登录失败提示和处理
        err = [NSError errorWithDomain:kApp91ErrorDomain
                                  code:error
                              userInfo:@{ NSLocalizedDescriptionKey: @"登录失败"}];
    }
    
    if (self.loginFinishBlock) {
        self.loginFinishBlock(loginSucceed, err);
        if(loginSucceed) {
           // [self show91ToolBar];
        }
    }
}

- (void)registerLoginNotification {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(SNSLoginProcess:)
                                                 name:(NSString *)kNdCPLoginNotification
                                               object:nil];
}
//在必要的时候(例如析构)移除登录消息的监听
-(void)removeLoginNotification {
    [[NSNotificationCenter defaultCenter] removeObserver:self name:(NSString *)kNdCPLoginNotification object:nil];
}



#pragma mark - buy goods

- (void)buyGood:(NSString *)goodid
          price:(NSInteger)price
          name:(NSString *)productName
          des:(NSString *)describe
          ext:(NSString *)extraData
    finishBlock:(App91BuyGoodsFinishBlock)buyFinishBlock{
    
    NSDictionary * dic = [extraData objectFromJSONString];
    NSString * accountName = [dic valueForKey:@"accountName"];
    NSString * accountID = [dic valueForKey:@"accountID"];
    NSString * token = [dic valueForKey:@"token"];
    NSString * serverID = [dic valueForKey:@"serverID"];
    NSLog(@"\nserverID: %@",serverID);
     self.buyFinishBlock = buyFinishBlock;
     NdBuyInfo * buyInfo = [[NdBuyInfo new] autorelease];
     buyInfo.productCount = 1;
     buyInfo.productId = goodid;
     buyInfo.productName = productName;
     buyInfo.productPrice = price;
     buyInfo.productOrignalPrice = price;
     buyInfo.payDescription = serverID;
    
     NSString * orderSerial = [[NSUUID UUID] UUIDString];
     NSLog(@"\nUUID: %@",orderSerial);

     buyInfo.cooOrderSerial = orderSerial;
     int payRv = [[NdComPlatform defaultPlatform] NdUniPayAsyn:buyInfo];
     if (payRv < 0) {
         PSAlertView *alert = [PSAlertView alertWithTitle:@"购买失败" message:[NSString stringWithFormat:@"error code :%d", payRv]];
         [alert show];
         self.buyFinishBlock(NO, nil);
     }
     return;
}


- (void)addBuyResultObserver {
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(NdBuyCommodityResult:)
                                                 name:kNdCPBuyResultNotification object:nil];
    
    // 异步的时候的回调通知，现在应该是无用的
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(NdBuyCommodityAsyncResult:)
                                                 name:kNdCPAsynPaySMSSentNotification
                                               object:nil];
}

- (void)removeBuyResultObserver {
    
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kNdCPBuyResultNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kNdCPAsynPaySMSSentNotification object:nil];
    
}

- (void)NdBuyCommodityAsyncResult:(NSNotification*)notify {
    
    [self NdBuyCommodityResult:notify];
}

- (void)NdBuyCommodityResult:(NSNotification*)notify {
    
     NSDictionary* dic = [notify userInfo];
     BOOL bSuccess = [[dic objectForKey:@"result"] boolValue];
     NdBuyInfo * buyInfo = (NdBuyInfo*)[dic objectForKey:@"buyInfo"];
     NSString * serialNo = buyInfo.cooOrderSerial;
     
     int nErrorCode = [[dic objectForKey:@"error"] intValue];
     if (nErrorCode < 0) {
         NSLog(@"buy goods error code: %d", nErrorCode);
     }
    if (!bSuccess){
        [[App91Manager sharedInstance] logSdkEvent:kAppEventSDKPayFailed errCode:nErrorCode];
    }
    
     switch (nErrorCode) {
         case ND_COM_PLATFORM_ERROR_NETWORK_FAIL:
         case ND_COM_PLATFORM_ERROR_VG_PRODUCT_ID_INVALID:
         case ND_COM_PLATFORM_ERROR_VG_PRODUCT_USE_SIGN_INVALID:
         case ND_COM_PLATFORM_ERROR_USER_CANCEL:
             [App91Manager messageBox:[NSString stringWithFormat:@"error code :%d",nErrorCode]];
             break;
             // 下面的情况都视为成功，特别注意：91的返回值在成功的时候也可能有error code
         case ND_COM_PLATFORM_ERROR_ORDER_SERIAL_SUBMITTED:
         case ND_COM_PLATFORM_ERROR_ORDER_SERIAL_DUPLICATE:
         case ND_COM_PLATFORM_NO_ERROR:
         default:
             break;
     }
     
     if (self.buyFinishBlock) {
         self.buyFinishBlock(bSuccess, buyInfo);
     } else if (serialNo) {
         NSLog(@"no buyFinishBlock %@",serialNo);
     }
    
}

- (void)NdGetUserProductsListDidFinish:(int)error result:(NdBasePageList*)result {
    
    // nothing
}
- (void)userLogin91 {
    
    if ([NdComPlatform defaultPlatform].isLogined == NO) {
        [self logoutAndBackToLoginView];
        return;
    }
    if (_login91Uin && [_login91Uin isEqualToString:[NdComPlatform defaultPlatform].loginUin] == NO) {
        [self switchUser];
    } else {
        _login91Uin = [NdComPlatform defaultPlatform].loginUin;
    }
}

#pragma mark - util

+ (void)messageBox:(NSString*)stringTip
{
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:stringTip
                                                    message:nil
                                                   delegate:nil
                                          cancelButtonTitle:nil
                                          otherButtonTitles:@"确定", nil];
	[alert show];
}

#pragma mark - class init and singleton method
+ (App91Manager *)sharedInstance {
    
    if (__instance == nil) {
        __instance = [[App91Manager alloc] init];
    }
    return __instance;
}

+(id)alloc {
    
	NSAssert(__instance == nil, @"Attempted to allocate a second instance of a singleton.");
	return [super alloc];
}

- (id)init {
    
    self = [super init];
    if (self) {
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(userLogin91) name:kNdCPLoginNotification object:nil];
        [self addBuyResultObserver];
        _isInit = NO;
        [self configure91SDKWhenBootup];
    }
    return self;
}

- (void)dealloc {
    
    [[NSNotificationCenter defaultCenter] removeObserver:self];
	NSAssert(NO, @"Attempted to deallocate a singleton instance.");
    [super dealloc];
}
@end

