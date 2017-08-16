//
//  ThirdPartyManager.m
//  Casino
//
//  Created by liulihua on 15/8/14.
//
//

#import "ThirdPartyManager.h"
#import "SDKConstants.h"
#ifdef CASINO_THIRD_FACEBOOK
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#endif

static ThirdPartyManager * __instance = nil;
@implementation ThirdPartyManager
- (void)logout{
#ifdef CASINO_THIRD_FACEBOOK
    [FBSDKAccessToken setCurrentAccessToken:nil];
#endif
}

- (void)loginWithThirdPartyType:(int)thirdType
                         finish:(AppThirdPartyLoginFinishBlock)finishBlock{
    NSMutableDictionary *dic = [[NSMutableDictionary alloc] initWithCapacity:1];
    if(thirdType == kAppThirdLoginTypeFacebook){
#ifdef CASINO_THIRD_FACEBOOK
        FBSDKAccessToken *curToken = [FBSDKAccessToken currentAccessToken];
        if(curToken){
            [dic setObject:curToken.tokenString forKey:@"tokenString"];
            [dic setObject:curToken.userID forKey:@"userID"];
            finishBlock(kAppThirdLoginErrorCodeNone,dic);
        }else{
            FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
            [login logInWithReadPermissions:@[@"public_profile",@"email"] handler:^(FBSDKLoginManagerLoginResult *result, NSError *error) {
                if (error) {
                    // Process error
                    finishBlock(kAppThirdLoginErrorCodeFailed,dic);
                } else if (result.isCancelled) {
                    finishBlock(kAppThirdLoginErrorCodeCanceld,dic);
                    // Handle cancellations
                } else {
                    // If you ask for multiple permissions at once, you
                    // should check if specific permissions missing
                    if ([result.grantedPermissions containsObject:@"email"]) {
                        // Do work
                        FBSDKAccessToken* token = result.token;
                        [dic setObject:token.tokenString forKey:@"tokenString"];
                        [dic setObject:token.userID forKey:@"userID"];
                        finishBlock(kAppThirdLoginErrorCodeNone,dic);
                    }else{
                        finishBlock(kAppThirdLoginErrorCodeInfoPoor,dic);
                    }
                }
            }];
        }
#else
        finishBlock(kAppThirdLoginErrorCodeFailed,dic);
#endif
    }else{
        finishBlock(kAppThirdLoginErrorCodeFailed,dic);
    }
}

#pragma mark - class init and singleton method
+ (ThirdPartyManager *)sharedInstance {
    
    if (__instance == nil) {
        __instance = [[ThirdPartyManager alloc] init];
    }
    return __instance;
}

+(id)alloc {
    
    NSAssert(__instance == nil, @"Attempted to allocate a second instance of a singleton.");
    return [super alloc];
}

- (id)init {
    
    self = [super init];
    if (self) {
        // init here
    }
    return self;
}

- (void)dealloc {
    NSAssert(NO, @"Attempted to deallocate a singleton instance.");
    [super dealloc];
}
@end
