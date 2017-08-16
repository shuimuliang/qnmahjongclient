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
static IosImagePicker * __instance = nil;
static const CGFloat kImageMaxEdgeLen = 640.f;

@interface IosImagePicker ()
@property (nonatomic, strong) UIImagePickerController *imagePicker;
@property (nonatomic, retain) UIPopoverController *popoverController;

@property (nonatomic, retain) UIViewController *rootViewController;
@property (nonatomic, copy) SelectFinishBlock finishBlock;
@property (nonatomic, copy) SelectCancelBlock cancelBlock;
@end

@implementation IosImagePicker
- (void)selectPhoto:(BOOL)isIpad
        finishBlock:(SelectFinishBlock) finishBlock
        cancelBlock:(SelectCancelBlock) cancelBlock {
    self.finishBlock = finishBlock;
    self.cancelBlock = cancelBlock;
    
    UIView *view = self.rootViewController.view;
    PSActionSheet *actionSheet = [PSActionSheet sheetWithTitle:(isIpad?@"Set a picture":nil)];
    if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) {
        [actionSheet setCancelButtonWithTitle:@"take a picture" block:^{
            [self performSelector:@selector(showImagePickerForCamera) withObject:nil afterDelay:.1f];
        }];
    }
    if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeSavedPhotosAlbum]) {
        [actionSheet setCancelButtonWithTitle:@"Choose a picture" block:^{
            if (isIpad) {
                [self performSelector:@selector(showImagePickerForAlbumOnIpad:) withObject:view afterDelay:.1f];
            }else{
                [self performSelector:@selector(showImagePickerForAlbum) withObject:nil afterDelay:.1f];
            }
        }];
    }
    [actionSheet setCancelButtonWithTitle:@"cancel" block:NULL];
    [actionSheet showInView:view];
}

- (void)showImagePickerForCamera {
    UIImagePickerController *picker = [self imagePickerController];
    picker.sourceType = UIImagePickerControllerSourceTypeCamera;
    [self.rootViewController.navigationController presentViewController:picker animated:YES completion:nil];
}

- (void)showImagePickerForAlbum {
    UIImagePickerController *picker = [self imagePickerController];
    picker.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
    [self.rootViewController.navigationController presentViewController:picker animated:YES completion:nil];
}

- (void)showImagePickerForAlbumOnIpad:(UIView *)view {
    
    UIImagePickerController *picker = [self imagePickerController];
    picker.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
    
    UIPopoverController *popover = [[UIPopoverController alloc] initWithContentViewController:picker];
    self.popoverController = popover;
    [self.popoverController presentPopoverFromRect:CGRectMake(0, 0, 300, 400) inView:view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
}
- (void)dismissImagePicker {
    [self.rootViewController.navigationController dismissViewControllerAnimated:YES completion:nil];
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];
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
    
    UIImage *editImage = [info objectForKey:UIImagePickerControllerEditedImage];
    CGFloat minEdge = MIN(editImage.size.width, editImage.size.height);
    if (kImageMaxEdgeLen < minEdge) {
        editImage = [editImage resizedImageWithContentMode:UIViewContentModeScaleAspectFit
                                                    bounds:CGSizeMake(kImageMaxEdgeLen, kImageMaxEdgeLen)
                                      interpolationQuality:kCGInterpolationMedium];
    } else if (editImage.size.width != editImage.size.height) {
        editImage = [editImage croppedImage:CGRectMake(0.f, 0.f, minEdge, minEdge)];
    }
    NSData *imageData = UIImageJPEGRepresentation(editImage, .85f);
    if (imageData && self.finishBlock){
        self.finishBlock(imageData);
    }
    [self dismissImagePicker];
    self.imagePicker = nil;
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
