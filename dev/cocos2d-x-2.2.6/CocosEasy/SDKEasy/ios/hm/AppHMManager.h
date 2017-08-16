//
//  AppPPManager.h
//  zhajinhua
//
//  Created by LiuLihua on 13-3-27.
//
//

#import <Foundation/Foundation.h>
#import <ZHPay/ZHPayPlatform.h>
#import "APPSDKBase.h"

typedef void (^AppHMLoginFinishBlock)(BOOL succeed);
typedef void (^AppHMBuyGoodsFinishBlock)(BOOL succeed, NSString * orderInfo);


@interface AppHMManager : APPSDKBase<ZHPayDelegate>
+ (AppHMManager *)sharedInstance;
- (void)configureHMSDKWhenBootup;
- (void)loginHM: (AppHMLoginFinishBlock)finishBlock;
- (NSString *)getNickName;
- (NSString *)getUserID;
- (NSString *)getSessionID;
- (BOOL)isLogined;
- (void)pay:(AppHMBuyGoodsFinishBlock)buyCallback
      order: (ZHPayOrderInfo*)orderInfo;
- (void)logout;
- (BOOL)shouldAutorotate;
- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window;
@end


#define APP_HM_MANAGER [AppHMManager sharedInstance]