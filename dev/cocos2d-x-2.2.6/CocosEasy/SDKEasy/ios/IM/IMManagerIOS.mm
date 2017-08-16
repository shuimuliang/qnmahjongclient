//
//  AdManagerIOS.m
//  testAd
//
//  Created by liulihua on 15/1/19.
//  Copyright (c) 2015年 snowCat. All rights reserved.
//

#import "SDKManager.h"
#import "IMManagerIOS.h"
#import <RongIMLib/RongIMLib.h>


static IMManagerIOS * __instance = nil;
@interface IMManagerIOS()
@end


@implementation IMManagerIOS
- (void)configureAdWhenBootup{
 /* 
    [[NIMSDK sharedSDK] registerWithAppID:@"1e0849638e92f56e984534c6b5b9979d"
                                  cerName:@"ENTERPRISE"];
    [[[NIMSDK sharedSDK] mediaManager] addDelegate:self];
    [[[NIMSDK sharedSDK] chatManager] addDelegate:self];
  */
    
    
    [[RCIMClient sharedRCIMClient] initWithAppKey:@"e0x9wycfe00dq"];

    [[RCIMClient sharedRCIMClient] connectWithToken:@"kIkB3pW8LjfOiXJ8Fzkf3Q5WyHco0KwpCBjcsuEJ5L1ciZngWtChxkUAXjUP3pVfbCtUOddmGzKYh14j+Egu6A=="
                                            success:^(NSString *userId) {
                                                [self enterChatRoom:"111"];
//                                                NSLog(@"登陆成功。当前登录的用户ID：%@", userId);
                                            } error:^(RCConnectErrorCode status) {
                                                NSLog(@"登陆的错误码为:%d", status);
                                            } tokenIncorrect:^{
                                                //token过期或者不正确。
                                                //如果设置了token有效期并且token过期，请重新请求您的服务器获取新的token
                                                //如果没有设置token有效期却提示token错误，请检查您客户端和服务器的appkey是否匹配，还有检查您获取token的流程。
                                                NSLog(@"token错误");
                                            }];
}

- (NSURL *)recordTempFileURL
{
    if (!_recordTempFileURL) {
        _recordTempFileURL = [NSURL fileURLWithPath:[NSTemporaryDirectory() stringByAppendingPathComponent: @"tempAC.caf"]];
    }
    return _recordTempFileURL;
}

- (AVAudioRecorder *)recorder
{
    if (!_recorder) {
        NSDictionary *settings = @{AVFormatIDKey: @(kAudioFormatLinearPCM),
                                   AVSampleRateKey: @8000.00f,
                                   AVNumberOfChannelsKey: @1,
                                   AVLinearPCMBitDepthKey: @16,
                                   AVLinearPCMIsNonInterleaved: @NO,
                                   AVLinearPCMIsFloatKey: @NO,
                                   AVLinearPCMIsBigEndianKey: @NO};
        NSError *error;
        _recorder = [[AVAudioRecorder alloc] initWithURL:self.recordTempFileURL settings:settings error:&error];
    }
    return _recorder;
}

- (void)setIsRecording:(BOOL)isRecording
{
    _isRecording = isRecording;
}

- (void)beginRecord:(int)time{
    [self.recorder prepareToRecord];
    self.isRecording = [self.recorder record];
}

- (void)stopRecord{
    if (self.isRecording) {
        self.recordedData =[NSData dataWithContentsOfURL:self.recordTempFileURL];
        self.duration = self.recorder.currentTime;
        [self.recorder stop];
        
        
    }
    self.isRecording = NO;
}

- (void)cancelRecord{
//    [[[NIMSDK sharedSDK] mediaManager] cancelRecord];
}

- (void)beginPlay:(NSString*)filePath{
    NSLog(@"hk ---- path     %@",filePath);
//    [[[NIMSDK sharedSDK] mediaManager] play:filePath];
    
    
    
}

- (void)sendMsg:(NSString *)sessionID filePath:(NSString *)filePath{
//    //构造消息
//    NIMAudioObject *audioObject = [[NIMAudioObject alloc] initWithSourcePath:filePath];
//    NIMMessage *message        = [[NIMMessage alloc] init];
//    message.messageObject      = audioObject;
//    
//    //构造会话
//    NIMSession *session = [NIMSession session:sessionID type:NIMSessionTypeChatroom];
//    
//    //发送消息
//    [[NIMSDK sharedSDK].chatManager sendMessage:message toSession:session error:nil];
}



- (void)enterChatRoom:(const char*)chatRoomID{
   [[RCIMClient sharedRCIMClient] joinChatRoom:@"111" messageCount:-1 success:^(void){
       NSLog(@"chenggong");
    } error:^(RCErrorCode status){
        
    }];

    
}

- (void)loginIM:(const char*)neteaseID token:(const char*)token{
//    NSString * neteaseIDNS = neteaseID?[NSString stringWithUTF8String : neteaseID]:nil;
//    NSString * tokenNS = token?[NSString stringWithUTF8String : token]:nil;
//    
//    [[[NIMSDK sharedSDK] loginManager] login:neteaseIDNS
//                                       token:tokenNS
//                                  completion:^(NSError *error) {
//                                      if (error == nil)
//                                      {
//                                          NSLog(@"登录成功 %@",neteaseIDNS);
//                                      }
//                                  }];
    
    
}






#pragma mark - class init and singleton method
+ (IMManagerIOS *)sharedInstance {
    
    if (__instance == nil) {
        __instance = [[IMManagerIOS alloc] init];
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
