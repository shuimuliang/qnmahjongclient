//
//  ThirdPartyManager.h
//  Casino
//
//  Created by liulihua on 15/8/14.
//
//

#import <Foundation/Foundation.h>

typedef enum {
    kAppThirdLoginErrorCodeNone = 0,
    kAppThirdLoginErrorCodeCanceld = 1,  //用户取消
    kAppThirdLoginErrorCodeFailed = 2, //失败
    kAppThirdLoginErrorCodeInfoPoor = 3  //信息不足
} AppThirdLoginErrorCode;

typedef void (^AppThirdPartyLoginFinishBlock)(AppThirdLoginErrorCode errCode, NSDictionary* resultDic);

@interface ThirdPartyManager : NSObject
+ (ThirdPartyManager *)sharedInstance;
- (void)loginWithThirdPartyType:(int)thirdType finish:(AppThirdPartyLoginFinishBlock)finishBlock;
- (void)logout;
@end
