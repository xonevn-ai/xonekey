//
//  AboutViewController.h
//  XoneKey
//
//  Created by Daniel Do on 16 Apr 2025.
//  Copyright © 2025 Daniel Do. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface AboutViewController : NSViewController
@property (weak) IBOutlet NSTextField *VersionInfo;
@property (weak) IBOutlet NSButton *CheckNewVersionButton;
@property (weak) IBOutlet NSButton *CheckUpdateOnStatus;

@end

NS_ASSUME_NONNULL_END
