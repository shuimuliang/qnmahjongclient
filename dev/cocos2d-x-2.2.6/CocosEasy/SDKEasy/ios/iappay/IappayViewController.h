//
//  IappayViewController.h
//  Casino
//
//  Created by liulihua on 15/6/13.
//
//

#import <UIKit/UIKit.h>
#import "APPSDKBase.h"
#import <Foundation/Foundation.h>

typedef void (^IappayPurchaseFinishBlock)(AppPurchaseErrorCode errCode, NSString* orderInfo);

@interface IappayViewController : UIViewController<UIWebViewDelegate>{
    UIWebView* _webView;
}

- (instancetype)initWithURL:(NSString *)payURL
                    payInfo:(NSString *)goodsInfo
                finishBlock:(IappayPurchaseFinishBlock)finishBlock;
@end
