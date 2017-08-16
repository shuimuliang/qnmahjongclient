//
//  AppPPManager.h
//  zhajinhua
//
//  Created by LiuLihua on 13-3-27.
//
//

#import <Foundation/Foundation.h>
#import <PPAppPlatformKit/PPAppPlatformKit.h>
#import "APPSDKBase.h"

typedef void (^AppPPLoginFinishBlock)(BOOL succeed);
typedef void (^AppPPBuyGoodsFinishBlock)(BOOL succeed, NSString * orderInfo);


@interface AppPPManager : APPSDKBase
<
PPAppPlatformKitDelegate
>

+ (AppPPManager *)sharedInstance;
- (void)configurePPSDKWhenBootup;

// user related api
- (BOOL)isLogined;
- (BOOL)isInitPP;
- (void)setAlixPayResult:(NSURL *)url;
- (void)loginPP:(AppPPLoginFinishBlock)finishBlock;
- (void)enterPP;
- (void)logout:(BOOL)iscallback;
- (NSString *)nickName;
- (NSString *)loginUin;
- (NSString *)sessionId;

// buy goods
- (void)buyGood:(NSString *)goodid
          price:(NSInteger)price
           name:(NSString *)productName
            des:(NSString *)describe
            ext:(NSString *)extraData
            finishBlock:(AppPPBuyGoodsFinishBlock)buyFinishBlock;

@end

#define APP_PP_MANAGER [AppPPManager sharedInstance]