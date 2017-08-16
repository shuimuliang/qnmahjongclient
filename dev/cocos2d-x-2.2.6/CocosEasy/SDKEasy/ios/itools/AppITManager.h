//
//  AppPPManager.h
//  zhajinhua
//
//  Created by LiuLihua on 13-3-27.
//
//

#import <Foundation/Foundation.h>
#import "HXAppPlatformKitPro.h"
#import "APPSDKBase.h"


typedef void (^AppITLoginFinishBlock)(BOOL succeed);
typedef void (^AppITBuyGoodsFinishBlock)(BOOL succeed, NSString * orderInfo);


@interface AppITManager : APPSDKBase


+ (AppITManager *)sharedInstance;
- (void)configureITSDKWhenBootup;

// user related api
- (BOOL)isLogined;
- (BOOL)isInitIT;
- (void)loginIT:(AppITLoginFinishBlock)finishBlock;
- (void)enterIT;
- (void)switchAccount;
- (void)logout:(BOOL)iscallback;
- (void)addITObserver;
- (NSString *)nickName;
- (NSString *)loginUin;
- (NSString *)sessionId;

// buy goods
- (void)buyGood:(NSString *)goodid
          price:(NSInteger)price
           name:(NSString *)productName
            des:(NSString *)describe
            ext:(NSString *)extraData
            finishBlock:(AppITBuyGoodsFinishBlock)buyFinishBlock;

@end

#define APP_PP_MANAGER [AppPPManager sharedInstance]