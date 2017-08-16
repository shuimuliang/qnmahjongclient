//
//  PayPalManager.m
//  Casino
//
//  Created by liulihua on 15/9/16.
//
//

#import "PayPalManager.h"
#import "JSONKit.h"
static PayPalManager * __instance = nil;

@interface PayPalManager ()

@property(nonatomic, strong, readwrite) PayPalConfiguration *payPalConfig;
@property (nonatomic, copy) PayPalPurchaseFinishBlock finishBlock;
@property (nonatomic, copy) NSString * goodsInfo;
@property (nonatomic, retain) PayPalPaymentViewController * paymentViewController;

@end

@implementation PayPalManager
- (void)configurePayPalWhenBootup{
    NSString* paypalClientID_Sandbox = @"AVNJj1L75UxFsQYwqBRk8s_5qc07ARsy-vznUCpnxLaP2pGqwzUXLFqaWANhBZjhGsrG83Ad7Xwtni1P";
    NSString* paypalClientID_Production = @"AVTYoKWoVolmUWZvFPIX9f--L-iXb7NKJpvwoPnaPSBTyHjmJ4YOFMcwc5zGPzV0af6RRpqaogCcd91R";
    [PayPalMobile initializeWithClientIdsForEnvironments:@{PayPalEnvironmentProduction : paypalClientID_Production,
                                                           PayPalEnvironmentSandbox : paypalClientID_Sandbox}];
#warning must modified when release
    [PayPalMobile preconnectWithEnvironment:PayPalEnvironmentProduction];

    // Set up payPalConfig
    _payPalConfig = [[PayPalConfiguration alloc] init];
    _payPalConfig.acceptCreditCards = NO;
    _payPalConfig.merchantName = @"Lucky Casino Master";
    _payPalConfig.merchantPrivacyPolicyURL = [NSURL URLWithString:@"https://www.paypal.com/webapps/mpp/ua/privacy-full"];
    _payPalConfig.merchantUserAgreementURL = [NSURL URLWithString:@"https://www.paypal.com/webapps/mpp/ua/useragreement-full"];
    
    // Setting the languageOrLocale property is optional.
    //
    // If you do not set languageOrLocale, then the PayPalPaymentViewController will present
    // its user interface according to the device's current language setting.
    //
    // Setting languageOrLocale to a particular language (e.g., @"es" for Spanish) or
    // locale (e.g., @"es_MX" for Mexican Spanish) forces the PayPalPaymentViewController
    // to use that language/locale.
    //
    // For full details, including a list of available languages and locales, see PayPalPaymentViewController.h.
    
    _payPalConfig.languageOrLocale = [NSLocale preferredLanguages][0];
    
    
    // Setting the payPalShippingAddressOption property is optional.
    //
    // See PayPalConfiguration.h for details.
    
    _payPalConfig.payPalShippingAddressOption = PayPalShippingAddressOptionPayPal;
}

-(void)payFinishWithErrorCode:(AppPurchaseErrorCode)errorCode
                      payment:(PayPalPayment *)completedPayment{
    if (self.finishBlock) {
        if(completedPayment){
            NSDictionary *jsonDic = completedPayment.confirmation;
            NSString *productID = @"";
            if(completedPayment.items > 0 ){
               PayPalItem* item = completedPayment.items[0];
                productID = item.sku;
            }
            NSDictionary *response = [jsonDic objectForKey:@"response"];
            NSString *orderID = [response objectForKey:@"id"];
            NSMutableDictionary *orderInfo = [[NSMutableDictionary alloc] initWithCapacity:0];
            [orderInfo setObject:orderID forKey:@"transaction_id"];
            [orderInfo setObject:productID forKey:@"appStoreProductId"];
            NSString *orderInfoJson = [orderInfo JSONString];
            self.finishBlock(errorCode,orderInfoJson);
            NSLog(@"paypal payment result : %@   \nxx:%@",completedPayment.confirmation,orderInfoJson);
        }else{
            self.finishBlock(errorCode,@"");
        }
    }
    self.finishBlock = NULL;
    self.goodsInfo = NULL;
    [self.paymentViewController dismissViewControllerAnimated:YES completion:^(void){
        self.paymentViewController = NULL;
    }];
}

-(void)payWithRootViewController:(UIViewController *)controller
                       goodsInfo:(NSString *)infoStr
                     finishBlock:(PayPalPurchaseFinishBlock)finishBlock{
    self.goodsInfo = infoStr;
    self.finishBlock = finishBlock;
    if(infoStr == NULL){
        [self payFinishWithErrorCode:kAppPurchaseErrorProductIDRequired payment:nil];
        return;
    }
    
    NSDictionary *dataDic = [infoStr objectFromJSONString];
    NSString * playerID = [dataDic objectForKey:@"playerID"];
    NSString * productID = [dataDic objectForKey:@"appStoreProductId"];
    NSString * platform = [dataDic objectForKey:@"channel"];
    
    NSString * goodsName = [dataDic objectForKey:@"name"];
    NSString * price = [[dataDic objectForKey:@"price"] stringValue];
    NSString * currencyType = [dataDic objectForKey:@"currencyType"];

    
    
    NSDecimalNumber *goodsPrice = [NSDecimalNumber decimalNumberWithString:price];
    
    PayPalItem *item1 = [PayPalItem itemWithName:goodsName
                                    withQuantity:1
                                       withPrice:goodsPrice
                                    withCurrency:currencyType
                                         withSku:productID];
    NSArray *items = @[item1];
    
    PayPalPayment *payment = [[PayPalPayment alloc] init];
    payment.amount = goodsPrice;
    payment.currencyCode = currencyType;
    payment.shortDescription = goodsName;
    payment.items = items;  // if not including multiple items, then leave payment.items as nil
    
    if (!payment.processable) {
        // This particular payment will always be processable. If, for
        // example, the amount was negative or the shortDescription was
        // empty, this payment wouldn't be processable, and you'd want
        // to handle that here.
        [self payFinishWithErrorCode:kAppPurchaseErrorPurchaseNotAllowed payment:nil];
        return;
    }
    self.paymentViewController = [[PayPalPaymentViewController alloc] initWithPayment:payment
                                                                        configuration:self.payPalConfig
                                                                             delegate:self];
    [controller presentViewController:self.paymentViewController animated:YES completion:nil];

}
#pragma mark PayPalPaymentDelegate methods

- (void)payPalPaymentViewController:(PayPalPaymentViewController *)paymentViewController didCompletePayment:(PayPalPayment *)completedPayment {
    NSLog(@"PayPal Payment Success!");
    [self payFinishWithErrorCode:kAppPurchaseErrorNone payment:completedPayment];
}

- (void)payPalPaymentDidCancel:(PayPalPaymentViewController *)paymentViewController {
    NSLog(@"PayPal Payment Canceled");
    [self payFinishWithErrorCode:kAppPurchaseErrorInProgressing payment:nil];
}

#pragma mark - class init and singleton method
+ (PayPalManager *)sharedInstance {
    
    if (__instance == nil) {
        __instance = [[PayPalManager alloc] init];
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
    }
    return self;
}

- (void)dealloc {
    NSAssert(NO, @"Attempted to deallocate a singleton instance.");
    [super dealloc];
}


@end
