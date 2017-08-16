//
//  PayPalManager.h
//  Casino
//
//  Created by liulihua on 15/9/16.
//
//

#import <Foundation/Foundation.h>
#import "PayPalMobile.h"
#import "APPSDKBase.h"

typedef void (^PayPalPurchaseFinishBlock)(AppPurchaseErrorCode errCode, NSString* orderInfo);

@interface PayPalManager : NSObject<PayPalPaymentDelegate>
+ (PayPalManager *)sharedInstance;
- (void)configurePayPalWhenBootup;

-(void)payWithRootViewController:(UIViewController *)controller
                       goodsInfo:(NSString *)infoStr
                     finishBlock:(PayPalPurchaseFinishBlock)finishBlock;
@end
