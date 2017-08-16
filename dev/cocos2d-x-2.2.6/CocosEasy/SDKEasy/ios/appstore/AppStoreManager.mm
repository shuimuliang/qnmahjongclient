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
#import "CakeProgressHUD.h"
#import "NSData+Base64.h"
#import "PSAlertView.h"
#import "Reachability.h"
#import "Analysis.h"
#import "IappayViewController.h"
#import "AppUtils.h"
#import "ThirdPartyManager.h"

#if CASINO_PAYPAL
#import "PayPalManager.h"
#endif

//SDKManager interface
void CSDKManager::initPlatform(int code)
{
#if  0//DEBUG
    m_ChannelID = SDK_CHANNEL_DEBUG;
#else
    m_ChannelID =  CASINO_CHANNEL_ID; //11300; //
#endif
    
    
    [AppStoreManager sharedInstance];
}

AppThirdLoginType CSDKManager::getLoginType()
{
    return CASINO_LOGIN_TYPE;
}

void CSDKManager::login()
{
    this->loginCallBack(true);
    
}
void CSDKManager::logout(bool iscallback)
{
    [[ThirdPartyManager sharedInstance] logout];
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

void CSDKManager::showWebPayView(std::string url,
                    std::string jsonData){
        if (url.length() <= 0)return;
        NSString *urlStr = [NSString stringWithUTF8String:url.c_str()] ;
        UIViewController* rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
    NSString * goodData =  jsonData.c_str()?[NSString stringWithUTF8String : jsonData.c_str()]:nil;
        IappayViewController *iappayVC = [[IappayViewController alloc] initWithURL:urlStr payInfo:goodData finishBlock:^(AppPurchaseErrorCode errorCode,NSString *orderInfo){
            BOOL succ = (errorCode == kAppPurchaseErrorNone || errorCode == kAppPurchaseErrorInProgressing);
            NSLog(@"%ld , %@",(long)errorCode,orderInfo);
            NSMutableDictionary *dic = [NSMutableDictionary dictionaryWithCapacity:1];
            [dic setObject:orderInfo forKey:@"transaction_id"];
            NSString *buyInfo = [dic JSONString];
            this->buyCallBack(succ,[buyInfo UTF8String]);
        }];
        UINavigationController* navVC = [[UINavigationController alloc]initWithRootViewController:iappayVC];
        [rootViewController  presentViewController:navVC animated:YES completion:NULL];
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
    NSString * productId = [dataDic objectForKey:@"appStoreProductId"];
    
    [CakeProgressHUD showWithStatus:APP_LOCALE(@"common.wait") maskType:CakeProgressHUDMaskTypeClear];
    BOOL errCode = [[AppStoreManager sharedInstance] buyGoods:productId finishBlock:^(AppPurchaseErrorCode errCode, SKPaymentTransaction* transactionInfo){
        [CakeProgressHUD dismiss];
        
        BOOL succ = (errCode == kAppPurchaseErrorNone || errCode == kAppPurchaseErrorInProgressing);
        if (succ) {
            NSMutableDictionary *dic = [NSMutableDictionary dictionaryWithCapacity:1];
            [dic setObject:transactionInfo.transactionIdentifier forKey:@"transaction_id"];
            [dic setObject: [transactionInfo.transactionReceipt base64EncodedString]  forKey:@"receipt-data"];
            NSString *buyInfo = [dic JSONString];
            this->buyCallBack(succ,[buyInfo UTF8String]);
        }else{
            [[AppStoreManager sharedInstance] logSdkEvent:kAppEventSDKPayFailed errCode:errCode];
            this->buyCallBack(NO,[productId UTF8String]);
        }
        
    }];
    
    if (errCode != kAppPurchaseErrorNone) {
        [CakeProgressHUD dismiss];
        this->buyCallBack(NO,[productId UTF8String]);
    }
}

void CSDKManager::payWithPayPal(const char* parmas){
#if !CASINO_PAYPAL 
    this->buyCallBack(NO,"");
    return;
#else
    
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
    NSString * productId = [dataDic objectForKey:@"appStoreProductId"];
    
    UIViewController* rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
    [[PayPalManager sharedInstance] payWithRootViewController:rootViewController goodsInfo:goodData finishBlock:^(AppPurchaseErrorCode errCode, NSString* orderInfo){
        BOOL succ = (errCode == kAppPurchaseErrorNone || errCode == kAppPurchaseErrorInProgressing);
        if (succ) {
            NSLog(@"orderInfoorderInfoorderInfoorderInfo : %@",orderInfo);
            NSString *jsonData = orderInfo;
            if(orderInfo == NULL){
                jsonData = @"";
            }
            this->buyCallBack(succ,[jsonData UTF8String]);
        }else{
            [[AppStoreManager sharedInstance] logSdkEvent:kAppEventSDKPayFailed errCode:errCode];
            this->buyCallBack(NO,[productId UTF8String]);
        }
    }];
#endif
}


const std::string  CSDKManager::getMissOrderList(){
    NSString*missOrderList = [[AppStoreManager sharedInstance] getMissOrderList];
    missOrderList = missOrderList ? missOrderList : @"";
    
    NSLog(@"haha   missOrderList : %@",missOrderList);
    
    return [missOrderList UTF8String];
}
const std::string CSDKManager::getOrderInfoAndRemove(const char* orderID){
    NSString * transactionIdentifier =  orderID?[NSString stringWithUTF8String : orderID]:nil;
    
    NSString* orderInfo = [[AppStoreManager sharedInstance] getOrderInfoAndRemove:transactionIdentifier];
    orderInfo = orderInfo ? orderInfo : @"";
    NSLog(@"haha   orderInfo : %@",orderInfo);
    
    return [orderInfo UTF8String];
}


//*******************  分割线  ******************************************//

//NSString * const kAppStoreErrorDomain = @"AppStoreErrorDomain";
static AppStoreManager * __instance = nil;
static NSString * const kMissOrderList = @"kMissOrderList";
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
    // create product identity
    NSMutableArray *productsIdentities = [[NSMutableArray alloc] initWithCapacity:1];
    
    
    for (int i = 0 ;i < goodIDList.count; i++) {
        NSString * productIdentity = [goodIDList objectAtIndex:i];
        [productsIdentities addObject:productIdentity];
    }
    // load products
    if (productsIdentities == nil || [productsIdentities count] < 1) {
        return;
    }
    SKProductsRequest * productInfoReq = [[SKProductsRequest alloc]
                                          initWithProductIdentifiers:[NSSet setWithArray:productsIdentities]];
    productInfoReq.delegate = self;
    [productInfoReq start];
}

- (AppPurchaseErrorCode)buyGoods:(NSString *)productId
                     finishBlock:(AppPurchaseFinishBlock)purchaseCompleteBlock {
    if (productId == nil || [productId length] < 1) {
        return kAppPurchaseErrorProductIDRequired;
    } else if ([SKPaymentQueue canMakePayments] == NO) {
        return kAppPurchaseErrorPurchaseNotAllowed;
    } else if (self.purchaseInProgressing) {
        return kAppPurchaseErrorInProgressing;
    } else {
        self.purchaseInProgressing = YES;
        self.pendingPurchaseProductId = productId;
        self.purchaseCompleteBlock = purchaseCompleteBlock;
        SKProduct * aProduct = [__products objectForKey:productId];
        if (aProduct) {
            [self purchaseProduct:aProduct];
        } else {
            SKProductsRequest * productInfoReq = [[SKProductsRequest alloc]
                                                  initWithProductIdentifiers:[NSSet setWithObject:productId]];
            productInfoReq.delegate = self;
            [productInfoReq start];
        }
        return kAppPurchaseErrorNone;
    }
}

#pragma mark - inner methods
- (void)purchaseProduct:(SKProduct *)product {
    if ([SKPaymentQueue canMakePayments] == NO) {
        [self callFinishBlockWithErrorCode:kAppPurchaseErrorPurchaseNotAllowed transaction:nil];
    } else {
        SKPayment *payment = [SKPayment paymentWithProduct:product];
        [[SKPaymentQueue defaultQueue] addPayment:payment];
    }
}

- (void)completeTransaction:(SKPaymentTransaction *)transaction {
    [self callFinishBlockWithErrorCode:kAppPurchaseErrorNone transaction:transaction];
    // Remove the transaction from the payment queue.
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction {
    [self callFinishBlockWithErrorCode:kAppPurchaseErrorPaymentFailed transaction:transaction];
    @try {
        [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    }
    @catch (NSException *exception) {
        NSLog(@" exception : %@",exception);
    }
}

- (void)callFinishBlockWithErrorCode:(AppPurchaseErrorCode)errCode
                         transaction:(SKPaymentTransaction *)transactionInfo {
    
    if (self.purchaseCompleteBlock) {
        self.purchaseCompleteBlock(errCode, transactionInfo);
    }else{
        if(errCode == kAppPurchaseErrorNone){
            [self saveMissOrder:transactionInfo];
        }
    }
    self.purchaseInProgressing = NO;
    self.pendingPurchaseProductId = nil;
    self.purchaseCompleteBlock = nil;
}

// 追踪以往订单的时候使用
- (void)restoreTransaction:(SKPaymentTransaction *)transaction {
    self.purchaseInProgressing = NO;
    self.pendingPurchaseProductId = nil;
}

#pragma mark - SKProductsRequestDelegate
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    
    NSArray * tests = response.invalidProductIdentifiers;
    if ([tests count] < 1) {
        NSLog(@"purchase failed");
        
    }else{
        NSLog(@"purchase failed");
        
    }
    NSArray * products = response.products;
    if ([products count] < 1) {
        NSLog(@"purchase failed");
        [self callFinishBlockWithErrorCode:kAppPurchaseErrorProductNotExit transaction:nil];
    } else {
        
        for (SKProduct * aProduct in products) {
            [__products setObject:aProduct forKey:aProduct.productIdentifier];
        }
        if (self.purchaseInProgressing && self.pendingPurchaseProductId) {
            SKProduct * purchasingProduct = [__products objectForKey:self.pendingPurchaseProductId];
            if (purchasingProduct) {
                self.pendingPurchaseProductId = nil;
                [self purchaseProduct:purchasingProduct];
            }
        }
    }
}

#pragma mark - SKPaymentTransactionObserver
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions {
    
    for (SKPaymentTransaction *transaction in transactions) {
        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
            default:
                break;
        }
    }
}


- (void)saveMissOrder:(SKPaymentTransaction*)transactionInfo{
    if (transactionInfo == NULL) return;
    NSDictionary*dic = [[NSUserDefaults standardUserDefaults] objectForKey:kMissOrderList];
    NSMutableDictionary *mutableDic = [[NSMutableDictionary alloc] initWithDictionary:dic];
    if(mutableDic == NULL){
        mutableDic = [[NSMutableDictionary alloc] initWithCapacity:1];
    }
    NSMutableDictionary *orderInfoDic = [NSMutableDictionary dictionaryWithCapacity:1];
    [orderInfoDic setObject:transactionInfo.transactionIdentifier forKey:@"transaction_id"];
    [orderInfoDic setObject: [transactionInfo.transactionReceipt base64EncodedString]  forKey:@"receipt-data"];
    NSString *buyInfo = [orderInfoDic JSONString];
    [mutableDic setObject:buyInfo forKey:transactionInfo.transactionIdentifier];
    [[NSUserDefaults standardUserDefaults] setObject:mutableDic forKey:kMissOrderList];
}


- (NSString *)getMissOrderList{
    NSMutableDictionary *mutableDic = [[NSUserDefaults standardUserDefaults] objectForKey:kMissOrderList];
    if(mutableDic == NULL){
        mutableDic = [[NSMutableDictionary alloc] initWithCapacity:1];
    }
    NSString* resultStr = @"";
    NSArray *transitions = mutableDic.allKeys;
    if (transitions && transitions.count > 0){
        resultStr = [transitions JSONString];
    }
    NSLog(@"%@",resultStr);
    return  resultStr;
}
- (NSString *)getOrderInfoAndRemove:(NSString*) transactionIdentifier{
    if (transactionIdentifier == NULL || transactionIdentifier.length <= 0 ) return @"";
    NSDictionary*dic = [[NSUserDefaults standardUserDefaults] objectForKey:kMissOrderList];
    NSMutableDictionary *mutableDic = [[NSMutableDictionary alloc] initWithDictionary:dic];
    if(mutableDic == NULL){
        mutableDic = [[NSMutableDictionary alloc] initWithCapacity:1];
    }
    NSString* resultStr = @"";
    if([mutableDic.allKeys containsObject:transactionIdentifier]){
        resultStr = [[NSString alloc] initWithString: [mutableDic objectForKey:transactionIdentifier]];
        [mutableDic removeObjectForKey:transactionIdentifier];
        [[NSUserDefaults standardUserDefaults] setObject:mutableDic forKey:kMissOrderList];
    }
    NSLog(@"%@",resultStr);
    return resultStr;
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
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
        // init here
        [self configureAppStoreWhenBootup];
    }
    return self;
}

- (void)dealloc {
	NSAssert(NO, @"Attempted to deallocate a singleton instance.");
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
    [super dealloc];
}
@end
