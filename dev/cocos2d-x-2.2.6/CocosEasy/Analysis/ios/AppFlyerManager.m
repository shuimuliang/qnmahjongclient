//
//  AppFlyerManager.m
//  Casino
//
//  Created by liulihua on 15/7/12.
//
//

#import "AppFlyerManager.h"

static NSString * const kAppGlobalAppFlyerKey = @"AbUyD3v8pkuYHb4zBNszi";

@implementation AppFlyerManager
+ (void)initAppFlyer{
#ifndef CASINO_APPFLYER_ID
#error "should set appflyer id"
#endif
    
#ifdef CASINO_APPFLYER_ID
    [AppsFlyerTracker sharedTracker].appsFlyerDevKey = kAppGlobalAppFlyerKey;
    [AppsFlyerTracker sharedTracker].appleAppID = CASINO_APPFLYER_ID;
#endif
}
+ (void)trackAppLaunch{
#ifdef CASINO_APPFLYER_ID
    // Track Installs, updates & sessions(app opens) (You must include this API to enable tracking)
    [[AppsFlyerTracker sharedTracker] trackAppLaunch];
#endif
}

+ (void) trackEvent:(NSString *)eventName withValues:(NSDictionary*)values{
#ifdef CASINO_APPFLYER_ID
    [[AppsFlyerTracker sharedTracker] trackEvent:eventName withValues:values];
#endif
}

+ (void) trackPurchaseEvent:(NSString*)iapID
                paymentType:(NSString*)paymentType
               currencyType:(NSString*)currencyType
             currencyAmount:(NSNumber*)currencyAmount{
#ifdef CASINO_APPFLYER_ID
    [[AppsFlyerTracker sharedTracker] trackEvent:AFEventPurchase
                                      withValues: @{
                                                    AFEventParamContentId:iapID,
                                                    AFEventParamContentType : paymentType,
                                                    AFEventParamRevenue: currencyAmount,
                                                    AFEventParamCurrency:currencyType}];
#endif
}
@end
