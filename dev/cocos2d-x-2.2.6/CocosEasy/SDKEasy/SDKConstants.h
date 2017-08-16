//
//  SDKConstants.h
//  client
//
//  Created by liulihua on 14-8-12.
//
//

#ifndef client_SDKConstants_h
#define client_SDKConstants_h


enum SDK_CHANNEL_DEF
{
    SDK_CHANNEL_NONE                  = 0,
    //ios
    SDK_CHANNEL_APP_STORE             = 104,// appstore 中文版
    SDK_CHANNEL_APP_STORE_PRO         = 101,// appstore 中文版 pro
    SDK_CHANNEL_HAIMA_IOS             = 102,//  中文版 海马
    SDK_CHANNEL_XY_IOS                = 103,//  iOS版 XY助手
    SDK_CHANNEL_APP_STORE_ZJH         = 100,//  iOS版 中文版 诈金花
    SDK_CHANNEL_APP_STORE_ZJH2        = 101,//  iOS版 中文版 土豪诈金花
    SDK_CHANNEL_APP_STORE_SLOTS       = 105,//  iOS版 中文版 老虎机
    SDK_CHANNEL_XY_ZJH_IOS            = 106,//  iOS版 xy助手 诈金花
    SDK_CHANNEL_HAIMA_ZJH_IOS         = 107,//  iOS版 海马 诈金花
    SDK_CHANNEL_APP_STORE_21          = 108,//  iOS版 21点
    SDK_CHANNEL_APP_STORE_DWC         = 109,//  iOS版 疯狂动物城
    SDK_CHANNEL_APP_STORE_SANZHANG    = 110,//  iOS版 欢乐斗赢三张

    SDK_CHANNEL_APP_STORE_ZJHNEW      = 125,//  iOS版 扎金花新

    SDK_CHANNEL_APP_STORE_ENG         = 151,// appstroe 英文版
    SDK_CHANNEL_IPHONE_CAKE           = 152,// iphonecake 版本
    SDK_CHANNEL_APP_CAKE              = 153,// appcake 版本

    SDK_CHANNEL_APP_STORE_TRADITIONAL = 11000,// appstroe 繁体版
    SDK_CHANNEL_APP_STORE_MALAYSIA    = 11100,// AppStore 马来西亚 版本
    SDK_CHANNEL_APP_STORE_ARAB        = 11200,// AppStore 阿拉伯 版本

    //android
    SDK_CHANNEL_GOOGLE_PLAY           = 21000,//googleplay
    SDK_CHANNEL_MALAI                 = 201,//马来西亚
    SDK_CHANNEL_ARABIC                = 202,//阿拉伯

    SDK_CHANNEL_WEICO                 = 230,//weico自己渠道
    SDK_CHANNEL_DIANXIN_GAME          = 231,//电信爱游戏
    SDK_CHANNEL_HAIMA_ANDROID         = 232,//android  海马
    SDK_CHANNEL_XY_ANDROID            = 233,//android XY助手
    SDK_CHANNEL_WEB_ANDROID           = 234,//百度 google web搜索
    SDK_CHANNEL_UC_ANDROID            = 235,//UC android

    SDK_CHANNEL_ZJH_ORIGIN_ANDROID    = 200,//诈金花 官网版本 android
    SDK_CHANNEL_ZJH_XY_ANDROID        = 21001,//诈金花 xy助手  android

    SDK_CHANNEL_SLOTS_ORIGIN_ANDROID  = 22000,//  老虎机 官网版本 android
    SDK_CHANNEL_21_ORIGIN_ANDROID     = 23000,//  21点 官网版本 android


    //windows phone
    SDK_CHANNEL_WP_WFK                = 300,
    SDK_CHANNEL_WP_247_SANZHANG       = 301,

    //windows
    SDK_CHANNEL_WIN                   = 400,
    //mac
    SDK_CHANNEL_MAC                   = 500,

    SDK_CHANNEL_DEBUG                 = 600,

    SDK_CHANNEL_PREVIEW               = 700,//渠道审核版本
};

typedef enum {
    kAppThirdLoginTypeNone            = 0,//邮箱登陆（包含 邮箱+游客）  默认方式
    kAppThirdLoginTypeFacebook        = 1,//FB登陆 （包含  FB+游客）
    kAppThirdLoginTypeHaiMa           = 2,//海马登陆
    kAppThirdLoginTypeXY              = 3,//XY助手
} AppThirdLoginType;

//sdk related constants
extern const char* kAppEventSDKInitFailed;
extern const char* kAppEventSDKLoginFailed;
extern const char* kAppEventSDKPayFailed;

extern const char* googlePublicKey;
extern const char* googlePublicKey_USA;
extern const char* googlePublicKey_MaLai;
extern const char* googlePublicKey_Arabic;

extern const char* paymentWallPrivate;
extern const char* paymentWallPublic;

extern const char* slotBuglyAndroidID;
extern const char* zjhBuglyAndroidID;
#endif
