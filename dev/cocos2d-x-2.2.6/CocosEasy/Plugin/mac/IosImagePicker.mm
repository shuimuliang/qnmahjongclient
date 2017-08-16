//
//  ImagePicker.m
//  Casino
//
//  Created by liulihua on 15/1/14.
//
//

#import "IosImagePicker.h"
#include "ImagePicker.h"

/*
IMPORTANT

The UIImagePickerController class supports portrait mode only. This class is intended to be used as-is and does not support subclassing. The view hierarchy for this class is private and must not be modified, with one exception. You can assign a custom view to the cameraOverlayView property and use that view to present additional information or manage the interactions between the camera interface and your code.
*/

void cocoseasy::ImagePicker::selectPic(int clipWidth){
    BOOL isIpad = (CCApplication::sharedApplication()->getTargetPlatform() == kTargetIpad);
    [[IosImagePicker sharedInstance] selectPhoto:isIpad finishBlock:^(NSString *filePath) {
        selectPicFinished([filePath UTF8String]);
    }cancelBlock:^(void){
        selectPicCanceled();
    }];
}

static IosImagePicker * __instance = nil;
static const CGFloat kImageMaxEdgeLen = 100.f;

@interface IosImagePicker ()
@property (nonatomic, copy) SelectFinishBlock finishBlock;
@property (nonatomic, copy) SelectCancelBlock cancelBlock;
@end

@implementation IosImagePicker
- (void)selectPhoto:(BOOL)isIpad
        finishBlock:(SelectFinishBlock) finishBlock
        cancelBlock:(SelectCancelBlock) cancelBlock {
    self.finishBlock = finishBlock;
    self.cancelBlock = cancelBlock;
   
}


- (void)dismissImagePicker {
//    [self.rootViewController dismissModalViewControllerAnimated:YES];
//    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];
}


- (NSString *)saveImageDataWithName:(NSString *)imageName data:(NSData *)imageData
{
    NSString *directoryDir = [NSString stringWithFormat:@"%@/Documents", NSHomeDirectory()];
    BOOL isDir = NO;
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL existed = [fileManager fileExistsAtPath:directoryDir isDirectory:&isDir];
    if ( !(isDir == YES && existed == YES) )
    {
        [fileManager createDirectoryAtPath:directoryDir withIntermediateDirectories:YES attributes:nil error:nil];
    }
    NSString* realPath = [NSString stringWithFormat:@"%@/%@",directoryDir,imageName];
    [fileManager createFileAtPath:realPath contents:imageData attributes:nil];
    return realPath;
}

#pragma mark - class init and singleton method
+ (IosImagePicker *)sharedInstance {
    
    if (__instance == nil) {
        __instance = [[IosImagePicker alloc] init];
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
        // init here
    }
    return self;
}

- (void)dealloc {
    NSAssert(NO, @"Attempted to deallocate a singleton instance.");
    [super dealloc];
}
@end
