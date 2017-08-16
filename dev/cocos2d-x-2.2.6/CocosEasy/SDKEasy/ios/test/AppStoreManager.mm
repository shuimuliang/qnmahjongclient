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
#import "SVProgressHUD.h"
#import "NSData+Base64.h"
#import "PSAlertView.h"
#import "Reachability.h"
#import "Analysis.h"

//SDKManager interface
void CSDKManager::initPlatform(int code)
{
#if DEBUG
    m_ChannelID = SDK_CHANNEL_DEBUG;
#else
	m_ChannelID =  SDK_CHANNEL_DEBUG;
#endif
    [AppStoreManager sharedInstance];
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
    [[AppStoreManager sharedInstance] startRecord];
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

void CSDKManager::pay(unsigned int ID,
                      const char* name,
                      unsigned int money,
                      const char* description,
                      const char* extraData){
    
    [[AppStoreManager sharedInstance] startRecord];
    return;
    
    if (![[Reachability reachabilityForInternetConnection] isReachable]){
        PSAlertView *alert = [PSAlertView alertWithTitle:@"提示" message:@"网络超时，请稍后重试"];
        [alert setCancelButtonWithTitle:@"确定" block:^(void){
            // do nothing
        }];
        [alert show];
        return;
    }
    
    NSString * goodData =  extraData?[NSString stringWithUTF8String : extraData]:nil;
    NSDictionary *dataDic = [goodData objectFromJSONString];
    NSString * productId = [dataDic objectForKey:@"appStoreProductId"];
    
    [SVProgressHUD showWithStatus:@"请稍等..." maskType:SVProgressHUDMaskTypeClear];
    BOOL errCode = [[AppStoreManager sharedInstance] buyGoods:productId finishBlock:^(AppPurchaseErrorCode errCode, SKPaymentTransaction* transactionInfo){
        [SVProgressHUD dismiss];
        
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
        [SVProgressHUD dismiss];
        this->buyCallBack(NO,[productId UTF8String]);
    }
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

- (void)startRecord{
    if (_isRecord){
        [Kamcord stopRecording];
        [Kamcord showView];
        _isRecord = false;
        PSAlertView *alert = [PSAlertView alertWithTitle:@"提示" message:@"已停止录屏，视频存储在 相册中"];
        [alert setCancelButtonWithTitle:@"确定" block:^(void){
            // do nothing
        }];
        [alert show];
    }else{
        [Kamcord startRecording];
        _isRecord = true;
        PSAlertView *alert = [PSAlertView alertWithTitle:@"提示" message:@"已开始录屏"];
        [alert setCancelButtonWithTitle:@"确定" block:^(void){
            // do nothing
        }];
        [alert show];
    }
}

- (void)videoFinishedProcessing:(BOOL)success{
    if (success){
        NSLog(@"process finished...");
        [self saveRecord];
    }
}
- (void)saveRecord{
    NSString* documentPath = [NSHomeDirectory() stringByAppendingFormat:@"/Documents/"];
    NSArray *tmplist = [[NSFileManager defaultManager] subpathsOfDirectoryAtPath:documentPath error:nil];
    
    for (NSString *pathName in tmplist) {
        NSString *realPath = [documentPath stringByAppendingString:pathName];
        NSLog(@"pathName: %@",realPath);
        if ([[realPath pathExtension] isEqualToString:@"mp4"]){
            UISaveVideoAtPathToSavedPhotosAlbum( realPath, self, @selector(video:didFinishSavingWithError:contextInfo:), nil );

//            UIVideoAtPathIsCompatibleWithSavedPhotosAlbum(pathName);
        }
    }
}

- (void)video:(NSString *)videoPath didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo{
    NSLog(@"videoPath : %@ \n error: %@",videoPath,error);
}

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
        _isRecord = false;
        [Kamcord setDelegate:self];
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