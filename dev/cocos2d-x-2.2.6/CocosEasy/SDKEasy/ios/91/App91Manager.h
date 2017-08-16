//
//  App91Manager.h
//  client
//
//  Created by liulihua on 14-4-8.
//
//

#import <Foundation/Foundation.h>
#import <NdComPlatform/NDComPlatform.h>
#import <NdComPlatform/NdComPlatformAPIResponse.h>
#import <NdComPlatform/NdCPNotifications.h>
#import "APPSDKBase.h"
//#define APP_DEBUG_91_PURCHASE 1




typedef void (^App91LoginFinishBlock)(BOOL succeed, NSError *err);
typedef void (^App91BuyGoodsFinishBlock)(BOOL succeed, NdBuyInfo* buyInfo);

@interface App91Manager : APPSDKBase

+ (App91Manager *)sharedInstance;
- (void)configure91SDKWhenBootup;

// user related api
- (BOOL)isInit91;
- (BOOL)isLogined;
- (void)login91:(App91LoginFinishBlock)finishBlock;
- (void)enter91;
- (void)logout:(BOOL)iscallback;
- (void)show91Profile;
- (NSString *)encryptedUUIDFor91;
- (void)checkMissingOrder;
- (NSString *)nickName;
- (NSString *)loginUin;
- (NSString *)sessionId;

- (void)showPause;

- (void)userFeedback;
- (void)enterAppBBS;
// buy goods
- (void)buyGood:(NSString *)goodid
          price:(NSInteger)price
           name:(NSString *)productName
            des:(NSString *)describe
            ext:(NSString *)extraData
    finishBlock:(App91BuyGoodsFinishBlock)buyFinishBlock;


@end