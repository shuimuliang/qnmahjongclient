/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#import <UIKit/UIKit.h>
#import "AppController.h"
#import "cocos2d.h"
#import "EAGLView.h"
#import "AppDelegate.h"
#import "SDKManager.h"
#import "Analysis.h"
#import "RootViewController.h"
#import "PSAlertView.h"
#import "UMessage.h"
#import "AppFlyerManager.h"
#import "JSONKit.h"

#if CASINO_THIRD_FACEBOOK
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#endif

#if CASINO_PAYPAL
#import "PayPalManager.h"
#endif

#if CASINO_THIRD_XY
#import "AppXYManager.h"
#endif
#import "AdTrakingManager.h"

#import "CocosEasyPlugin.h"
#include "Config.h"

#if CASINO_THIRD_HM
#import "AppHMManager.h"
#endif


#define UMSYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)
#define _IPHONE80_ 80000

@implementation AppController

#pragma mark -
#pragma mark Application lifecycle


// cocos2d application instance
static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGBA8
                                     depthFormat: GL_DEPTH24_STENCIL8_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples: 0 ];

    [__glView setMultipleTouchEnabled:FALSE];
    // Use RootViewController manage EAGLView
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    
    [window makeKeyAndVisible];
// init sdk
    CSDKManager::getInstance()->initPlatform(0);
// init talkingdata
    CAnalysis::getInstance()->initAnalysis();
        
    [[UIApplication sharedApplication] setStatusBarHidden: YES];
  
#ifdef CASINO_WXAPPID
    // 向微信注册APPid
    NSLog(@"向微信注册APPid: %@", CASINO_WXAPPID);
    [WXApi registerApp:CASINO_WXAPPID];
#endif

    cocos2d::CCApplication::sharedApplication()->run();
    
    [self startUMessageWithOptions:launchOptions];

#if CASINO_THIRD_FACEBOOK
    [[FBSDKApplicationDelegate sharedInstance] application:application
                             didFinishLaunchingWithOptions:launchOptions];
#endif
    
#if CASINO_PAYPAL
    [[PayPalManager sharedInstance] configurePayPalWhenBootup];
#endif
    [AppFlyerManager initAppFlyer];

    [AdTrakingManager initAdTraking];
 
    return YES;
}

- (void)startUMessageWithOptions:(NSDictionary *)launchOptions{
    //set AppKey and AppSecret
#ifdef CASINO_UMENG_KEY
    [UMessage startWithAppkey:CASINO_UMENG_KEY launchOptions:launchOptions];
    
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= _IPHONE80_
    if(UMSYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"8.0"))
    {
        //register remoteNotification types （iOS 8.0及其以上版本）
        UIMutableUserNotificationAction *action1 = [[UIMutableUserNotificationAction alloc] init];
        action1.identifier = @"action1_identifier";
        action1.title=@"接受";
        action1.activationMode = UIUserNotificationActivationModeForeground;//当点击的时候启动程序
        
        UIMutableUserNotificationAction *action2 = [[UIMutableUserNotificationAction alloc] init];  //第二按钮
        action2.identifier = @"action2_identifier";
        action2.title=@"拒绝";
        action2.activationMode = UIUserNotificationActivationModeBackground;//当点击的时候不启动程序，在后台处理
        action2.authenticationRequired = YES;//需要解锁才能处理，如果action.activationMode = UIUserNotificationActivationModeForeground;则这个属性被忽略；
        action2.destructive = YES;
        
        UIMutableUserNotificationCategory *categorys = [[UIMutableUserNotificationCategory alloc] init];
        categorys.identifier = @"category1";//这组动作的唯一标示
        [categorys setActions:@[action1,action2] forContext:(UIUserNotificationActionContextDefault)];
        
        UIUserNotificationSettings *userSettings = [UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeBadge|UIUserNotificationTypeSound|UIUserNotificationTypeAlert
                                                                                     categories:[NSSet setWithObject:categorys]];
        [UMessage registerRemoteNotificationAndUserNotificationSettings:userSettings];
        
    } else{
        //register remoteNotification types (iOS 8.0以下)
        [UMessage registerForRemoteNotificationTypes:UIRemoteNotificationTypeBadge
         |UIRemoteNotificationTypeSound
         |UIRemoteNotificationTypeAlert];
    }
#else
    
    //register remoteNotification types (iOS 8.0以下)
    [UMessage registerForRemoteNotificationTypes:UIRemoteNotificationTypeBadge
     |UIRemoteNotificationTypeSound
     |UIRemoteNotificationTypeAlert];
    
#endif
#if DEBUG
    //for log
    [UMessage setLogEnabled:YES];
#endif
    
    [UMessage setAutoAlert:NO];
//    [UMessage setBadgeClear:NO];
    NSInteger platFrom = CSDKManager::getInstance()->getChannelID();
    [UMessage setChannel:[NSString stringWithFormat:@"%ld",(long)platFrom]];
#endif   //end CASINO_UMENG_KEY
}

//独立客户端回调函数
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
    
//#ifdef APP_PP_SDK
//    [[AppPPManager sharedInstance] setAlixPayResult];
//#endif
#ifdef CASINO_THIRD_XY
    [[AppXYManager sharedInstance] XYHandleOpenURL:url];
#endif
    
#ifdef CASINO_WXAPPID
    return [WXApi handleOpenURL:url delegate:self];
#endif
    
    return YES;

}

- (BOOL)application:(UIApplication *)application
            openURL:(NSURL *)url
  sourceApplication:(NSString *)sourceApplication
         annotation:(id)annotation {
#ifdef CASINO_THIRD_XY
    [[AppXYManager sharedInstance] XYHandleOpenURL:url];
#endif
    
#ifdef CASINO_WXAPPID
    return [WXApi handleOpenURL:url delegate:self];
#endif
    
#ifdef CASINO_THIRD_FACEBOOK
    return [[FBSDKApplicationDelegate sharedInstance] application:application
                                                          openURL:url
                                                sourceApplication:sourceApplication
                                                       annotation:annotation];
#else
    return YES;
#endif
}

-(void) onReq:(BaseReq*)req {
    
}

/* errcode
WXSuccess           = 0,    **< 成功
WXErrCodeCommon     = -1,   **< 普通错误类型
WXErrCodeUserCancel = -2,   **< 用户点击取消并返回
WXErrCodeSentFail   = -3,   **< 发送失败
WXErrCodeAuthDeny   = -4,   **< 授权失败
WXErrCodeUnsupport  = -5,   **< 微信不支持
*/

-(void) onResp:(BaseResp*)resp {
    if ([resp isKindOfClass:[SendAuthResp class]]) {
        SendAuthResp *authResp = (SendAuthResp *)resp;
        NSString* curState = authResp.state;
        string state = CSDKManager::getInstance()->getWchatState();
        NSString* mState = [NSString stringWithUTF8String:state.c_str()];
        
        if ([curState isEqualToString:mState]) {
            if (resp.errCode == WXSuccess) {
                NSMutableDictionary *dic = [NSMutableDictionary dictionaryWithCapacity:1];
                [dic setObject:authResp.code forKey:@"tokenString"];
                NSString *info = [dic JSONString];
                CSDKManager::getInstance()->thirdLoginCallBack(0,[info UTF8String]);
            }else{
                CSDKManager::getInstance()->thirdLoginCallBack(1,"");
            }
        }
    }else if ([resp isKindOfClass:[SendMessageToWXResp class]]){
        if (resp.errCode == WXSuccess) {
            CSDKManager::getInstance()->shareCallBack(true);
        }
    }
}

- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    [AppFlyerManager trackAppLaunch];
#ifdef CASINO_THIRD_FACEBOOK
    [FBSDKAppEvents activateApp];
#endif

    cocos2d::CCDirector::sharedDirector()->resume();
}

- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window{
#ifdef CASINO_THIRD_HM
    return [APP_HM_MANAGER application:application supportedInterfaceOrientationsForWindow:window];
#endif
    return UIInterfaceOrientationMaskAll;
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    NSString *deviceTokenStr = [[[[deviceToken description] stringByReplacingOccurrencesOfString: @"<" withString: @""]
                                 stringByReplacingOccurrencesOfString: @">" withString: @""]
                                stringByReplacingOccurrencesOfString: @" " withString: @""];
    CSDKManager::getInstance()->setDeviceToken([deviceTokenStr UTF8String]);
#if DEBUG
    NSLog(@"%@",deviceTokenStr);
#endif
    [UMessage registerDeviceToken:deviceToken];
}
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    [UMessage didReceiveRemoteNotification:userInfo];
}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
}

- (void)dealloc {
    [super dealloc];
}


@end

