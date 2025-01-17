//
//  AppDelegate.m
//  OvrvisionCalibration
//
//  Created by WizapplyOSX on 2015/12/22.
//  Copyright (c) 2015 Wizapply. All rights reserved.
//

#import "AppDelegate.h"
#import "ChessSizeFormDelegate.h"
#import "HowToUseDelegate.h"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Use the classes and functions in the bundle.
    NSBundle *thisBundle = [NSBundle mainBundle];
    NSString *bundlePath= [thisBundle pathForResource:@"ovrvision" ofType:@"bundle"];
    NSURL* bundleURL = [NSURL fileURLWithPath:bundlePath];
    
    cfBundle = CFBundleCreate(kCFAllocatorDefault, (__bridge CFURLRef)bundleURL);
    if(cfBundle)
    {
        ovOpen = (ovOpen_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovOpen"));
        ovClose = (ovClose_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovClose"));
        ovRelease = (ovRelease_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovRelease"));
        ovPreStoreCamData = (ovPreStoreCamData_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovPreStoreCamData"));
        ovGetCamImageBGRA = (ovGetCamImageBGRA_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetCamImageBGRA"));
        ovGetCamImageBGRAPointer = (ovGetCamImageBGRAPointer_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetCamImageBGRAPointer"));
        ovGetCamImageRGB = (ovGetCamImageRGB_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetCamImageRGB"));
        ovGetCamImageBGR = (ovGetCamImageBGR_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetCamImageBGR"));
        ovGetCamImageForUnity = NULL;
        ovGetCamImageForUnityNative = NULL;
        
        ovGetImageWidth = (ovGetImageWidth_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetImageWidth"));
        ovGetImageHeight =(ovGetImageHeight_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetImageHeight"));
        ovGetImageRate = (ovGetImageRate_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetImageRate"));
        ovGetBufferSize = (ovGetBufferSize_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetBufferSize"));
        ovGetPixelSize = (ovGetPixelSize_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetPixelSize"));
        
        //Set camera properties
        ovSetExposure = (ovSetExposure_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovSetExposure"));
        ovSetGain = (ovSetGain_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovSetGain"));
        ovSetBLC = (ovSetBLC_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovSetBLC"));
        ovSetWhiteBalanceR = (ovSetWhiteBalanceR_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovSetWhiteBalanceR"));
        ovSetWhiteBalanceG = (ovSetWhiteBalanceG_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovSetWhiteBalanceG"));
        ovSetWhiteBalanceB = (ovSetWhiteBalanceB_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovSetWhiteBalanceB"));
        ovSetWhiteBalanceAuto = (ovSetWhiteBalanceAuto_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovSetWhiteBalanceAuto"));
        //Get camera properties
        ovGetExposure = (ovGetExposure_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetExposure"));
        ovGetGain = (ovGetGain_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetGain"));
        ovGetBLC = (ovGetBLC_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetBLC"));
        ovGetWhiteBalanceR = (ovGetWhiteBalanceR_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetWhiteBalanceR"));
        ovGetWhiteBalanceG = (ovGetWhiteBalanceG_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetWhiteBalanceG"));
        ovGetWhiteBalanceB = (ovGetWhiteBalanceB_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetWhiteBalanceB"));
        ovGetWhiteBalanceAuto = (ovGetWhiteBalanceAuto_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetWhiteBalanceAuto"));
        ovGetFocalPoint = (ovGetFocalPoint_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetFocalPoint"));
        ovGetHMDRightGap = (ovGetHMDRightGap_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovGetHMDRightGap"));
        
        ovSetCamSyncMode = (ovSetCamSyncMode_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovSetCamSyncMode"));
        
        ////////////// Ovrvision AR System //////////////
        ovARRender = (ovARRender_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovARRender"));
        ovARGetData = (ovARGetData_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovARGetData"));
        ovARSetMarkerSize = (ovARSetMarkerSize_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovARSetMarkerSize"));
        ovARGetMarkerSize = (ovARGetMarkerSize_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovARGetMarkerSize"));
        
        ////////////// Ovrvision Tracking System //////////////
        //testing
        //ovTrackRender_ptr ovTrackRender;
        //ovGetTrackData_ptr ovGetTrackData;
        //ovTrackingCalibReset_ptr ovTrackingCalibReset;
        
        ////////////// Ovrvision Calibration //////////////
        ovCalibInitialize = (ovCalibInitialize_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovCalibInitialize"));
        ovCalibClose = (ovCalibClose_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovCalibClose"));
        ovCalibFindChess = (ovCalibFindChess_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovCalibFindChess"));
        ovCalibSolveStereoParameter = (ovCalibSolveStereoParameter_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovCalibSolveStereoParameter"));
        ovCalibGetImageCount = (ovCalibGetImageCount_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovCalibGetImageCount"));
        
        ovSaveCamStatusToEEPROM = (ovSaveCamStatusToEEPROM_ptr)CFBundleGetFunctionPointerForName(cfBundle, CFSTR("ovSaveCamStatusToEEPROM"));
        //////////////////////////////////
        
        NSLog(@"[OK] Loaded the ovrvision.bundle.");
    }
    
    // ボードを表示するためにNSImageを作成
    NSString *chessboradPath= [thisBundle pathForResource:@"chess4x7" ofType:@"bmp"];
    NSImage *image = [[NSImage alloc] initByReferencingFile:chessboradPath];
    // Image Wellに設定
    [self.imageView setImage:image];
    
    endThread = false;
}

- (void)dealloc {

    if([[_OvrvisionCtrl title] isEqualToString:@"Close Ovrvision"]) {
        endThread = false;
        if(ovClose()==0) {
            ovRelease();
            NSLog(@"Ovrvision was closed.");
        }
    }
    
    //close
    CFRelease(cfBundle);
}

- (IBAction)OvrvisionCtrlButton:(id)sender {
    
    if([[_OvrvisionCtrl title] isEqualToString:@"Open Ovrvision"])
    {
        if(ovOpen(0, 0.15f, OV_CAMHD_FULL)==0)
        {
            [_OvrvisionCtrl setTitle:@"Close Ovrvision"];
            [_OvrvisionCalib setEnabled: YES];
            
            [_outputTextView setString:@"Ovrvision was opened.\n"]; //initialize text

            //camera image
            left_bitmap = [[NSBitmapImageRep alloc]
                                        initWithBitmapDataPlanes:NULL
                                        pixelsWide:ovGetImageWidth()
                                        pixelsHigh:ovGetImageHeight()
                                        bitsPerSample:8
                                        samplesPerPixel:3
                                        hasAlpha:NO
                                        isPlanar:NO
                                        colorSpaceName:NSCalibratedRGBColorSpace
                                        bytesPerRow:ovGetImageWidth()*3
                                        bitsPerPixel:24];
        
            right_bitmap = [[NSBitmapImageRep alloc]
                           initWithBitmapDataPlanes:NULL
                           pixelsWide:ovGetImageWidth()
                           pixelsHigh:ovGetImageHeight()
                           bitsPerSample:8
                           samplesPerPixel:3
                           hasAlpha:NO
                           isPlanar:NO
                           colorSpaceName:NSCalibratedRGBColorSpace
                           bytesPerRow:ovGetImageWidth()*3
                           bitsPerPixel:24];
            
            endThread = true;
            [NSThread detachNewThreadSelector:@selector(updateThread)
                                     toTarget:self
                                   withObject:nil];
        }
        
    }
    else if([[_OvrvisionCtrl title] isEqualToString:@"Close Ovrvision"])
    {
        endThread = false;
        
        if(ovClose()==0)
        {
            [_OvrvisionCtrl setTitle:@"Open Ovrvision"];
            [_OvrvisionCalib setTitle:@"Start Calibration"];
            [_OvrvisionCalib setEnabled: NO];
            
            [self.leftCameraView setImage:NULL];
            [self.rightCameraView setImage:NULL];
        }
    }

}

- (IBAction)OvrvisionCalibButton:(id)sender {

    if ([[_OvrvisionCalib title] isEqualToString:@"Start Calibration"])
    {
        float tilesize = 0;
        
        ChessSizeFormDelegate* chesssizeform = [[ChessSizeFormDelegate alloc] initWithWindowNibName:@"ChessSizeForm"];
        int result = (int)[[NSApplication sharedApplication] runModalForWindow:[chesssizeform window]];
        [[chesssizeform window] orderOut:nil];
        
        if(result == 1) {
            tilesize = [chesssizeform getSize];
            
            if(tilesize > 5) {
                
                HowToUseDelegate* howtouse = [[HowToUseDelegate alloc] initWithWindowNibName:@"HowToUse"];
                int result = (int)[[NSApplication sharedApplication] runModalForWindow:[howtouse window]];
                [[howtouse window] orderOut:nil];
                
                ovCalibInitialize(7, 4, tilesize);
                [_OvrvisionCalib setTitle:@"Find Chessboard"];
        
                [self appendText:[NSString stringWithFormat:@"Start calibration.... tile size is %f.2mm\n", tilesize]];
            }
        }
    }
    else if ([[_OvrvisionCalib title] isEqualToString:@"Find Chessboard"])
    {
        if(ovCalibFindChess() != 0)
            [self appendText:[NSString stringWithFormat:@"[Success]Chess board was found: No.%d/%d\n", ovCalibGetImageCount(), CalibrationImageNum]];
        else
            [self appendText:@"[Failure]Can not find the chess board.\n"];
        
        if (ovCalibGetImageCount() >= CalibrationImageNum)
        {
            [_OvrvisionCalib setTitle:@"Create Parameters"];
        }
    }
    else if ([[_OvrvisionCalib title] isEqualToString:@"Create Parameters"])
    {
        [self appendText:@"Setup in the data..... "];
        ovCalibSolveStereoParameter();
        [self appendText:@"Success.\n"];
        [NSThread sleepForTimeInterval:0.5]; //1s wait
        [self appendText:@"The calibration params was saved successfully.\n"];
        
        //Close
        endThread = false;
        if (ovClose() == 0)
        {
            [_OvrvisionCtrl setTitle:@"Open Ovrvision"];
            [_OvrvisionCalib setTitle:@"Start Calibration"];
            [_OvrvisionCalib setEnabled: NO];
            
            [self.leftCameraView setImage:NULL];
            [self.rightCameraView setImage:NULL];
        }
        
        [self appendText:@"Ovrvision calibration is done.\n"];
    }
    
}

- (void)updateThread
{
    //update view
    while(endThread)
    {
        NSImage *left_image = [[NSImage alloc] init];
        NSImage *right_image = [[NSImage alloc] init];
        
        ovPreStoreCamData(OV_CAMQT_DMS);
        
        ovGetCamImageRGB([left_bitmap bitmapData],OV_CAMEYE_LEFT);    //Ref
        ovGetCamImageRGB([right_bitmap bitmapData],OV_CAMEYE_RIGHT);
        
        [left_image addRepresentation:left_bitmap];
        [right_image addRepresentation:right_bitmap];

        [self.leftCameraView setImage:left_image];
        [self.rightCameraView setImage:right_image];
        
        [NSThread sleepForTimeInterval:0.005];  //5ms
    }
    
}

-(void)appendText:(NSString*)text;
{
    //描画を一時的に止める
    [_outputTextView.textStorage beginEditing];
    
    //テキストを追加
    NSAttributedString* atrstr = [[NSAttributedString alloc] initWithString:text];
    [_outputTextView.textStorage appendAttributedString: atrstr];
    
    //描画再開
    [_outputTextView.textStorage endEditing];
    
    //最終行へスクロール
    [_outputTextView autoscroll:nil];
}

- (IBAction)QuitTool:(id)sender {
    [[NSApplication sharedApplication] terminate:self];
}

@end
