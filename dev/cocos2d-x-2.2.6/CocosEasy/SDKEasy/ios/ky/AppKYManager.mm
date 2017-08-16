//
//  AppTBManager.m
//  zhajinhua
//
//  Created by LiuLihua on 13-3-27.
//
//

#import "AppKYManager.h"
#import "SDKManager.h"
#include <stdio.h>
#import "JSONKit.h"


void CSDKManager::initPlatform(int code)
{
    m_ChannelID = SDK_CHANNEL_DEBUG;
    [AppKYManager sharedInstance];
 }

void CSDKManager::preLoadGoods(const char* goodsList){
}


void CSDKManager::login()
{
    [[AppKYManager sharedInstance] loginKY:^(BOOL succ){
        this->loginCallBack(succ);
    }];
}
void CSDKManager::logout(bool iscallback)
{
    [[AppKYManager sharedInstance] logout:iscallback];
}
void CSDKManager::userCenter()
{
    [[AppKYManager sharedInstance] enterKY];
}
bool CSDKManager::isLogined()
{
    return [[AppKYManager sharedInstance] isLogined];
}
bool CSDKManager::isInited()
{
    return [[AppKYManager sharedInstance] isInitKY];
}

const std::string CSDKManager::getAccountName(){
    return [[[AppKYManager sharedInstance] nickName] UTF8String];
}
const std::string CSDKManager::getAccountID(){
    return [[[AppKYManager sharedInstance] loginUin] UTF8String];
}
const std::string CSDKManager::getSessionID(){
    return [[[AppKYManager sharedInstance] sessionId] UTF8String];
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
    
    [[AppKYManager sharedInstance] buyGood:[NSString stringWithFormat:@"%d",ID]
                                     price:money
                                      name:nsname
                                       des:nsdescription
                                       ext:nsextraData
                               finishBlock:^(BOOL succ ,NSString * ord){
                                   this->buyCallBack(succ,[ord UTF8String]);
                               }];
    
    
}

static AppKYManager * __instance = nil;


@interface AppKYManager ()
@property (nonatomic, copy) AppKYLoginFinishBlock loginFinish;
@property (nonatomic, copy) AppKYBuyGoodsFinishBlock buyFinishBlock;
@end

@implementation AppKYManager{
    BOOL _isInit;
    BOOL _isLogined;
    NSString * _sessionID;
    NSString * _nickName;
    NSString * _userID;
    
    NSString * _token;
    NSString *_currentSeriaNo;
    
    BOOL _needLogoutCallback;
}

- (void)configureKYSDKWhenBootup{
    //SDK初始化，务必放在其他SDK接口的调用之前
    
    [[KYSDK instance] setSdkdelegate:self];
    [self sdkInitFinished];   //该sdk不需要初始化
}

- (BOOL)isInitKY{
    return _isInit;
}

- (BOOL)isLogined{
    return _isLogined;
}

- (void)loginKY:(AppKYLoginFinishBlock)finishBlock{
    [[KYSDK instance] changeLogOption:KYLOG_OFFGAMENAME];
    [[KYSDK instance] setISSendSMS:NO];
    [[KYSDK instance] showUserView];
    
    self.loginFinish = finishBlock;
}

- (void)switchAccount{
    [[KYSDK instance] userBackToLog];
}

- (void)enterKY{
    [[KYSDK instance] setUpUser];
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

- (void)logout:(BOOL)iscallback{
    _needLogoutCallback = iscallback;
     [[KYSDK instance] userBackToLog];
}

- (void)buyGood:(NSString *)goodid
          price:(NSInteger)price
           name:(NSString *)productName
            des:(NSString *)describe
            ext:(NSString *)extraData
    finishBlock:(AppKYBuyGoodsFinishBlock)buyFinishBlock{
    
    self.buyFinishBlock = buyFinishBlock;

    NSDictionary * dic = [extraData objectFromJSONString];
    NSString * accountName = [dic valueForKey:@"accountName"];
    NSString * accountID = [dic valueForKey:@"accountID"];
    NSString * token = [dic valueForKey:@"token"];
    NSString * serverID = [dic valueForKey:@"serverID"];
    
    NSString * priceString = [NSString stringWithFormat:@"%d", price];
    
    int time = [[NSDate date] timeIntervalSince1970];
    _currentSeriaNo =  [NSString stringWithFormat:@"%@-%@-%d-%@-%@",@"TY",accountID,time,goodid,serverID];
    
    //其中，game和md5key还未申请
    [[KYSDK instance] showPayWith:_currentSeriaNo fee:priceString game:@"957" gamesvr:serverID subject:productName md5Key:@"3PXupxWAJoR5ULbgU6J82FLZxamxYJwi" userId:accountID];

}


#pragma mark - 消息回调处理
/**
 *  SDK初始化结束通知（登录等操作务必放到初始化完成的通知里！！！！）
 */
- (void)sdkInitFinished{
    _isInit = YES;
}


/**
 *  @method-(void)logCallBack:(NSString *)tokenKey
 *  用户登录回调
 *  @param  tokenKey
 **/
-(void)loginCallBack:(NSString *)tokenKey{
    NSLog(@"login callback!!!");
    
    _token = tokenKey;
    
    if (self.loginFinish) {
        self.loginFinish(YES);
    }
}

/**
 *  @method-(void)quickLogCallBack:(NSString *)tokenKey
 *  用户快速登录回调
 *  @param  tokenKey
 **/
-(void)quickLogCallBack:(NSString *)tokenKey{
    NSLog(@"quick login callback!!!");
    
    _token = tokenKey;
    
    if (self.loginFinish) {
        self.loginFinish(YES);
    }
}

/**
 *  @method-(void)logOutCallBack:(NSString *)guid
 *  注销方法，userLogOut该方法回调
 *  @param  guid
 **/
-(void)logOutCallBack:(NSString *)guid{
    _token = nil;
    if (_needLogoutCallback){
        CSDKManager::getInstance()->logoutCallBack();
    }
}


- (void)onBillingResult:(BillingResultType)resultCode billingIndex:(NSString *)index message:(NSString *)message{
    
}



#pragma mark - class init and singleton method

+ (AppKYManager *)sharedInstance {
  
  if (__instance == nil) {
    __instance = [[AppKYManager alloc] init];
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
  [self configureKYSDKWhenBootup];
  }
  return self;
}

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
  NSAssert(NO, @"Attempted to deallocate a singleton instance.");
    [super dealloc];
}


@end
