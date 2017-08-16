//
//  LocationManager.m
//  Casino
//
//  Created by Zheng Wu on 2017/5/8.
//
//

#import "LocationManager.h"

static LocationManager* __instance = nil;

@implementation LocationManager

+ (LocationManager *)sharedInstance {
    if (__instance == nil) {
        __instance = [[LocationManager alloc] init];
    }
    return __instance;
}

- (id)init {
    
    self = [super init];
    if (self) {
        // init here
        [self startUpdatingLocation];
    }
    return self;
}

+ (id)alloc {
    
    if (__instance) {
        return __instance;
    }
    return [super alloc];
}

- (void)startUpdatingLocation {
    if ([CLLocationManager locationServicesEnabled]) {
        self.locationManager = [[[CLLocationManager alloc] init] autorelease];
        self.locationManager.delegate = self;
        [self.locationManager requestWhenInUseAuthorization];
        [self.locationManager setDesiredAccuracy:kCLLocationAccuracyBest];
        [self.locationManager setDistanceFilter:5.0];
    }
    [self.locationManager startUpdatingLocation];
}

- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray<CLLocation *> *)locations
{
    // 此处locations存储了持续更新的位置坐标，取最后一个值为最新位置，如果不想让其持续更新位置，则在此方法中获取到一个值之后，让locationManager stopUpdatingLocation
    CLLocation *currentLocation = [locations lastObject];
    CLLocationCoordinate2D coor = currentLocation.coordinate;
    latitude = coor.latitude;
    longitude = coor.longitude;
    NSLog(@"wz : latitude = %lf, longitude = %lf", latitude, longitude);
//    [locationManager stopUpdatingLocation];
}

- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error
{
    NSLog(@"location service error = %ld", (long)error.code);
    if (error.code == kCLErrorDenied) {
        // 提示用户出错原因
    }
}

- (Boolean)locationServicesEnabled
{
    if([CLLocationManager locationServicesEnabled]) {
        CLAuthorizationStatus status = [CLLocationManager authorizationStatus];
        switch (status) {
        case kCLAuthorizationStatusAuthorizedWhenInUse:
        case kCLAuthorizationStatusAuthorizedAlways:
            return true;
        case kCLAuthorizationStatusNotDetermined:
        case kCLAuthorizationStatusRestricted:
        case kCLAuthorizationStatusDenied:
            return false;
        }
    }else{
        return false;
    }
    return false;
}

- (void)requestLocationService {
//    if ([CLLocationManager locationServicesEnabled]) {
//        CLAuthorizationStatus status = [CLLocationManager authorizationStatus];
//        switch (status) {
//            case kCLAuthorizationStatusNotDetermined:
//            case kCLAuthorizationStatusRestricted:
//            case kCLAuthorizationStatusDenied:
//                [self.locationManager requestWhenInUseAuthorization];
//                break;
//                
//            default:
//                break;
//        }
//    } else {
        NSURL *url = [NSURL URLWithString:UIApplicationOpenSettingsURLString];
        if ([[UIApplication sharedApplication] canOpenURL:url]) {
            [[UIApplication sharedApplication] openURL:url];
        }
//    }
}

- (double)getLatitude {
    if([self locationServicesEnabled]) {
        return latitude;
    }
    return 0.0f;
}

- (double)getLongitude {
    if([self locationServicesEnabled]) {
        return longitude;
    }
    return 0.0f;
}

@end
