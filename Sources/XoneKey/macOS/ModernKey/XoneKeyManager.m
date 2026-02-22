//
//  XoneKeyManager.m
//  XoneKey
//
//  Created by Daniel Do on 16 Apr 2025.
//  Copyright © 2025 Daniel Do. All rights reserved.
//

#import "XoneKeyManager.h"

extern void XoneKeyInit(void);

extern CGEventRef XoneKeyCallback(CGEventTapProxy proxy,
                                  CGEventType type,
                                  CGEventRef event,
                                  void *refcon);

extern NSString* ConvertUtil(NSString* str);

@interface XoneKeyManager ()

@end

@implementation XoneKeyManager {

}
static BOOL _isInited = NO;
static NSLock *_initLock = nil;

static CFMachPortRef      eventTap;
static CGEventMask        eventMask;
static CFRunLoopSourceRef runLoopSource;

+(void)initialize {
    if (self == [XoneKeyManager class]) {
        _initLock = [[NSLock alloc] init];
    }
}

+(BOOL)isInited {
    [_initLock lock];
    @try {
        return _isInited;
    } @finally {
        [_initLock unlock];
    }
}

+(BOOL)initEventTap {
    [_initLock lock];
    @try {
        if (_isInited) {
            return YES;
        }
    
        //init modernKey
        XoneKeyInit();
        
        // Create an event tap. We are interested in key presses.
        eventMask = ((1 << kCGEventKeyDown) |
                     (1 << kCGEventKeyUp) |
                     (1 << kCGEventFlagsChanged) |
                     (1 << kCGEventLeftMouseDown) |
                     (1 << kCGEventRightMouseDown) |
                     (1 << kCGEventLeftMouseDragged) |
                     (1 << kCGEventRightMouseDragged));
        
        eventTap = CGEventTapCreate(kCGSessionEventTap,
                                    kCGHeadInsertEventTap,
                                    0,
                                    eventMask,
                                    XoneKeyCallback,
                                    NULL);
        
        if (!eventTap) {
            NSLog(@"Failed to create event tap - check Accessibility permission");
            return NO;
        }
        
        // Create a run loop source with error checking
        runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
        if (!runLoopSource) {
            NSLog(@"Failed to create run loop source");
            CFRelease(eventTap);
            eventTap = NULL;
            return NO;
        }
        
        // Add to the current run loop.
        CFRunLoopRef runLoop = CFRunLoopGetCurrent();
        if (!runLoop) {
            NSLog(@"Failed to get current run loop");
            CFRelease(runLoopSource);
            CFRelease(eventTap);
            runLoopSource = NULL;
            eventTap = NULL;
            return NO;
        }
        
        CFRunLoopAddSource(runLoop, runLoopSource, kCFRunLoopCommonModes);
        
        // Enable the event tap with verification
        CGEventTapEnable(eventTap, true);
        if (!CGEventTapIsEnabled(eventTap)) {
            NSLog(@"Event tap enabled but verification failed");
            CFRunLoopRemoveSource(runLoop, runLoopSource, kCFRunLoopCommonModes);
            CFRelease(runLoopSource);
            CFRelease(eventTap);
            runLoopSource = NULL;
            eventTap = NULL;
            return NO;
        }
        
        _isInited = YES;
        
        // Set it all running.
        CFRunLoopRun();
        
        return YES;
    } @finally {
        [_initLock unlock];
    }
}

+(BOOL)stopEventTap {
    [_initLock lock];
    @try {
        if (!_isInited) {
            return YES;
        }
        
        CFRunLoopRef runLoop = CFRunLoopGetCurrent();
        if (runLoop && runLoopSource) {
            CFRunLoopStop(runLoop);
            CFRunLoopRemoveSource(runLoop, runLoopSource, kCFRunLoopDefaultMode);
        }
        
        if (runLoopSource) {
            CFRelease(runLoopSource);
            runLoopSource = NULL;
        }
        
        if (eventTap) {
            CGEventTapEnable(eventTap, false);
            CFMachPortInvalidate(eventTap);
            CFRelease(eventTap);
            eventTap = NULL;
        }
        
        _isInited = NO;
    } @catch (NSException *exception) {
        NSLog(@"Error stopping event tap: %@", exception);
        _isInited = NO;
    } @finally {
        [_initLock unlock];
    }
    
    return YES;
}

+(NSArray*)getTableCodes {
    return [[NSArray alloc] initWithObjects:
            @"Unicode",
            @"TCVN3 (ABC)",
            @"VNI Windows",
            @"Unicode tổ hợp",
            @"Vietnamese Locale CP 1258", nil];
}

+(NSString*)getBuildDate {
    return [NSString stringWithUTF8String:__DATE__];
}

#pragma mark -Convert feature
+(BOOL)quickConvert {
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    NSString *htmlString = [pasteboard stringForType:NSPasteboardTypeHTML];
    NSString *rawString = [pasteboard stringForType:NSPasteboardTypeString];
    bool converted = false;
    if (htmlString != nil) {
        htmlString = ConvertUtil(htmlString);
        converted = true;
    }
    if (rawString != nil) {
        rawString = ConvertUtil(rawString);
        converted = true;
    }
    if (converted) {
        [pasteboard clearContents];
        if (htmlString != nil)
            [pasteboard setString:htmlString forType:NSPasteboardTypeHTML];
        if (rawString != nil)
            [pasteboard setString:rawString forType:NSPasteboardTypeString];
        
        return YES;
    }
    return NO;
}

+(void)showMessage:(NSWindow*)window message:(NSString*)msg subMsg:(NSString*)subMsg {
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:msg];
    [alert setInformativeText:subMsg];
    [alert addButtonWithTitle:@"OK"];
    if (window) {
        [alert beginSheetModalForWindow:window completionHandler:^(NSModalResponse returnCode) {
        }];
    } else {
        [alert runModal];
    }
}

#pragma mark -AutoUpdate feature

+(void)checkNewVersion:(NSWindow*)parent callbackFunc:(CheckNewVersionCallback) callback {
    //load new version config
    NSURLSession *aSession = [NSURLSession sessionWithConfiguration:[NSURLSessionConfiguration defaultSessionConfiguration]];
    [[aSession dataTaskWithURL:[NSURL URLWithString:@"https://raw.githubusercontent.com/vituocgia/XoneKey/master/version.json"] completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
        if (((NSHTTPURLResponse *)response).statusCode == 200) {
            if (data) {
                if(NSClassFromString(@"NSJSONSerialization")) {
                    NSError *error = nil;
                    id object = [NSJSONSerialization
                                 JSONObjectWithData:data
                                 options:0
                                 error:&error];
                    
                    if(error) {  }
                    if([object isKindOfClass:[NSDictionary class]]) {
                        NSDictionary *results = object;
                        NSDictionary *ver = [results valueForKey:@"latestVersion"];
                        NSString* versionCodeString = [ver valueForKey:@"versionCode"];
                        int versionCode = (int)[versionCodeString integerValue];
                        int currentVersionCode = (int)[((NSString*)[[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleVersion"]) integerValue];
                        
                        dispatch_async(dispatch_get_main_queue(), ^{
                            if (callback != nil) {
                                callback();
                            }
                            if (versionCode > currentVersionCode || callback != nil) {
                                [self showUpdateMessage:parent needUpdating:versionCode > currentVersionCode newVersion:[ver valueForKey:@"versionName"]];
                            }
                        });
                    }
                    else {
                        //oh my god
                    }
                }
                else {
                    //can not parse json
                }
            }
        }
    }] resume];
}

+(void)showUpdateMessage:(NSWindow*)parent needUpdating:(BOOL)needUpdating newVersion:(NSString*)versionString {
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:(needUpdating ? [NSString stringWithFormat:@"XoneKey Có phiên bản mới (%@), bạn có muốn cập nhật không?", versionString] : @"Bạn đang dùng phiên bản mới nhất!")];
    [alert setInformativeText:(needUpdating ? @"Bấm 'Có' để cập nhật XoneKey." : @"")];
    
    if (!needUpdating) {
        [alert addButtonWithTitle:@"OK"];
    } else {
        [alert addButtonWithTitle:@"Có"];
        [alert addButtonWithTitle:@"Không"];
    }
    if (parent == nil) {
        [alert.window makeKeyAndOrderFront:nil];
        [alert.window setLevel:NSStatusWindowLevel];
        NSModalResponse res = [alert runModal];
        if (res == 1000 && needUpdating) {
            [self launchUpdateHelper];
        }
    } else {
        [alert beginSheetModalForWindow:parent completionHandler:^(NSModalResponse returnCode) {
            if (returnCode == 1000 && needUpdating) {
                [self launchUpdateHelper];
            }
        }];
    }
}

+(void)launchUpdateHelper {
    //check update app has exist or not
    NSError *copyError = nil;
    NSString* target = [NSString stringWithFormat:@"%@/XoneKeyUpdate.app", [self getApplicationSupportFolder]];
    [[NSFileManager defaultManager] removeItemAtPath:target error:&copyError];
    if (![[NSFileManager defaultManager] fileExistsAtPath:target]) {
        [[NSFileManager defaultManager] createDirectoryAtPath:[self getApplicationSupportFolder] withIntermediateDirectories:YES attributes:nil error:nil];
        
        if (![[NSFileManager defaultManager] copyItemAtPath:[self getUpdateBundlePath] toPath:target error:&copyError]) {
            NSLog(@"Error on copy");
        }
    }
    
    NSWorkspace *workspace = [NSWorkspace sharedWorkspace];
    NSURL *url = [NSURL fileURLWithPath:[workspace fullPathForApplication:target]];
    NSError *error = nil;
    NSArray *arguments = [NSArray arrayWithObjects: @"yeah", nil];
    [workspace launchApplicationAtURL:url options:0 configuration:[NSDictionary dictionaryWithObject:arguments forKey:NSWorkspaceLaunchConfigurationArguments] error:&error];
    
    [NSApp terminate:0]; //exit main app
}

+(NSString*)getApplicationSupportFolder {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
    NSString *applicationSupportDirectory = [paths firstObject];
    return [NSString stringWithFormat:@"%@/XoneKey", applicationSupportDirectory];
}

+(NSString*)getUpdateBundlePath {
    NSString *currentpath = [[NSBundle mainBundle] bundlePath];
    return [NSString stringWithFormat:@"%@/Contents/Library/LoginItems/XoneKeyUpdate.app", currentpath];
}
@end
