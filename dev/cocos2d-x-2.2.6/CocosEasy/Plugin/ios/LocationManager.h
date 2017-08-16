//
//  LocationManager.h
//  Casino
//
//  Created by Zheng Wu on 2017/5/8.
//
//

#ifndef LocationManager_h
#define LocationManager_h

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>

@interface LocationManager : NSObject<CLLocationManagerDelegate>

{
    CLLocationDegrees latitude;
    CLLocationDegrees longitude;
}

@property (strong, nonatomic) CLLocationManager* locationManager;

+ (LocationManager *)sharedInstance;
- (Boolean)locationServicesEnabled;
- (void)startUpdatingLocation;
- (double)getLatitude;
- (double)getLongitude;
- (void)requestLocationService;

@end

#endif /* LocationManager_h */
