//
//  AppStoreManager.h
//  client
//
//  Created by liulihua on 14-4-8.
//
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import "APPSDKBase.h"
#import <Kamcord/Kamcord.h>

typedef enum {
    kAppPurchaseErrorNone = 0,
    kAppPurchaseErrorInProgressing,  //正在处理中
    kAppPurchaseErrorPurchaseNotAllowed,  // 用户没有购买权限
    kAppPurchaseErrorProductNotExit,
    kAppPurchaseErrorPaymentFailed,
    kAppPurchaseErrorProductIDRequired
} AppPurchaseErrorCode;

typedef void (^AppPurchaseFinishBlock)(AppPurchaseErrorCode errCode, SKPaymentTransaction* transactionInfo);


@interface AppStoreManager : APPSDKBase <SKProductsRequestDelegate, SKPaymentTransactionObserver,KamcordDelegate>{
    BOOL _isRecord;
}
+ (AppStoreManager *)sharedInstance;
- (void)startRecord;
- (void)configureAppStoreWhenBootup;
- (AppPurchaseErrorCode)buyGoods:(NSString*)productId
                     finishBlock:(AppPurchaseFinishBlock)purchaseCompleteBlock;
- (void)preFetchAppStoreProducts:(NSArray *)goodIDList;

@end
