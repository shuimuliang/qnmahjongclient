//
//  AdTrakingManager.h
//  Casino
//
//  Created by liulihua on 15/10/12.
//
//

#import <Foundation/Foundation.h>

@interface AdTrakingManager : NSObject
+ (void)initAdTraking;
+ (void)onRegister:(NSString *)account;

+ (void)onLogin:(NSString *)account;
+ (void) onPay:(NSString*)playerID
   withOrderId:(NSString *)orderID
    withAmount:(int)amount
withCurrencyType:(NSString*)currencyType
   withPayType:(NSString*)paymentType;

@end
