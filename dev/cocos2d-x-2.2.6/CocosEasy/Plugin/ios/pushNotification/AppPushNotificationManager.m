//
//  AppPushNotificationManager.m
//  zhajinhua
//
//  Created by Jianjun Wu on 11/14/12.
//
//
#include <AudioToolbox/AudioToolbox.h>
#import "AppPushNotificationManager.h"
#import "PSAlertView.h"
#import "AppEventLogger.h"
#import "NSDate+Additions.h"

static AppPushNotificationManager * __instance = nil;

static void soundCompleted(SystemSoundID soundFileObject, void *clientData) {
  // Clean up.
  if (soundFileObject != kSystemSoundID_Vibrate) {
    AudioServicesDisposeSystemSoundID(soundFileObject);
  }
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
  // App could implement its own preferences so the user could specify if they want sounds or alerts.
  // if (userEnabledSounds)
  
  NSString *soundName = (NSString *)[apsDictionary valueForKey:(id)@"sound"];
  if (soundName != nil) {
    SystemSoundID soundFileObject   = kSystemSoundID_Vibrate;
    CFURLRef soundFileURLRef        = NULL;
    
    if ([soundName compare:@"default"] != NSOrderedSame) {
      // Get the main bundle for the app.
      CFBundleRef mainBundle = CFBundleGetMainBundle();
      
      // Get the URL to the sound file to play. The sound property's value is the full filename including the extension.
      soundFileURLRef = CFBundleCopyResourceURL(mainBundle,
                                                (__bridge CFStringRef)soundName,
                                                NULL,
                                                NULL);
      
      // Create a system sound object representing the sound file.
      AudioServicesCreateSystemSoundID(soundFileURLRef,
                                       &soundFileObject);
      
      CFRelease(soundFileURLRef);
    }
    
    // Register a function to be called when the sound is done playing.
    AudioServicesAddSystemSoundCompletion(soundFileObject, NULL, NULL, soundCompleted, NULL);
    
    // Play the sound.
    AudioServicesPlaySystemSound(soundFileObject);
  }
}


- (void)badgeApplicationIcon:(NSDictionary *)apsDictionary {
  
  id badge = [apsDictionary valueForKey:@"badge"];

  @try {
    [UIApplication sharedApplication].applicationIconBadgeNumber = [badge integerValue];
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
  
  [[UIApplication sharedApplication] cancelAllLocalNotifications];
  UILocalNotification * localNotif = [[UILocalNotification alloc] init];
    [localNotif setAlertBody:msg];
    //可以在这里修改 本地notification的时间
    [localNotif setFireDate:date];
    
    localNotif.repeatInterval = kCFCalendarUnitMonth;
    localNotif.soundName = UILocalNotificationDefaultSoundName;
    NSMutableDictionary *userInfo = [[NSMutableDictionary alloc] initWithCapacity:1];
    [userInfo setObject:[NSNumber numberWithInteger:notificationID] forKey:@"notificationID"];
    localNotif.userInfo = userInfo;
    
    [[UIApplication sharedApplication] scheduleLocalNotification:localNotif];
}

- (void)cancelLocalNotification:(NSInteger)notificationID{
    NSArray* notificationList = [self getAllLocalNotifications];
    for (UILocalNotification *notification in notificationList) {
        if(notification.userInfo == NULL) continue;
        NSNumber *numberID = [notification.userInfo objectForKey:@"notificationID"];
        if(numberID == NULL) continue;
        if(notificationID == [numberID integerValue]){
            [[UIApplication sharedApplication] cancelLocalNotification:notification];
        }
    }
}

- (void)cancelAllLocalNotification{
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
}

- (NSArray* )getAllLocalNotifications{
   NSArray* notifications =  [[UIApplication sharedApplication] scheduledLocalNotifications];
#if DEBUG
    NSLog(@"all notifications : %@",notifications);
#endif
    return notifications;
}

#pragma mark - notification handling
- (void)handleLaunchOptions:(NSDictionary *)launchOptions {

  UILocalNotification *localNotif = [launchOptions objectForKey:UIApplicationLaunchOptionsLocalNotificationKey];
  [self handleLocalNotificationWhenBootup:localNotif];
  NSDictionary *remoteUserInfo = [launchOptions objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey];
  [self handleRemoteNotificationWhenBootup:remoteUserInfo];
  
//  NSString * source = [launchOptions objectForKey:UIApplicationLaunchOptionsSourceApplicationKey];
  [UIApplication sharedApplication].applicationIconBadgeNumber = 0;
  [[UIApplication sharedApplication] cancelAllLocalNotifications];
}

- (void)handleLocalNotificationWhenBootup:(UILocalNotification *)localNotif {
  
  if (localNotif == nil) {
    return;
  }
}

- (void)handleRemoteNotificationWhenBootup:(NSDictionary *)userInfo {
  
  if (userInfo == nil) {
    return;
  }
  //NSDictionary *apsDictionary = [userInfo valueForKey:@"aps"];

}

@end
