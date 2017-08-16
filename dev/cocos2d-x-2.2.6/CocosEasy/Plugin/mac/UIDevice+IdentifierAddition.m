//
//  UIDevice(Identifier).m
//  UIDeviceAddition
//
//  Created by liulihua on 14-2-5.
//
//

#import "UIDevice+IdentifierAddition.h"
#import "SSKeychain.h"

@implementation UIDevice (IdentifierAddition)

//将 生成的uuid保存在 keychain里 随时可以读取之
//此处未加密
- (NSString *)uniqueDeviceIdentifier{
#if TARGET_IPHONE_SIMULATOR
  //    return [self stringWithNewUUID];
  return @"43765926636A4766AA35F63642CF59399";
//  return @"FC8B78075430454DB980D7BB7F50E0DC";
#endif
    
  NSString *key = [[NSBundle mainBundle] bundleIdentifier];
#if DEBUG
    NSLog(@"bundle id is : %@",key);
#endif
  NSString *retrieveuuid = [SSKeychain passwordForService:key account:@"deviceID"];
  if (retrieveuuid == nil) {
    NSString *idStr = [self stringWithNewUUID];
    idStr = [idStr stringByReplacingOccurrencesOfString:@"-" withString:@""];
    [SSKeychain setPassword:idStr forService:key account:@"deviceID"];
    retrieveuuid = [SSKeychain passwordForService:key account:@"deviceID"];
  }
  return retrieveuuid;
}

- (NSString*)stringWithNewUUID
{
    // Create a new UUID
    CFUUIDRef uuidObj = CFUUIDCreate(nil);
    
    // Get the string representation of the UUID
    NSString *newUUID = (NSString*)CFBridgingRelease(CFUUIDCreateString(nil, uuidObj));
    CFRelease(uuidObj);
    return newUUID;
}
@end
