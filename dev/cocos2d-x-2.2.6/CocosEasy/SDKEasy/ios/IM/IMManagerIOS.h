//
//  AdManagerIOS.h
//  testAd
//
//  Created by liulihua on 15/1/19.
//  Copyright (c) 2015年 snowCat. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>


//@class GADInterstitial;
@interface IMManagerIOS : NSObject
@property (nonatomic, copy) NSString * mChatRoomID;
@property (strong, nonatomic)NSURL *recordTempFileURL;
@property (strong, nonatomic)AVAudioRecorder *recorder;
@property (nonatomic)BOOL isRecording;
@property (strong, nonatomic)NSData *recordedData;
@property (nonatomic)NSTimeInterval duration;


+ (IMManagerIOS *)sharedInstance;
- (void)configureAdWhenBootup;

- (void)beginRecord:(int)time;
- (void)stopRecord;
- (void)cancelRecord;

- (void)sendMsg:(NSString*)sessionID filePath:(NSString*)filePath;

- (void)enterChatRoom:(const char*)chatRoomID;
- (void)loginIM:(const char*)neteaseID token:(const char*)token;

- (void)beginPlay:(NSString*)filePath;

@end
