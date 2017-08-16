//
//  APPSDKBase.h
//  client
//
//  Created by liulihua on 14-8-13.
//
//

#import <Foundation/Foundation.h>
typedef enum {
    kAppPurchaseErrorNone = 0,
    kAppPurchaseErrorInProgressing,  //正在处理中
    kAppPurchaseErrorPurchaseNotAllowed,  // 用户没有购买权限
    kAppPurchaseErrorProductNotExit,
    kAppPurchaseErrorPaymentFailed,
    kAppPurchaseErrorProductIDRequired,
    kAppPurchaseErrorUserCanceled,
    kAppPurchaseErrorNetError

} AppPurchaseErrorCode;

@interface APPSDKBase : NSObject
- (void)logSdkEvent:(const char *)eventID errCode:(int)errCode;
@end
