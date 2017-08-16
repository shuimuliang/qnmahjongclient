//
//  AppHMManager.m
//  zhajinhua
//
//  Created by LiuLihua on 13-3-27.
//
//

#import "AppHMManager.h"
#import "SDKManager.h"
#include <stdio.h>
#import "NSString+MD5Addition.h"
#import "Reachability.h"
#import "PSAlertView.h"
#import "JSONKit.h"
#import "AppUtils.h"
#if CASINO_PAYPAL
#import "PayPalManager.h"
#endif
//SDKManager interface
void CSDKManager::initPlatform(int code)
{
    m_ChannelID = CASINO_CHANNEL_ID;
    [AppHMManager sharedInstance];
}

AppThirdLoginType CSDKManager::getLoginType()
{
    return CASINO_LOGIN_TYPE;
}

void CSDKManager::login()
{
    CCLog("海马登录");
    [[AppHMManager sharedInstance] loginHM:^(BOOL succ){
        this->loginCallBack(succ);
    }];
}
void CSDKManager::logout(bool iscallback)
{
    [[AppHMManager sharedInstance] logout];
}
void CSDKManager::userCenter()
{
    
}
bool CSDKManager::isLogined(){
    return [[AppHMManager sharedInstance] isLogined];
}
bool CSDKManager::isInited()
{
    return true;
}
void CSDKManager::unInit(){
    
}
const std::string CSDKManager::getAccountName(){
    return [[[AppHMManager sharedInstance] getNickName] UTF8String];
}
const std::string CSDKManager::getAccountID(){
    return [[[AppHMManager sharedInstance] getUserID] UTF8String];
}
const std::string CSDKManager::getSessionID(){
    return [[[AppHMManager sharedInstance] getSessionID] UTF8String];
}

void CSDKManager::preLoadGoods(const char* goodsList){
}

void CSDKManager::showWebPayView(std::string url,
                                 std::string jsonData){
    
}
void CSDKManager::thirdLogin(int loginType){ // 第三方登陆
  
}

/*
 unsigned int ID,
 const char* name,
 unsigned int money,
 const char* description,
 
 */

void CSDKManager::pay(const char* parmas){
    if (![[Reachability reachabilityForInternetConnection] isReachable]){
        PSAlertView *alert = [PSAlertView alertWithTitle:APP_LOCALE(@"common.tip.title") message:APP_LOCALE(@"common.error.network")];
        [alert setCancelButtonWithTitle:APP_LOCALE(@"common.ok") block:^(void){
            // do nothing
        }];
        [alert show];
        return;
    }
    
    NSString * goodData =  parmas?[NSString stringWithUTF8String : parmas]:nil;
    NSDictionary *dataDic = [goodData objectFromJSONString];
    ZHPayOrderInfo *orderInfo = [[ZHPayOrderInfo alloc] init];

    int time = [[NSDate date] timeIntervalSince1970];
    int channel = this->getChannelID();
    NSString *productID = [dataDic objectForKey:@"appStoreProductId"];
    NSString*billNO = [NSString stringWithFormat:@"%@-%@-%d-%@-%d",@"HM",[dataDic objectForKey:@"userID"],time,productID,channel];
    orderInfo.orderId = billNO;
    orderInfo.productName = [dataDic objectForKey:@"name"];
    orderInfo.gameName = [dataDic objectForKey:@"gameName"];
    orderInfo.productPrice = [[dataDic objectForKey:@"price"] doubleValue];
#if DEBUG
    orderInfo.productPrice = 0.01;
#endif
    orderInfo.userParams = [NSString stringWithFormat:@"%@-%d-%@",[dataDic objectForKey:@"userID"],channel,productID];
    [[AppHMManager sharedInstance] pay:^(BOOL succeed, NSString * orderInfo){
        this->buyCallBack(succeed, [orderInfo UTF8String]);
    } order:(ZHPayOrderInfo *)orderInfo];
}
void CSDKManager::payWithPayPal(const char* parmas){
    
}

const std::string  CSDKManager::getMissOrderList(){
   return "";
}
const std::string CSDKManager::getOrderInfoAndRemove(const char* orderID){
    return "";
}

/********* 我是华丽的分割线 *********/
static AppHMManager * __instance = nil;

@interface AppHMManager()<ZHPayResultDelegate>
@property (nonatomic, copy) AppHMLoginFinishBlock loginFinishBlock;
@property (nonatomic, copy) AppHMBuyGoodsFinishBlock buyFinishBlock;
@end

@implementation AppHMManager {
    NSString * _sessionID;
    NSString * _nickName;
    NSString * _userID;
}

+ (AppHMManager *) sharedInstance {
    if (__instance == nil) {
        __instance = [[AppHMManager alloc] init];
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
        [self configureHMSDKWhenBootup];
    }
    return self;
}

- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    NSAssert(NO, @"Attempted to deallocate a singleton instance.");
    [super dealloc];
}

- (void)configureHMSDKWhenBootup {
    [ZHPayPlatform initWithAppId:CASINO_HM_APPID withDelegate:self testUpdateMode:NO alertTypeCheckFailed:0];
    [ZHPayPlatform setSupportOrientation:UIInterfaceOrientationMaskLandscape];
    
#if DEBUG
    [ZHPayPlatform setLogEnable:YES];
#endif
}

- (void)loginHM: (AppHMLoginFinishBlock)finishBlock {
    NSLog(@"海马登录sdk调用");
    self.loginFinishBlock = finishBlock;
    [ZHPayPlatform startLogin];
}

- (void)logout {
    NSLog(@"海马登出");
    [ZHPayPlatform startLogout];
}

- (void)ZHPayLoginSuccess:(ZHPayUserInfo *)userInfo {
    NSLog(@"海马账户登录成功");
    _sessionID = userInfo.validateToken;
    _nickName = userInfo.userName;
    _userID = userInfo.userId;
    self.loginFinishBlock(true);
}

- (void)ZHPayLoginCancel {
    NSLog(@"海马账户登录取消");
    self.loginFinishBlock(false);
    self.loginFinishBlock = nil;
}

- (void)ZHPayDidLogout {
    NSLog(@"海马账户注销成功，此时需要将游戏退出，并展示登录前界面");
    _sessionID = nil;
    _nickName = nil;
    _userID = nil;
//    self.loginFinishBlock = nil;
    CSDKManager::getInstance()->logoutCallBack();
}

- (void)ZHPayViewIn {
    NSLog(@"SDK视图出现");
}

- (void)ZHPayViewOut {
    NSLog(@"SDK视图消失");
}

- (NSString *)getNickName{
    return _nickName;
}
- (NSString *)getUserID{
    return _userID;
}
- (NSString *)getSessionID{
    return _sessionID;
}

- (BOOL)isLogined {
    return [ZHPayPlatform isLogined];
}

- (void)pay: (AppHMBuyGoodsFinishBlock)buyCallback  order: (ZHPayOrderInfo*)orderInfo {
    self.buyFinishBlock = buyCallback;
    [ZHPayPlatform startPay:orderInfo delegate:self];
}

- (void)ZHPayResultSuccessWithOrder:(ZHPayOrderInfo *)orderInfo {
    NSLog(@"订单⽀支付成功");
    NSMutableDictionary *dic = [NSMutableDictionary dictionaryWithCapacity:1];
    [dic setObject:orderInfo.orderId forKey:@"transaction_id"];
//    [dic setObject: [orderInfo.transactionReceipt base64EncodedString]  forKey:@"receipt-data"];
    NSString *buyInfo = [dic JSONString];
    if (self.buyFinishBlock) {
        self.buyFinishBlock(true, buyInfo);
    }
}

- (void)ZHPayResultFailedWithOrder:(ZHPayOrderInfo *)orderInfo resultCode:(ZH_PAY_ERROR)errorType {
    NSLog(@"订单⽀付失败");
    if (self.buyFinishBlock) {
        self.buyFinishBlock(false, @"");
    }
}

- (void)ZHPayResultCancelWithOrder:(ZHPayOrderInfo *)orderInfo {
    NSLog(@"订单取消");
    if (self.buyFinishBlock) {
        self.buyFinishBlock(false, @"");
    }
}
- (BOOL)shouldAutorotate{
    return [ZHPayPlatform shouldAutorotate];
}


- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window {
    return [ZHPayPlatform application:application
supportedInterfaceOrientationsForWindow:window];
}
@end
