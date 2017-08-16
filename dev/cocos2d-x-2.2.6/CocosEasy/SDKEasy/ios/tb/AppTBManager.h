//
//  AppPPManager.h
//  zhajinhua
//
//  Created by LiuLihua on 13-3-27.
//
//

#import <Foundation/Foundation.h>
#import <TBPlatform/TBPlatform.h>
#import "APPSDKBase.h"


typedef void (^AppTBLoginFinishBlock)(BOOL succeed);
typedef void (^AppTBBuyGoodsFinishBlock)(BOOL succeed, NSString * orderInfo);


@interface AppTBManager : APPSDKBase
<
TBBuyGoodsProtocol,TBCheckOrderDelegate
>

+ (AppTBManager *)sharedInstance;
- (void)configureTBSDKWhenBootup;

// user related api
- (BOOL)isLogined;
- (BOOL)isInitTB;
- (void)loginTB:(AppTBLoginFinishBlock)finishBlock;
- (void)enterTB;
- (void)switchAccount;
- (void)logout:(BOOL)iscallback;
- (void)addTBObserver;
- (NSString *)nickName;
- (NSString *)loginUin;
- (NSString *)sessionId;

// buy goods
- (void)buyGood:(NSString *)goodid
          price:(NSInteger)price
           name:(NSString *)productName
            des:(NSString *)describe
            ext:(NSString *)extraData
            finishBlock:(AppTBBuyGoodsFinishBlock)buyFinishBlock;

@end

#define APP_PP_MANAGER [AppPPManager sharedInstance]