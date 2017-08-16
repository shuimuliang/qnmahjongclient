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

typedef void (^AppPurchaseFinishBlock)(AppPurchaseErrorCode errCode, SKPaymentTransaction* transactionInfo);


@interface AppStoreManager : APPSDKBase <SKProductsRequestDelegate, SKPaymentTransactionObserver>
+ (AppStoreManager *)sharedInstance;
- (void)configureAppStoreWhenBootup;
- (AppPurchaseErrorCode)buyGoods:(NSString*)productId
                     finishBlock:(AppPurchaseFinishBlock)purchaseCompleteBlock;
- (void)preFetchAppStoreProducts:(NSArray *)goodIDList;


- (NSString *)getMissOrderList;
- (NSString *)getOrderInfoAndRemove:(NSString*) transactionIdentifier;
@end
