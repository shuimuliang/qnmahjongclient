//
//  AppStoreManager.m
//  client
//
//  Created by liulihua on 14-4-8.
//
//

#import "AppStoreManager.h"
#import "SDKManager.h"
#import "CocosEasyPlugin.h"
#import "JSONKit.h"
#import "NSData+Base64.h"
#import "Analysis.h"

//SDKManager interface
void CSDKManager::initPlatform(int code)
{
#if DEBUG
    m_ChannelID = SDK_CHANNEL_MAC;
#else
	m_ChannelID =  SDK_CHANNEL_MAC;
#endif
    [AppStoreManager sharedInstance];
}
void CSDKManager::thirdLogin(int loginType){ // 第三方登陆

}

AppThirdLoginType CSDKManager::getLoginType()
{
    return kAppThirdLoginTypeNone;
}


void CSDKManager::login()
{
    this->loginCallBack(true);
    
}
void CSDKManager::logout(bool iscallback)
{
    if (iscallback){
        this->logoutCallBack();
    }
}
void CSDKManager::userCenter()
{
    
}
bool CSDKManager::isLogined(){
    return true;
}
bool CSDKManager::isInited()
{
    return true;
}
void CSDKManager::unInit(){
    
}
const std::string CSDKManager::getAccountName(){
    return CCEPlugin::getDeviceID();
}
const std::string CSDKManager::getAccountID(){
    return CCEPlugin::getDeviceID();
}
const std::string CSDKManager::getSessionID(){
    return CCEPlugin::getDeviceID();
}

void CSDKManager::preLoadGoods(const char* goodsList){
    NSString * idList =  goodsList?[NSString stringWithUTF8String : goodsList]:nil;
    NSArray *goodIDList = [idList objectFromJSONString];
    if (goodIDList == nil || goodIDList.count <= 0 ) return;

    [[AppStoreManager sharedInstance] preFetchAppStoreProducts:goodIDList];
}

void CSDKManager::pay(const char* extraData){
    
}
void CSDKManager::showWebPayView(std::string url,
                                 std::string jsonData){
}
void CSDKManager::payWithPayPal(const char* parmas){
}

//*******************  分割线  ******************************************//

//NSString * const kAppStoreErrorDomain = @"AppStoreErrorDomain";
static AppStoreManager * __instance = nil;
static NSCache * __products;

@interface AppStoreManager()
@property (nonatomic, copy) AppPurchaseFinishBlock purchaseCompleteBlock;
@property (nonatomic, assign) BOOL purchaseInProgressing;
@property (nonatomic, copy) NSString * pendingPurchaseProductId;
@property (nonatomic, strong) NSArray * gemEntityArr;
@property (nonatomic, strong) NSArray * goodsListForGem;
@end

@implementation AppStoreManager
- (void)configureAppStoreWhenBootup{
    NSLog(@"[AppStoreManager configureAppStoreWhenBootup]");
}

- (void)preFetchAppStoreProducts:(NSArray *)goodIDList {
    if (goodIDList == nil || goodIDList.count <= 0 ) return ;
}

- (AppPurchaseErrorCode)buyGoods:(NSString *)productId
                     finishBlock:(AppPurchaseFinishBlock)purchaseCompleteBlock {
    return kAppPurchaseErrorNone;
}


- (void)callFinishBlockWithErrorCode:(AppPurchaseErrorCode)errCode
                         transaction:(SKPaymentTransaction *)transactionInfo {
    
   
}

// 追踪以往订单的时候使用
- (void)restoreTransaction:(SKPaymentTransaction *)transaction {
    self.purchaseInProgressing = NO;
    self.pendingPurchaseProductId = nil;
}

#pragma mark - SKProductsRequestDelegate
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    
    
}

#pragma mark - SKPaymentTransactionObserver
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions {
    
   
}


#pragma mark - class init and singleton method
+ (AppStoreManager *)sharedInstance {
    
    if (__instance == nil) {
        __instance = [[AppStoreManager alloc] init];
        __products = [[NSCache alloc] init];
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
        // init here
        [self configureAppStoreWhenBootup];
    }
    return self;
}

- (void)dealloc {
	NSAssert(NO, @"Attempted to deallocate a singleton instance.");
    [super dealloc];
}
@end