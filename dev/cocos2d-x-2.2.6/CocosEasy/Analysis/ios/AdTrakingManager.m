//
//  AdTrakingManager.m
//  Casino
//
//  Created by liulihua on 15/10/12.
//
//

#import "AdTrakingManager.h"
#import "TalkingDataAppCpa.h"


@implementation AdTrakingManager
+ (void)initAdTraking{
#ifdef CASINO_ADTRAKING_APPID
    [TalkingDataAppCpa init:CASINO_ADTRAKING_APPID withChannelId:CASINO_ADTRAKING_CHANNEL_NAME];
#endif
}
+ (void)onRegister:(NSString *)account{
#ifdef CASINO_ADTRAKING_APPID
    [TalkingDataAppCpa onRegister:account];
#endif
}

+ (void)onLogin:(NSString *)account{
#ifdef CASINO_ADTRAKING_APPID
    [TalkingDataAppCpa onLogin:account];
#endif
}

+ (void) onPay:(NSString*)playerID
   withOrderId:(NSString *)orderID
    withAmount:(int)amount
withCurrencyType:(NSString*)currencyType
   withPayType:(NSString*)paymentType{
#ifdef CASINO_ADTRAKING_APPID
    [TalkingDataAppCpa onPay:playerID
                 withOrderId:orderID
                  withAmount:amount
            withCurrencyType:currencyType
                 withPayType:paymentType];
#endif
}
@end
