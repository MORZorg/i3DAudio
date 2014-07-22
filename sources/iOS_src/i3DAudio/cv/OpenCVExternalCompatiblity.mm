//
//  OpenCVExternalCompatiblity.m
//  i3DAudio
//
//  Created by Maurizio Zucchelli on 20/07/14.
//  Copyright (c) 2014 MORZorg. All rights reserved.
//

#import "OpenCVExternalCompatibility.h"
#import <opencv2/highgui/cap_ios.h>

@implementation OpenCVExternalCompatibility
+(void) test: (UIImageView*) imageView {
    CvVideoCamera* videoCamera = [[CvVideoCamera alloc] initWithParentView:imageView];
    videoCamera.defaultAVCaptureDevicePosition = AVCaptureDevicePositionFront;
    videoCamera.defaultAVCaptureSessionPreset = AVCaptureSessionPreset352x288;
    videoCamera.defaultAVCaptureVideoOrientation = AVCaptureVideoOrientationPortrait;
    videoCamera.defaultFPS = 30;
}
@end
