//
//  AppPushNotificationManager.m
//  zhajinhua
//
//  Created by Jianjun Wu on 11/14/12.
//
//
#include <AudioToolbox/AudioToolbox.h>
#import "AppPushNotificationManager.h"
#import "AppEventLogger.h"
#import "NSDate+Additions.h"

static AppPushNotificationManager * __instance = nil;

static void soundCompleted(SystemSoundID soundFileObject, void *clientData) {
  
}

@implementation AppPushNotificationManager

#pragma mark -
+ (AppPushNotificationManager *)sharedManager {
  
  if (__instance == nil) {
    __instance = [[AppPushNotificationManager alloc] init];
      
  }
  return __instance;
}

+ (id)alloc {
  
  if (__instance) {
    return __instance;
  }
  return [super alloc];
}

#pragma mark -
- (void)playNotificationSound:(NSDictionary *)apsDictionary
{
  
}


- (void)badgeApplicationIcon:(NSDictionary *)apsDictionary {
  
  id badge = [apsDictionary valueForKey:@"badge"];

  @try {
  }
  @catch (NSException *exception) {
  }
}

- (void)showNotificationAlert:(NSDictionary *)apsDictionary {
  
  
}

#pragma mark - create local notifications
- (void)scheduleLocalNotification:(NSDate*)date
                          message:(NSString *)msg
                   notificationID:(NSInteger)notificationID{
  
  }

- (void)cancelLocalNotification:(NSInteger)notificationID{
    NSArray* notificationList = [self getAllLocalNotifications];
    
}

- (void)cancelAllLocalNotification{
}

- (NSArray* )getAllLocalNotifications{
    return NULL;
}

#pragma mark - notification handling
- (void)handleLaunchOptions:(NSDictionary *)launchOptions {

}

- (void)handleRemoteNotificationWhenBootup:(NSDictionary *)userInfo {
  
  if (userInfo == nil) {
    return;
  }
  //NSDictionary *apsDictionary = [userInfo valueForKey:@"aps"];

}

@end
