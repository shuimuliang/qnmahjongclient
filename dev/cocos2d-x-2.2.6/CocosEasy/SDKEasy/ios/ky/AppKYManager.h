//
//  AppPPManager.h
//  zhajinhua
//
//  Created by LiuLihua on 13-3-27.
//
//

#import <Foundation/Foundation.h>
#import "KYSDK.h"
#import "APPSDKBase.h"

typedef void (^AppKYLoginFinishBlock)(BOOL succeed);
typedef void (^AppKYBuyGoodsFinishBlock)(BOOL succeed, NSString * orderInfo);


@interface AppKYManager : APPSDKBase
<
KYSDKDelegate,KYMobilePayDelegate
>

+ (AppKYManager *)sharedInstance;
- (void)configureKYSDKWhenBootup;

// user related api
- (BOOL)isLogined;
- (BOOL)isInitKY;
- (void)loginKY:(AppKYLoginFinishBlock)finishBlock;
- (void)enterKY;
- (void)switchAccount;
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
            finishBlock:(AppKYBuyGoodsFinishBlock)buyFinishBlock;

@end

#define APP_KY_MANAGER [AppPPManager sharedInstance]