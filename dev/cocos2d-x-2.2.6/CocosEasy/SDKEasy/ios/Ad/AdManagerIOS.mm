//
//  AdManagerIOS.m
//  testAd
//
//  Created by liulihua on 15/1/19.
//  Copyright (c) 2015年 snowCat. All rights reserved.
//

#import "AdManagerIOS.h"
#import "SDKManager.h"
#import "Analysis.h"
#if CASINO_ENABLE_AD
#endif
//#import "AdManagerIOS+Admob.h"
#import "AppUtils.h"

void CSDKManager::playVideo(const char* parmas){

}

static AdManagerIOS * __instance = nil;
@interface AdManagerIOS()
@end


@implementation AdManagerIOS
- (void)configureAdWhenBootup{
}

- (BOOL)isVideoAvalible{
    return YES;
}

- (void)showVideoAdWithViewController:(UIViewController *)viewController
                             playerID:(NSString *)playerID
                            channelID:(int)channelID
                          finishBlock:(ShowVideoAdFinishBlock)finishBlock{

}

- (BOOL)isChinese{
   // NSString *language = [AppUtils getCurrentLanguage];
    return YES;// [language hasPrefix:@"zh-Hans"];
}

#pragma mark - class init and singleton method
+ (AdManagerIOS *)sharedInstance {
    
    if (__instance == nil) {
        __instance = [[AdManagerIOS alloc] init];
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
        
    }
    return self;
}

- (void)dealloc {
    NSAssert(NO, @"Attempted to deallocate a singleton instance.");
}
@end
