//
//  APPSDKBase.h
//  client
//
//  Created by liulihua on 14-8-13.
//
//

#import <Foundation/Foundation.h>

@interface APPSDKBase : NSObject
- (void)logSdkEvent:(const char *)eventID errCode:(int)errCode;
@end
