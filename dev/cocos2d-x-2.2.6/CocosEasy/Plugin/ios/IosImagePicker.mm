//
//  ImagePicker.m
//  Casino
//
//  Created by liulihua on 15/1/14.
//
//

#import "IosImagePicker.h"
#import "PSActionSheet.h"
#import "UIImage+Resize.h"
#include "ImagePicker.h"
#import "AppUtils.h"

/*
IMPORTANT

The UIImagePickerController class supports portrait mode only. This class is intended to be used as-is and does not support subclassing. The view hierarchy for this class is private and must not be modified, with one exception. You can assign a custom view to the cameraOverlayView property and use that view to present additional information or manage the interactions between the camera interface and your code.
*/

void cocoseasy::ImagePicker::selectPic(int clipWidth){
    BOOL isIpad = (CCApplication::sharedApplication()->getTargetPlatform() == kTargetIpad);
    [[IosImagePicker sharedInstance] selectPhoto:isIpad clipWidth:clipWidth finishBlock:^(NSString *filePath) {
        selectPicFinished([filePath UTF8String]);
    }cancelBlock:^(void){
        selectPicCanceled();
    }];
}

static IosImagePicker * __instance = nil;
//static const CGFloat kImageMaxEdgeLen = 100.f;
static NSInteger kTempImageIndex = 1;

@interface IosImagePicker ()
@property (nonatomic, strong) UIImagePickerController *imagePicker;
@property (nonatomic, retain) UIPopoverController *popoverController;

@property (nonatomic, retain) UIViewController *rootViewController;
@property (nonatomic, copy) SelectFinishBlock finishBlock;
@property (nonatomic, copy) SelectCancelBlock cancelBlock;
@property (nonatomic, assign) NSInteger clipWidth;
@end

@implementation IosImagePicker
- (void)selectPhoto:(BOOL)isIpad
          clipWidth:(NSInteger)clipWidth
        finishBlock:(SelectFinishBlock) finishBlock
        cancelBlock:(SelectCancelBlock) cancelBlock {
    self.finishBlock = finishBlock;
    self.cancelBlock = cancelBlock;
    self.clipWidth = clipWidth;
    
    UIView *view = self.rootViewController.view;
    PSActionSheet *actionSheet = [PSActionSheet sheetWithTitle:(isIpad?APP_LOCALE(@"imagepicker.pic.select"):nil)];
    if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) {
        [actionSheet setCancelButtonWithTitle:APP_LOCALE(@"imagepicker.pic.photo") block:^{
            [self performSelector:@selector(showImagePickerForCamera) withObject:nil afterDelay:.1f];
        }];
    }
    if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeSavedPhotosAlbum]) {
        [actionSheet setCancelButtonWithTitle:APP_LOCALE(@"imagepicker.pic.select2") block:^{
            if (isIpad) {
                [self performSelector:@selector(showImagePickerForAlbumOnIpad:) withObject:view afterDelay:.1f];
            }else{
                [self performSelector:@selector(showImagePickerForAlbum) withObject:nil afterDelay:.1f];
            }
        }];
    }
    [actionSheet setCancelButtonWithTitle:APP_LOCALE(@"common.cancel") block:NULL];
    [actionSheet showInView:view];
}

- (void)showImagePickerForCamera {
    UIImagePickerController *picker = [self imagePickerController];
    picker.sourceType = UIImagePickerControllerSourceTypeCamera;

//    [self.rootViewController presentModalViewController:picker animated:YES];
    [self.rootViewController presentViewController:picker animated:YES completion:NULL];
}

- (void)showImagePickerForAlbum {
    UIImagePickerController *picker = [self imagePickerController];
    picker.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
//    [self.rootViewController presentModalViewController:picker animated:YES];
    [self.rootViewController presentViewController:picker animated:YES completion:NULL];

}

- (void)showImagePickerForAlbumOnIpad:(UIView *)view {
    
    UIImagePickerController *picker = [self imagePickerController];
    picker.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
    
    UIPopoverController *popover = [[UIPopoverController alloc] initWithContentViewController:picker];
    self.popoverController = popover;
    [self.popoverController presentPopoverFromRect:CGRectMake(0, 0, 300, 400) inView:view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
}
- (void)dismissImagePicker {
//    [self.rootViewController dismissModalViewControllerAnimated:YES];
    [self.rootViewController dismissViewControllerAnimated:YES completion:NULL];
//    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];
}

- (UIImagePickerController *)imagePickerController {
    if (self.imagePicker == nil) {
        self.imagePicker = [[UIImagePickerController alloc] init];
        self.imagePicker.allowsEditing = YES;
        self.imagePicker.delegate = self;
    }
    return self.imagePicker;
}

#pragma mark - delegate
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info {
    
    if (self.popoverController) {
        [self.popoverController dismissPopoverAnimated:YES];
    }
    
    UIImage *editImage = [info objectForKey: UIImagePickerControllerOriginalImage];
    float compress = 0.5f;
    
    if (self.clipWidth > 0) {
        editImage = [info objectForKey:UIImagePickerControllerEditedImage];
        CGFloat minEdge = MIN(editImage.size.width, editImage.size.height);
        if (self.clipWidth < minEdge) {
            editImage = [editImage resizedImageWithContentMode:UIViewContentModeScaleAspectFit
                                                        bounds:CGSizeMake(self.clipWidth, self.clipWidth)
                                          interpolationQuality:kCGInterpolationMedium];
        } else if (editImage.size.width != editImage.size.height) {
            editImage = [editImage croppedImage:CGRectMake(0.f, 0.f, minEdge, minEdge)];
        }
        compress = 1.0f;
    }
    NSData *imageData = UIImageJPEGRepresentation(editImage, compress);
    
    NSString *fileName = [NSString stringWithFormat:@"defaultAvatar%ld.png",(long)kTempImageIndex];
    kTempImageIndex = kTempImageIndex + 1;
    NSString* realPath = [self saveImageDataWithName:fileName data:imageData];
    
    if (imageData && self.finishBlock){
        self.finishBlock(realPath);
    }
    [self dismissImagePicker];
    self.imagePicker = nil;
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
    //is file existed
    NSString* realPath = [NSString stringWithFormat:@"%@/%@",directoryDir,imageName];
    if([fileManager fileExistsAtPath:realPath]){
        [fileManager removeItemAtPath:realPath error:nil];
    }
    [fileManager createFileAtPath:realPath contents:imageData attributes:nil];
    return realPath;
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker {
    self.imagePicker = nil;
    if (self.cancelBlock) self.cancelBlock();
    [self dismissImagePicker];
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
        self.rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
    }
    return self;
}

- (void)dealloc {
    NSAssert(NO, @"Attempted to deallocate a singleton instance.");
    [super dealloc];
}
@end
