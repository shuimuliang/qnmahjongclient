//
//  AdManagerIOS.h
//  testAd
//
//  Created by liulihua on 15/1/19.
//  Copyright (c) 2015年 snowCat. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


typedef enum {
    kVedioAdType_none = 0,
    kVedioAdType_youmi = 1,
    kVedioAdType_vungle = 2
}kVedioAdType;

typedef enum {
    kAdPlayStatus_none = 0,
    kAdPlayStatus_succ = 1,
    kAdPlayStatus_failed = 2,
    kAdPlayStatus_half = 3,
}kAdPlayStatus;

typedef void (^ShowVideoAdFinishBlock)(kAdPlayStatus status);
typedef void (^CheckAvalibleFinishBlock)(BOOL isAvalible);


//@class GADInterstitial;
@interface AdManagerIOS : NSObject
@property (nonatomic, copy) ShowVideoAdFinishBlock showFinishBlock;
@property (nonatomic, copy) CheckAvalibleFinishBlock checkAvalibleBlock;

//chukong
@property (nonatomic, assign) BOOL isChuKongSpotEnabled;
// google admob
//@property (nonatomic, strong) GADInterstitial *gaInterstitial;



+ (AdManagerIOS *)sharedInstance;
- (void)configureAdWhenBootup;

- (BOOL)isVideoAvalible;
//展示视频广告
- (void)showVideoAdWithViewController:(UIViewController *)viewController
                             playerID:(NSString *)playerID
                            channelID:(int)channelID
                          finishBlock:(ShowVideoAdFinishBlock)finishBlock;
@end
