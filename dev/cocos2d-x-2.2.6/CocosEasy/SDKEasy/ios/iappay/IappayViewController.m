//
//  IappayViewController.m
//  Casino
//
//  Created by liulihua on 15/6/13.
//
//

#import "IappayViewController.h"
#import "PSAlertView.h"
#import "JSONKit.h"
#import "AppUtils.h"
#import "WebViewJsBridge.h"

@interface IappayViewController ()
@property (nonatomic) WebViewJsBridge* bridge;
@property (nonatomic, copy) IappayPurchaseFinishBlock finishBlock;
@property (nonatomic, copy) NSString * goodsInfo;
@property (nonatomic, copy) NSString * payURL;
@property (nonatomic, assign) BOOL process;
@end

@implementation IappayViewController

- (instancetype)initWithURL:(NSString *)payURL payInfo:(NSString *)goodsInfo finishBlock:(IappayPurchaseFinishBlock)finishBlock{
    self = [super init];
    if (self) {
        // init here
        self.payURL = payURL;
        self.goodsInfo = goodsInfo;
        self.finishBlock = finishBlock;
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.view.backgroundColor = [UIColor whiteColor];
    
//    [ self.navigationItem setTitle:APP_LOCALE(@"iapppay.recharge.title")];
//    self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:APP_LOCALE(@"common.cancel") style:UIBarButtonItemStylePlain
//                                                                                                               target:self
//                                                                                                               action:@selector(userDidCancelPayment)];
//
    
//    self.navigationItem
    self.navigationController.navigationBar.hidden = true;
    
    //create web view
    _webView = [[UIWebView alloc] initWithFrame:self.view.bounds];
    [self.view addSubview:_webView];
    _webView.backgroundColor = [UIColor clearColor];
    _webView.scalesPageToFit = YES;
    //设置oc和js的桥接
    _bridge = [WebViewJsBridge bridgeForWebView:_webView webViewDelegate:self];
    
    
//    NSString* htmlPath = [[NSBundle mainBundle] pathForResource:@"test" ofType:@"html"];
//    NSString* appHtml = [NSString stringWithContentsOfFile:htmlPath encoding:NSUTF8StringEncoding error:nil];
//    NSURL *baseURL = [NSURL fileURLWithPath:htmlPath];
//    [_webView loadHTMLString:appHtml baseURL:baseURL];
    
    //load web view
    NSURL *url =[[NSURL alloc] initWithString:self.payURL];
    NSURLRequest *request =  [[NSURLRequest alloc] initWithURL:url cachePolicy:NSURLRequestReloadIgnoringLocalCacheData timeoutInterval:15];
    [_webView loadRequest:request];
}


- (void)userDidCancelPayment {
    if (self.finishBlock) {
        self.finishBlock(kAppPurchaseErrorUserCanceled,self.goodsInfo);
    }
    [self dismissViewControllerAnimated:YES completion:nil];
}
- (NSString *)stringByDecodingURLFormat:(NSString *)str
{
    NSString *result = [str stringByReplacingOccurrencesOfString:@"+" withString:@" "];
    result = [result stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    return result;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (void)payWithGoodInfo:(NSString *)goodInfo {
    
    if (self.process) return ;
    self.process = true;
    
    
    NSDictionary *dataDic = [goodInfo objectFromJSONString];
    NSString * playerID = [dataDic objectForKey:@"playerID"];
    NSString * iappayID = [dataDic objectForKey:@"appStoreProductId"];
    NSString * platform = [dataDic objectForKey:@"channel"];

    
    NSMutableDictionary *dic = [[NSMutableDictionary alloc] init];
    [dic setValue:iappayID forKey:@"wares_id"];
    [dic setValue:playerID forKey:@"player_id"];
    [dic setValue:platform forKey:@"channel"];
    
    NSString *str =     [dic JSONString];
    NSLog(@"xxxxxxxx    : %@",str);
    
    [_bridge excuteJSWithObj:@"casinoIpayH5" function:[NSString stringWithFormat:@"pay('%@')",str]];
}

- (void)webViewDidFinishLoad:(UIWebView *)webView{
    if (webView != _webView) { return; }
//    [self payWithGoodInfo:self.goodsInfo];
}

- (void)iappayFailed{
//    NSLog(@"iappayFailed: %@",resultJsonStr);
    if (self.finishBlock) {
        self.finishBlock(kAppPurchaseErrorPaymentFailed,@"");
    }
    [self dismissViewControllerAnimated:YES completion:nil];

}


/*
 {
 OrderStatus = 0;
 RetCode = 0;
 TransId = 32011506241819259933;
 }
 
 {"transaction_id" :"1111111232432423432423"}

 非必填	0:支付成功
 1:支付失败
 2:待支付
 3:正在处理
 4:系统异常
 当客户端未返回该数据时，请调用服务端查询结果确认订单状态。
 
 返回码，
 0：表示正常通知。
 -1：http请求异常
 
 
 kAppPurchaseErrorNone = 0,
 kAppPurchaseErrorInProgressing,  //正在处理中
 kAppPurchaseErrorPurchaseNotAllowed,  // 用户没有购买权限
 kAppPurchaseErrorProductNotExit,
 kAppPurchaseErrorPaymentFailed,
 kAppPurchaseErrorProductIDRequired,
 kAppPurchaseErrorUserCanceled,
 kAppPurchaseErrorNetError
 */
- (void)iappayFinished:(NSArray *)resultArray{
//    NSLog(@"iappayFinished: %@",resultJsonStr);
   NSDictionary*dic = [resultArray objectAtIndex:0];
    
    NSInteger orderStatus = [[dic objectForKey:@"OrderStatus"] integerValue];
    NSInteger retCode = [[dic objectForKey:@"RetCode"] integerValue];
    NSString* transID = [dic objectForKey:@"TransId"];
    transID = transID ? transID : @"";
    //check retCode
    if (retCode != 0){
        if (self.finishBlock) {
            self.finishBlock(kAppPurchaseErrorNetError,self.goodsInfo);
            return;
        }
    }
    //check orderStatus
    AppPurchaseErrorCode errorCode = kAppPurchaseErrorNone;
//    if(orderStatus == 1 || orderStatus == 2 || orderStatus == 4  ){
//        errorCode = kAppPurchaseErrorPaymentFailed;
//    }else if(orderStatus == 3){
//        errorCode = kAppPurchaseErrorInProgressing;
//    }
    if (self.finishBlock) {
        self.finishBlock(errorCode,transID);
    }
    [self dismissViewControllerAnimated:YES completion:nil];

}

#pragma mark - Private Method
+ (void)openURL:(NSURL *)object complete:(void(^)(BOOL))complete
{
    UIApplication *application = nil;
    application = [UIApplication sharedApplication];
    SEL selector = @selector(openURL:options:completionHandler:);
    if ([UIApplication instancesRespondToSelector:selector])
    {
#ifdef __IPHONE_10_0
        [application openURL:object
                     options:[NSDictionary dictionary]
           completionHandler:complete];
#else
        if (complete) {
            complete([application openURL:object]);
        }
#endif
    }
    else
    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        if (complete) {
            complete([application openURL:object]);
        }
#pragma clang diagnostic pop
    }
}

- (BOOL)isOpenAppSpecialURLValue:(NSString *)string
{
    if ([string hasPrefix:@"http://"]) {
        return NO;
    } else if ([string hasPrefix:@"https://"]) {
        return NO;
    }
    return YES;
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType{
    NSLog(@"request.URL.schemerequest.URL.scheme : %@",request.URL.scheme);
    NSLog(@"absoluteString : %@",request.URL.absoluteString);

//    if ([request.URL.scheme isEqualToString:@"weixin"]){
//        NSURL * myURL_APP_A = request.URL;
//        if ([[UIApplication sharedApplication] canOpenURL:myURL_APP_A]) {
//            NSLog(@"canOpenURL");
//            dispatch_async(dispatch_get_main_queue(), ^{
//                [[UIApplication sharedApplication] openURL:myURL_APP_A];
//            });
//        }
//    }
    NSString *absoluteString = request.URL.absoluteString;
    if ([self isOpenAppSpecialURLValue:absoluteString]) {
        //空白地址就直接返回不执行加载
        if ([absoluteString hasPrefix:@"about:blank"]) {
            return NO;
        }
        
        //非http和https开头的链接就使用OpenURL方法打开
        [[self class] openURL:request.URL complete:^(BOOL status) {
//            if (self.openComplete) {
//                self.openComplete(absoluteString, status);
//            }
        }];
        return NO;
    }
    
    //将代理传递到下一个代理对象
//    if (self.webViewDelegate) {
//        return [self.webViewDelegate webView:self
//                  shouldStartLoadWithRequest:request
//                              navigationType:navigationType];
//    } else {
//        return YES;
//    }
    return  YES;
}


/*

#pragma mark webview delegate
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType{
    
    if (webView != _webView) { return YES; }
    
    
    
    NSString *urlString = [[request URL] absoluteString];
    NSRange range = [urlString rangeOfString:@"iappay://"];
    if(range.length!=0){
        NSString *orderInfo = [self stringByDecodingURLFormat:[urlString substringFromIndex:(range.location+range.length)]];
        NSLog(@"%@",orderInfo);
        
        NSInteger errorCode = 200;
        NSString *orderSerial = @"12345678";
        if (self.finishBlock) {
            self.finishBlock(errorCode,orderInfo);
        }
        [self dismissViewControllerAnimated:YES completion:nil];
        return NO;
    }
    return YES;
}


- (void)webViewDidStartLoad:(UIWebView *)webView{
    
}

- (void)webViewDidFinishLoad:(UIWebView *)webView{
    
    if (webView != _webView) { return; }
    
    NSString *jsStr = [NSString stringWithFormat:@"casinoIpayH5.pay('%@')",self.goodsInfo];
    NSString *jsRet = [_webView stringByEvaluatingJavaScriptFromString:jsStr];
    

//    NSString *jsCode = [NSString stringWithFormat:@"alert(1);"];
//    [_webView stringByEvaluatingJavaScriptFromString:jsCode];
}
- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error{
    NSString *str = [NSString stringWithFormat:@"error : %ld",(long)error.code];
    [PSAlertView alertWithTitle:@"请求失败" message:str];
}
 */


/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (void)relayoutViews{
//    [UIView animateWithDuration:0.5 animations:^(void){
        _webView.frame = self.view.frame;
//    }];
}
- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation{
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];
    // do something after rotation
    [self relayoutViews];
}

// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (UIInterfaceOrientationMask) supportedInterfaceOrientations{
    return UIInterfaceOrientationMaskLandscape;
}


- (BOOL) shouldAutorotate {
    return YES;
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

@end
