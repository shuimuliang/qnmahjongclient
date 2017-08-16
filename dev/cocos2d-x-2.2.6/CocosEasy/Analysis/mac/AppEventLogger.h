//
//  AppEventLogger.h
//  client
//
//  Created by liulihua on 14-4-10.
//
//

#import <Foundation/Foundation.h>


@interface AppEventLogger : NSObject
+ (void)initEventLogger;

+(void) logEvent:(NSString*) eventId eventLabel:(NSString*)eventLabel;

/**
 *  标识某个页面访问的开始，请参见Example程序，在合适的位置调用。
 */
+(void) pageviewStartWithName:(NSString*) name;
/**
 *  标识某个页面访问的结束，与pageviewStartWithName配对使用，请参见Example程序，在合适的位置调用。
 */
+(void) pageviewEndWithName:(NSString*) name;


@end
