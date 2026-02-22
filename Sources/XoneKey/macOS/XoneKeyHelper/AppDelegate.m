//
//  AppDelegate.m
//  XoneKeyHelper
//
//  Created by Daniel Do on 16 Apr 2025.
//  Copyright © 2025 Daniel Do. All rights reserved.
//

#import "AppDelegate.h"

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    NSArray* runningApp = [[NSWorkspace sharedWorkspace] runningApplications];
    if (![runningApp containsObject:@"com.xonevn.openkey"]) {
        NSString* path = [[NSBundle mainBundle] bundlePath];
        for (int i = 0; i < 4; i++)
            path = [path stringByDeletingLastPathComponent];
        [[NSWorkspace sharedWorkspace] launchApplication:path];
    }
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


@end
