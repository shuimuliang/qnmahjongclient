//
//  IosImagePicker.h
//  Casino
//
//  Created by liulihua on 15/1/14.
//
//

#ifndef Casino_IosImagePicker_h
#define Casino_IosImagePicker_h
#import <Foundation/Foundation.h>

typedef void (^SelectFinishBlock)(NSString *filePath);
typedef void (^SelectCancelBlock)();


@interface IosImagePicker : NSObject
+ (IosImagePicker *)sharedInstance;
- (void)selectPhoto:(BOOL)isIpad
        finishBlock:(SelectFinishBlock) finishBlock
        cancelBlock:(SelectCancelBlock) cancelBlock;
@end
#endif
