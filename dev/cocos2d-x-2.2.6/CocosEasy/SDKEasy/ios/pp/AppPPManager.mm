//
//  AppPPManager.m
//  zhajinhua
//
//  Created by LiuLihua on 13-3-27.
//
//

#import "AppPPManager.h"
#import "SDKManager.h"
#include <stdio.h>
#import "NSString+MD5Addition.h"
#import "JSONKit.h"


void CSDKManager::initPlatform(int code){
    m_ChannelID = SDK_CHANNEL_PP;
    [AppPPManager sharedInstance];
}
void CSDKManager::preLoadGoods(const char* goodsList){
}

void CSDKManager::login(){
    [[AppPPManager sharedInstance] loginPP:^(BOOL succ){
        this->loginCallBack(succ);
    }];
}
void CSDKManager::logout(bool iscallback){
    [[AppPPManager sharedInstance] logout:iscallback];
}
void CSDKManager::userCenter(){
    [[AppPPManager sharedInstance] enterPP];
}
bool CSDKManager::isLogined(){
    return [[AppPPManager sharedInstance] isLogined];
}
bool CSDKManager::isInited(){
    return [[AppPPManager sharedInstance] isInitPP];
}

const std::string CSDKManager::getAccountName(){
    return [[[AppPPManager sharedInstance] nickName] UTF8String];
}
const std::string CSDKManager::getAccountID(){
    return [[[AppPPManager sharedInstance] loginUin] UTF8String];
}
const std::string CSDKManager::getSessionID(){
    return [[[AppPPManager sharedInstance] sessionId] UTF8String];
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
    
    [[AppPPManager sharedInstance] buyGood:[NSString stringWithFormat:@"%d",ID]
                                     price:money
                                      name:nsname
                                       des:nsdescription
                                       ext:nsextraData
                               finishBlock:^(BOOL succ ,NSString * ord){
                                   this->buyCallBack(succ,[ord UTF8String]);
                               }];
    
}

static AppPPManager * __instance = nil;

@interface AppPPManager ()
@property (nonatomic, copy) AppPPLoginFinishBlock loginFinish;
@property (nonatomic, copy) AppPPBuyGoodsFinishBlock buyFinishBlock;
@end

@implementation AppPPManager{
    BOOL _isInit;
    NSString * _token;
    NSString * _currentSeriaNo;
    BOOL _needLogoutCallback;

}

- (void)configurePPSDKWhenBootup{
  
  [[PPAppPlatformKit sharedInstance] setAppId:3657 AppKey:@"9ac81e5d7d14ba9b4dafc630034d49bb"];
  [[PPAppPlatformKit sharedInstance] setIsNSlogData:NO];
  [[PPAppPlatformKit sharedInstance] setRechargeAmount:10];
  [[PPAppPlatformKit sharedInstance] setIsLogOutPushLoginView:NO];
  [[PPAppPlatformKit sharedInstance] setIsOpenRecharge:YES];
  [[PPAppPlatformKit sharedInstance] setIsLongComet:YES];
  [[PPAppPlatformKit sharedInstance] setCloseRechargeAlertMessage:@"关闭充值提示语"];
  [[PPUIKit sharedInstance] checkGameUpdate];
  [[PPAppPlatformKit sharedInstance] setDelegate:self];
  _isInit = NO;
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(unionPayResult:) name:@"PPCUPPayResultNotification" object: nil];
}

- (void)setAlixPayResult:(NSURL *)url{
    [[PPAppPlatformKit sharedInstance] alixPayResult:url];
}

- (BOOL)isInitPP{
    return _isInit;
}

- (BOOL)isLogined{
    
    BOOL loginStagte = [[PPAppPlatformKit sharedInstance] loginState];
  return [[PPAppPlatformKit sharedInstance] loginState];
}

- (void)loginPP:(AppPPLoginFinishBlock)finishBlock{
  [[PPAppPlatformKit sharedInstance] showLogin];
  self.loginFinish = finishBlock;
}

- (NSString *)nickName{
    return @"";
}
- (NSString *)loginUin{
    return @"";
}
- (NSString *)sessionId{
    return _token;
}

- (void)enterPP{
  [[PPAppPlatformKit sharedInstance] showCenter];
}

- (void)logout:(BOOL)iscallback{
    _needLogoutCallback = iscallback;
    [[PPAppPlatformKit sharedInstance] PPlogout];
}

- (void)buyGood:(NSString *)goodid
          price:(NSInteger)price
           name:(NSString *)productName
            des:(NSString *)describe
            ext:(NSString *)extraData
    finishBlock:(AppPPBuyGoodsFinishBlock)buyFinishBlock{
    
    self.buyFinishBlock = buyFinishBlock;
    
    NSDictionary * dic = [extraData objectFromJSONString];
    NSString * accountName = [dic valueForKey:@"accountName"];
    NSString * accountID = [dic valueForKey:@"accountID"];
    NSString * token = [dic valueForKey:@"token"];
    NSString * serverID = [dic valueForKey:@"serverID"];
    
    int intServerID = (serverID == NULL ? 0 : [serverID integerValue]);
    
    int time = [[NSDate date] timeIntervalSince1970];
    NSString*billNO = [NSString stringWithFormat:@"%@-%@-%d-%@-%@",@"PP",accountID,time,goodid,serverID];
    _currentSeriaNo = [[NSString alloc] initWithString: billNO];
    
    [[PPAppPlatformKit sharedInstance] exchangeGoods:price
                                              BillNo:billNO
                                           BillTitle:productName
                                              RoleId:accountID
                                              ZoneId:intServerID];

}


#pragma mark    ---------------SDK CALLBACK ---------------
-(void)unionPayResult:(NSNotification *)noti
{
    NSLog(@"银联支付结果-------%@",noti.object);
    NSString * str = noti.object ;
    BOOL bl = [str isEqual:@"success"];
    if (bl) {
        if (self.buyFinishBlock) {
            self.buyFinishBlock(YES, _currentSeriaNo);
        }
    }
}
////字符串登录成功回调【实现其中一个就可以】
- (void)ppLoginStrCallBack:(NSString *)paramStrToKenKey
{
    _token = paramStrToKenKey;
    
    if (self.loginFinish) {
        self.loginFinish(YES);
    }
    
    [[PPAppPlatformKit sharedInstance] getUserInfoSecurity];
    
}

//关闭客户端页面回调方法
-(void)ppClosePageViewCallBack:(PPPageCode)paramPPPageCode{
    //可根据关闭的VIEW页面做你需要的业务处理
    NSLog(@"当前关闭的VIEW页面回调是%d", paramPPPageCode);
}



//关闭WEB页面回调方法
- (void)ppCloseWebViewCallBack:(PPWebViewCode)paramPPWebViewCode{
    //可根据关闭的WEB页面做你需要的业务处理
    NSLog(@"当前关闭的WEB页面回调是%d", paramPPWebViewCode);
    if (self.buyFinishBlock) {
        self.buyFinishBlock(YES, _currentSeriaNo);
    }
 
}

//注销回调方法
- (void)ppLogOffCallBack{
    NSLog(@"注销的回调");
    if (_needLogoutCallback){
        CSDKManager::getInstance()->logoutCallBack();
    }
    _token = nil;
}

//兑换回调接口【只有兑换会执行此回调】
- (void)ppPayResultCallBack:(PPPayResultCode)paramPPPayResultCode{
    NSLog(@"兑换回调返回编码%d",paramPPPayResultCode);
    //回调购买成功。其余都是失败

    if(paramPPPayResultCode == PPPayResultCodeSucceed){
        //购买成功发放道具
        if (self.buyFinishBlock) {
            self.buyFinishBlock(YES, _currentSeriaNo);
        }
    }else{
         self.buyFinishBlock(NO, _currentSeriaNo);
        [[AppPPManager sharedInstance] logSdkEvent:kAppEventSDKPayFailed errCode:paramPPPayResultCode];
    }
}

-(void)ppVerifyingUpdatePassCallBack
{
    NSLog(@"验证游戏版本完毕回调");
    _isInit = YES;
}

#pragma mark      ---------------------ios supportedInterfaceOrientations -------------
//iOS 6.0旋屏
- (BOOL)shouldAutorotate
{
  return YES;
}


//iOS 6.0旋屏
- (NSUInteger)supportedInterfaceOrientations
{
  /**iOS6以上旋屏处理。默认SDK与游戏客户端支持方向是同步的，也就是说游戏支持什么方向旋转，SDK则也支持。游戏不支持该方向。SDK也不支持。
   *如果特殊方向不需要SDK旋转，而游戏需要支持。请将PPUserUIKit所不支持的方向设置NO。
   *至少要保持一个方向为YES
   */
  //    [PPUIKit setIsDeviceOrientationLandscapeLeft:NO];
  //    [PPUIKit setIsDeviceOrientationLandscapeRight:NO];
  [PPUIKit setIsDeviceOrientationPortrait:NO];
  [PPUIKit setIsDeviceOrientationPortraitUpsideDown:NO];
  return UIInterfaceOrientationMaskLandscape;
//  return UIInterfaceOrientationMaskAll;
}


//iOS 6.0以下旋屏
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
  /**iOS6以下旋屏处理。默认SDK与游戏客户端支持方向是同步的，也就是说游戏支持什么方向旋转，SDK则也支持。游戏不支持该方向。SDK也不支持。
   *如果特殊方向不需要SDK旋转，而游戏需要支持。请将PPUserUIKit所不支持的方向设置NO。
   *至少要保持一个方向为YES
   */
  
  //    [PPUIKit setIsDeviceOrientationLandscapeLeft:NO];
  //    [PPUIKit setIsDeviceOrientationLandscapeRight:NO];
      [PPUIKit setIsDeviceOrientationPortrait:NO];
  [PPUIKit setIsDeviceOrientationPortraitUpsideDown:NO];
  if (UIInterfaceOrientationIsLandscape(interfaceOrientation)) {
    return YES;
  }
  return YES;
}

#pragma mark - class init and singleton method
+ (AppPPManager *)sharedInstance {
  
  if (__instance == nil) {
    __instance = [[AppPPManager alloc] init];
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
  _isInit = NO;
      [self configurePPSDKWhenBootup];
  }
  return self;
}

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
  NSAssert(NO, @"Attempted to deallocate a singleton instance.");
    [super dealloc];
}
@end
