//
//  AppXYManager.h
//
//
//
//

#import "AppXYManager.h"
#import "SDKManager.h"
#include <stdio.h>
#import "JSONKit.h"
#import "Reachability.h"
#import "PSAlertView.h"
#import "AppUtils.h"

void CSDKManager::initPlatform(int code)
{
     m_ChannelID = CASINO_CHANNEL_ID;
    [AppXYManager sharedInstance];
}

void CSDKManager::preLoadGoods(const char* goodsList){
}

AppThirdLoginType CSDKManager::getLoginType()
{
    return CASINO_LOGIN_TYPE;
}

void CSDKManager::login()
{
    [[AppXYManager sharedInstance] loginXY:^(BOOL succ){
        this->loginCallBack(succ);
    }];
}
void CSDKManager::logout(bool iscallback)
{
    [[AppXYManager sharedInstance] logout:YES];
}
void CSDKManager::userCenter()
{
    [[AppXYManager sharedInstance] enterXY];
}
bool CSDKManager::isLogined()
{
    return [[AppXYManager sharedInstance] isLogined];
}
bool CSDKManager::isInited()
{
    return [[AppXYManager sharedInstance] isInitXY];
}

const std::string CSDKManager::getAccountName(){
    return [[[AppXYManager sharedInstance] getNickName] UTF8String];
}
const std::string CSDKManager::getAccountID(){
    return [[[AppXYManager sharedInstance] getUserID] UTF8String];
}
const std::string CSDKManager::getSessionID(){
    return [[[AppXYManager sharedInstance] getSessionID] UTF8String];
}
void CSDKManager::unInit(){
    
}

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
    [[AppXYManager sharedInstance] buyGood:dataDic finishBlock:^(BOOL succeed, NSString *orderInfo) {
        this->buyCallBack(succeed, [orderInfo UTF8String]);
    }]; 
}

void CSDKManager::payWithPayPal(const char* parmas){
    
}

const std::string  CSDKManager::getMissOrderList(){
    return "";
}
const std::string CSDKManager::getOrderInfoAndRemove(const char* orderID){
    return "";
}

void CSDKManager::showWebPayView(std::string url,
                                 std::string jsonData){
    
}
void CSDKManager::thirdLogin(int loginType){ // 第三方登陆
    
}
static AppXYManager * __instance = nil;


@interface AppXYManager ()
@property (nonatomic, copy) AppXYLoginFinishBlock loginFinish;
@property (nonatomic, copy) AppXYBuyGoodsFinishBlock buyFinishBlock;
@end

@implementation AppXYManager{
    BOOL _isInit;
    BOOL _isLogined;
//    NSString *_currentSeriaNo;
//    BOOL _needLogoutCallback;
}

- (void)configureXYSDKWhenBootup{
    [self addXYObserver];
    // ******* 1 ******
    /*!SDK初始化, 务必放在SDK其他接口调用之前
     * isContinueWhenCheckUpdateFailed 传YES, 因网络错误获取更新信息失败情况下，强制更新可能失败。 传NO则必须强制更新之后才会开始游戏
     * 注意： xy sdk1.3.0 版本， 初始化要在游戏window创建之前
     */
    [[XYPlatform defaultPlatform] initializeWithAppId:CASINO_XY_APPID appKey:CASINO_XY_APPKEY isContinueWhenCheckUpdateFailed:YES];
    
    NSString *bID = [[NSBundle mainBundle]bundleIdentifier];
//    [XYPlatform defaultPlatform].appScheme = @"com.jingling.zjh.xy.alipay";
    [XYPlatform defaultPlatform].appScheme = [bID stringByAppendingString:@".alipay"];
    
    
    /*! 设置 平台页面 屏幕方向
     * 1、其中设置的方向需要在 app plist文件Supported interface orientations 中支持，否则会Assert
     * 2、UIInterfaceOrientation, 设置 UIInterfaceOrientationLandscapeLeft 或者 UIInterfaceOrientationLandscapeRight，平台页面仅支持横屏幕。
     * 3、设置 UIInterfaceOrientationPortrait ，平台仅支持Portrait方向
     * 4、若不设置，平台页面会根据设备方向以及plist中配置适配方向做自适应，
     * 5、可不设置
     */
    //    [[XYPlatform defaultPlatform] XYSetScreenOrientation:UIInterfaceOrientationLandscapeLeft];
    

    
#if DEBUG
    //打印log到控制台， 设置方便查看日志，可不设置
    [[XYPlatform defaultPlatform] XYSetShowSDKLog:YES];
    [[XYPlatform defaultPlatform] XYSetDebugModel:NO];
    NSLog(@"It's debug model.");
#else
#warning +++++++++debug+++++++
    // 默认为正式环境, 即为NO, 测试环境为 YES
    [[XYPlatform defaultPlatform] XYSetDebugModel:NO];
#endif
    
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
}



- (void)addXYObserver{
    //添加XYPlatform 各类通知的观察者
    
    /*初始化结束通知, 登录等操作务必在收到该通知后调用*/
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(xyplatformInitFinished:)
                                                 name:kXYPlatformInitDidFinishedNotification
                                               object:nil];
    
    /*登录通知*/
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(xyplatformLoginNoti:)
                                                 name:kXYPlatformLoginNotification
                                               object:nil];
    
    /* 注销登录通知 */
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(xyplatformLogoutFinished:)
                                                 name:kXYPlatformLogoutNotification
                                               object:nil];
    
    /*离开平台通知*/
//    [[NSNotificationCenter defaultCenter] addObserver:self
//                                             selector:@selector(xyplatformLeavedPlatform:)
//                                                 name:kXYPlatformLeavedNotification
//                                               object:nil];
//    
//    [[NSNotificationCenter defaultCenter] addObserver:self
//                                             selector:@selector(xyPlatformGuestTurnOffical:)
//                                                 name:kXYPlatformGuestTurnOfficialNotification
//                                               object:nil];
}


- (BOOL)isInitXY{
    return _isInit;
}

- (BOOL)isLogined{
 //   return   - (void)XYIsLogined:(void (^)(BOOL isLogined)) bLogined;
    return _isLogined;
}

- (void)loginXY:(AppXYLoginFinishBlock)finishBlock{
//    [[XYPlatform defaultPlatform] XYUserLogin:0];
    self.loginFinish = finishBlock;
    [[XYPlatform defaultPlatform] XYAutoLogin:0];
}

- (void)switchAccount{
    [[XYPlatform defaultPlatform] XYUserLogin:0];
  //  self.loginFinish = finishBlock;

}

- (NSString *)getNickName{
    NSString *str = [[XYPlatform defaultPlatform] XYLoginUserAccount];
//    NSLog(@"_nickName = %@", _nickName);
    str = str ?str : @"";
    return str;
}
- (NSString *)getUserID{
    NSString *userID = [[XYPlatform defaultPlatform] XYOpenUID];   // uid
    userID = userID?userID : @"";
    return userID;
}
- (NSString *)getSessionID{
    NSString *sessionID =[[XYPlatform defaultPlatform] XYToken];       // token
    sessionID = sessionID ? sessionID : @"";
    return sessionID;
}

- (void)enterXY{
    [[XYPlatform defaultPlatform] XYEnterUserCenter:0];
}

- (void)logout:(BOOL)iscallback{
    [[XYPlatform defaultPlatform] XYLogout:0];
}

- (void)buyGood:(NSDictionary *)dic
    finishBlock:(AppXYBuyGoodsFinishBlock)buyFinishBlock{
    
    self.buyFinishBlock = buyFinishBlock;

    NSString * accountName = [dic valueForKey:@"accountName"];
    NSString * accountID = [dic valueForKey:@"accountID"];
    NSString * token = [dic valueForKey:@"token"];
    NSString * serverID = @"1";//[dic valueForKey:@"serverID"];//分服时候可用
    
#if DEBUG
    NSString * nsPrice = @"1";
#else
    NSString * nsPrice = [dic valueForKey:@"price"];
#endif
    
    int time = [[NSDate date] timeIntervalSince1970];
    int channel = CSDKManager::getInstance()->getChannelID();
    NSString *productID = [dic objectForKey:@"appStoreProductId"];
    NSString*billNO = [NSString stringWithFormat:@"%@-%@-%d-%@-%d",@"XY",[dic objectForKey:@"userID"],time,productID,channel];
    NSString * extra = [NSString stringWithFormat:@"%@-%d-%@",[dic objectForKey:@"userID"],channel,productID];
    [[XYPlatform defaultPlatform] XYPayWithAmount:nsPrice
                                  appServerId:serverID
                                  appExtra:extra
                                  delegate:self];
}


#pragma mark - 消息回调处理
/**
 *  SDK初始化结束通知（登录等操作务必放到初始化完成的通知里！！！！）
 */
- (void)xyplatformInitFinished:(NSNotification*)notification{
//    [[XYPlatform defaultPlatform] XYAutoLogin:0];
    _isInit = YES;
    [[XYPlatform defaultPlatform] XYShowToolBar:XYToolBarAtMiddleLeft isUseOldPlace:YES];
}


/**
 *  登录结束通知
 */
- (void)xyplatformLoginNoti:(NSNotification*)notification
{
    NSLog(@"登录成功回调");
     // 登录完成, 提供token 以及 openuid 给游戏校验
     NSDictionary *userInfo = notification.userInfo;
     if ([userInfo[kXYPlatformErrorKey] intValue] == XY_PLATFORM_NO_ERROR) {
         //登录成功
         [[XYPlatform defaultPlatform] XYShowToolBar:XYToolBarAtTopLeft isUseOldPlace:YES];
         _isLogined = YES;
         if (self.loginFinish) {
             self.loginFinish(true);
         }else{
             CSDKManager::getInstance()->loginCallBack(true);
         }
     }
     else{
         if (self.loginFinish) {
             self.loginFinish(false);
         }else{
             CSDKManager::getInstance()->loginCallBack(false);
         }
     }
}

/**
 *  注销通知
 */
- (void)xyplatformLogoutFinished:(NSNotification*)notification{
    [[XYPlatform defaultPlatform] XYHideToolBar];
    if (_isLogined){
        CSDKManager::getInstance()->logoutCallBack();
    }
    _isLogined = NO;
    NSLog(@"注销通知");
}

- (void)payCallback:(BOOL)result withInfo:(NSString *)orderID{
    NSMutableDictionary *dic = [NSMutableDictionary dictionaryWithCapacity:1];
    [dic setObject:orderID forKey:@"transaction_id"];
    NSString *buyInfo = [dic JSONString];
    self.buyFinishBlock(result, buyInfo);
}

- (void)XYHandleOpenURL:(NSURL *)url{
    [[XYPlatform defaultPlatform] XYHandleOpenURL:url];
}

#pragma mark-- 充值回调 XYPayDelegate

- (void) XYPayFailedWithOrder:(NSString *)orderId
{
    if (self.buyFinishBlock) {
        [self payCallback:NO withInfo:@""];
    }
    //  支付失败
    NSLog(@"支付失败  orderID＝ %@", orderId);
}

- (void) XYPaySuccessWithOrder:(NSString *)orderId
{
    if (self.buyFinishBlock) {
        [self payCallback:YES withInfo:orderId];
    }
    // 支付成功
    NSLog(@"支付成功  orderID＝ %@", orderId);
   // [self doSomeThingAfterPayActionWithOrder:orderId];
}

- (void) XYPayDidCancelByUser:(NSString *)orderId
{
    // 用户取消操作，若orderId为空则是用户取消充值， 若orderId不为空则不确定支付成功或失败
    
    NSLog(@"用户取消支付 orderID＝ %@", orderId);
    if ([orderId length] == 0) {
        if (self.buyFinishBlock) {
            [self payCallback:NO withInfo:@""];
        }
        // here 用户取消充值
    }else{
        if (self.buyFinishBlock) {
            [self payCallback:YES withInfo:orderId];
        }
    }
}

#pragma mark - class init and singleton method

+ (AppXYManager *)sharedInstance {
  
  if (__instance == nil) {
    __instance = [[AppXYManager alloc] init];
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
    [self configureXYSDKWhenBootup];
  }
  return self;
}

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
  NSAssert(NO, @"Attempted to deallocate a singleton instance.");
    [super dealloc];
}


@end
