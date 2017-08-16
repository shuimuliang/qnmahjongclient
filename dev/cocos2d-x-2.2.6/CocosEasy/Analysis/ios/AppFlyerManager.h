//
//  AppFlyerManager.h
//  Casino
//
//  Created by liulihua on 15/7/12.
//
//

#import <Foundation/Foundation.h>
#import "AppsFlyerTracker.h"

@interface AppFlyerManager : NSObject
+ (void)initAppFlyer;
+ (void)trackAppLaunch;
+ (void) trackPurchaseEvent:(NSString*)iapID
                paymentType:(NSString*)paymentType
               currencyType:(NSString*)currencyType
             currencyAmount:(NSNumber*)currencyAmount;
@end
