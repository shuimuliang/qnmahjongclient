//
//  AppXYManager.h
//
//
//
//

#import <Foundation/Foundation.h>
#import <XYPlatform/XYPlatform.h>
#import "APPSDKBase.h"


typedef void (^AppXYLoginFinishBlock)(BOOL succeed);
typedef void (^AppXYBuyGoodsFinishBlock)(BOOL succeed, NSString * orderInfo);


@interface AppXYManager : APPSDKBase
<
  XYCheckPayOrderDelegate, XYPayDelegate
>

+ (AppXYManager *)sharedInstance;
- (void)configureXYSDKWhenBootup;

// user related api
- (BOOL)isLogined;
- (BOOL)isInitXY;
- (void)loginXY:(AppXYLoginFinishBlock)finishBlock;
- (void)enterXY;
- (void)switchAccount;
- (void)logout:(BOOL)iscallback;
- (void)addXYObserver;
- (NSString *)getNickName;
- (NSString *)getUserID;
- (NSString *)getSessionID;

// buy goods
- (void)buyGood:(NSDictionary *)dataDic
    finishBlock:(AppXYBuyGoodsFinishBlock)buyFinishBlock;

- (void)payCallback:(BOOL)result withInfo:(NSString *)orderID;
- (void)XYHandleOpenURL:(NSURL *)url;
@end

#define APP_PP_MANAGER [AppPPManager sharedInstance]
