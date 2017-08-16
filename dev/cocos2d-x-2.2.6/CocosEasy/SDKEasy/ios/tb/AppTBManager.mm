//
//  AppTBManager.m
//  zhajinhua
//
//  Created by LiuLihua on 13-3-27.
//
//

#import "AppTBManager.h"
#import "SDKManager.h"
#include <stdio.h>
#import "JSONKit.h"
#import "SVProgressHUD.h"

void CSDKManager::initPlatform(int code)
{
     m_ChannelID = SDK_CHANNEL_DEBUG;
    [AppTBManager sharedInstance];
}

void CSDKManager::preLoadGoods(const char* goodsList){
}


void CSDKManager::login()
{
    [[AppTBManager sharedInstance] loginTB:^(BOOL succ){
        this->loginCallBack(succ);
    }];
}
void CSDKManager::logout(bool iscallback)
{
    [[AppTBManager sharedInstance] logout:iscallback];
}
void CSDKManager::userCenter()
{
    [[AppTBManager sharedInstance] enterTB];
}
bool CSDKManager::isLogined()
{
    return [[AppTBManager sharedInstance] isLogined];
}
bool CSDKManager::isInited()
{
    return [[AppTBManager sharedInstance] isInitTB];
}

const std::string CSDKManager::getAccountName(){
    return [[[AppTBManager sharedInstance] nickName] UTF8String];
}
const std::string CSDKManager::getAccountID(){
    return [[[AppTBManager sharedInstance] loginUin] UTF8String];
}
const std::string CSDKManager::getSessionID(){
    return [[[AppTBManager sharedInstance] sessionId] UTF8String];
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
    [SVProgressHUD showWithStatus:@"请稍等..." maskType:SVProgressHUDMaskTypeClear];

    [[AppTBManager sharedInstance] buyGood:[NSString stringWithFormat:@"%d",ID]
                                     price:money
                                      name:nsname
                                       des:nsdescription
                                       ext:nsextraData
                               finishBlock:^(BOOL succ ,NSString * ord){
                                    [SVProgressHUD dismiss];
                                    this->buyCallBack(succ,[ord UTF8String]);
                               }];
    
}

static AppTBManager * __instance = nil;


@interface AppTBManager ()
@property (nonatomic, copy) AppTBLoginFinishBlock loginFinish;
@property (nonatomic, copy) AppTBBuyGoodsFinishBlock buyFinishBlock;
@end

@implementation AppTBManager{
    BOOL _isInit;
    NSString * _sessionID;
    NSString * _nickName;
    NSString * _userID;
    NSString *_currentSeriaNo;
    BOOL _needLogoutCallback;
}

- (void)configureTBSDKWhenBootup{
    [self addTBObserver];
    //SDK初始化，务必放在其他SDK接口的调用之前
   
    [[TBPlatform defaultPlatform] TBInitPlatformWithAppID:140616
                                        screenOrientation:UIDeviceOrientationPortrait
                          isContinueWhenCheckUpdateFailed:YES];
    [[TBPlatform defaultPlatform] TBSetAutoRotation:YES];
}

- (void)addTBObserver{
    //添加SDK各类通知的观察者
    /*初始化结束通知，登录等操作务必在收到该通知后调用！！*/
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(sdkInitFinished)
                                                 name:kTBInitDidFinishNotification
                                               object:Nil];
    /*登录成功通知*/
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(loginFinished)
                                                 name:kTBLoginNotification
                                               object:nil];
    /*注销通知（个人中心页面的注销也会触发该通知，注意处理*/
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(didLogout)
                                                 name:kTBUserLogoutNotification
                                               object:nil];
    /*离开平台通知（包括登录页面、个人中心页面、web充值页等*/
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(leavedSDKPlatform:)
                                                 name:kTBLeavePlatformNotification
                                               object:nil];
}


- (BOOL)isInitTB{
    return _isInit;
}

- (BOOL)isLogined{
    return  [[TBPlatform defaultPlatform] TBIsLogined];
}

- (void)loginTB:(AppTBLoginFinishBlock)finishBlock{
    [[TBPlatform defaultPlatform] TBLogin:0];
  self.loginFinish = finishBlock;
}

- (void)switchAccount{
     [[TBPlatform defaultPlatform] TBSwitchAccount];
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

- (void)enterTB{
    [[TBPlatform defaultPlatform] TBEnterUserCenter:0];
}

- (void)logout:(BOOL)iscallback{
    _needLogoutCallback = iscallback;
    [[TBPlatform defaultPlatform] TBLogout:0];
}

- (void)buyGood:(NSString *)goodid
          price:(NSInteger)price
           name:(NSString *)productName
            des:(NSString *)describe
            ext:(NSString *)extraData
    finishBlock:(AppTBBuyGoodsFinishBlock)buyFinishBlock{
    
    self.buyFinishBlock = buyFinishBlock;

    NSDictionary * dic = [extraData objectFromJSONString];
    NSString * accountName = [dic valueForKey:@"accountName"];
    NSString * accountID = [dic valueForKey:@"accountID"];
    NSString * token = [dic valueForKey:@"token"];
    NSString * serverID = [dic valueForKey:@"serverID"];
    
    int time = [[NSDate date] timeIntervalSince1970];
    _currentSeriaNo =  [NSString stringWithFormat:@"%@-%@-%d-%@-%@",@"TB",accountID,time,goodid,serverID];
    
    [[TBPlatform defaultPlatform] TBUniPayForCoin:_currentSeriaNo
                                       needPayRMB:price
                                   payDescription:describe
                                         delegate:self];

}


#pragma mark - 消息回调处理
/**
 *  SDK初始化结束通知（登录等操作务必放到初始化完成的通知里！！！！）
 */
- (void)sdkInitFinished{
    _isInit = YES;
}

/**
 *  离开平台
 *ˆ
 *  @param notification
 */
- (void)leavedSDKPlatform:(NSNotification *)notification{
    NSDictionary *notifyUserInfo = notification.userInfo;
    TBPlatformLeavedType leavedFromType = (TBPlatformLeavedType)[[notifyUserInfo objectForKey:
                                                                  TBLeavedPlatformTypeKey] intValue];
    switch (leavedFromType) {
            //从登录页离开
        case TBPlatformLeavedFromLogin:{
        }
            break;
            //从个人中心离开
        case TBPlatformLeavedFromUserCenter:{
        }
            break;
            //从充值页面离开
        case TBPlatformLeavedFromUserPay:{
            NSString *orderString = [notifyUserInfo objectForKey:TBLeavedPlatformOrderKey];
            [[TBPlatform defaultPlatform] TBCheckPaySuccess:orderString
                                                   delegate:self];
        }
            break;
        default:
            break;
    }
}

/**
 *  登录结束通知
 */
- (void)loginFinished{

     if (self.isLogined) {
        TBPlatformUserInfo *userInfo = [[TBPlatform defaultPlatform] TBGetMyInfo];
        _nickName = [userInfo nickName];
        _sessionID = [userInfo sessionID];
        _userID = [userInfo userID];
         
         //显示浮动工具条
         [[TBPlatform defaultPlatform] TBShowToolBar:TBToolBarAtMiddleLeft
                                       isUseOldPlace:YES];
    
     }
        if (self.loginFinish) {
            self.loginFinish([self isLogined]);
        }

}

/**
 *  注销通知
 */
- (void)didLogout{
    //    [self quickShowMessage:@"已注销"];
    [[TBPlatform defaultPlatform] TBHideToolBar];
    if (_needLogoutCallback){
        CSDKManager::getInstance()->logoutCallBack();
    }
    _userID = nil;
    _sessionID = nil;
    _nickName = nil;
}

#pragma mark - 购买物品回调

/**
 *	@brief	使用推币直接购买商品成功
 *
 *	@param 	order 	订单号
 */
- (void)TBBuyGoodsDidSuccessWithOrder:(NSString*)order{
    
    if (self.buyFinishBlock) {
        self.buyFinishBlock(YES, _currentSeriaNo);
    }
    
}

/**
 *	@brief	使用推币直接购买商品失败
 *
 *	@param 	order 	订单号
 *	@param 	errorType  错误类型，见TB_BUYGOODS_ERROR
 */
- (void)TBBuyGoodsDidFailedWithOrder:(NSString *)order
                          resultCode:(TB_BUYGOODS_ERROR)errorType{
 
    if (self.buyFinishBlock) {
        self.buyFinishBlock(NO, _currentSeriaNo);
    }
    [[AppTBManager sharedInstance] logSdkEvent:kAppEventSDKPayFailed errCode:errorType];
}

/**
 *	@brief	推币余额不足，进入充值页面（开发者需要手动查询订单以获取充值购买结果）
 *
 *	@param 	order 	订单号
 */
- (void)TBBuyGoodsDidStartRechargeWithOrder:(NSString*)order{
    
}

/**
 *	@brief  跳提示框时，用户取消
 *
 *	@param	order	订单号
 */
- (void)TBBuyGoodsDidCancelByUser:(NSString *)order{
    
    NSLog(@"cancel!!!!!!!");
}

#pragma mark - 查询订单回调

/**
 *  查询订单结束
 *
 *  @param orderString 订单号
 *  @param amount      订单金额（单位：分）
 *  @param statusType  订单状态（详见TBPlatformDefines.h）
 */
- (void)TBCheckOrderFinishedWithOrder:(NSString *)orderString
                               amount:(int)amount
                               status:(TBCheckOrderStatusType)statusType{
    switch (statusType) {
        case TBCheckOrderStatusSuccess:
        case TBCheckOrderStatusPaying:
        case TBCheckOrderStatusWaitingForPay:
            if (self.buyFinishBlock) {
                self.buyFinishBlock(YES, _currentSeriaNo);
            }
            break;
        case TBCheckOrderStatusFailed:
            [[AppTBManager sharedInstance] logSdkEvent:kAppEventSDKPayFailed errCode:statusType];
            if (self.buyFinishBlock) {
                self.buyFinishBlock(NO, _currentSeriaNo);
            }
            break;
                  default:
            break;
    }
   
}
/**
 *  @brief 查询订单失败（网络不通畅，或服务器返回错误）
 */
- (void)TBCheckOrderDidFailed:(NSString*)order{
  
    if (self.buyFinishBlock) {
        self.buyFinishBlock(NO, _currentSeriaNo);
    }
}

#pragma mark - class init and singleton method

+ (AppTBManager *)sharedInstance {
  
  if (__instance == nil) {
    __instance = [[AppTBManager alloc] init];
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
    [self configureTBSDKWhenBootup];
  }
  return self;
}

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
  NSAssert(NO, @"Attempted to deallocate a singleton instance.");
    [super dealloc];
}


@end
