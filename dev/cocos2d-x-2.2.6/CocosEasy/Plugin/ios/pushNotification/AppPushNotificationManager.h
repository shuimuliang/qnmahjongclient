//
//  AppPushNotificationManager.h
//  zhajinhua
//
//  Created by Jianjun Wu on 11/14/12.
//
//

#import <Foundation/Foundation.h>

@interface AppPushNotificationManager : NSObject

+ (AppPushNotificationManager *)sharedManager;

- (void)playNotificationSound:(NSDictionary *)apsDictionary;
- (void)badgeApplicationIcon:(NSDictionary *)apsDictionary;
- (void)showNotificationAlert:(NSDictionary *)apsDictionary;

- (void)handleLaunchOptions:(NSDictionary *)launchOptions;

- (void)scheduleLocalNotification:(NSDate*)date
                          message:(NSString *)msg
                   notificationID:(NSInteger)notificationID;
- (void)cancelLocalNotification:(NSInteger)notificationID;

- (void)cancelAllLocalNotification;
- (NSArray *)getAllLocalNotifications;

@end
