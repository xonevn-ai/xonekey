//
//  MacroViewController.h
//  XoneKey
//
//  Created by Daniel Do on 16 Apr 2025.
//  Copyright © 2025 Daniel Do. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface MacroViewController : NSViewController<NSTableViewDataSource, NSTableViewDelegate, NSTextFieldDelegate>
@property (weak) IBOutlet NSTableView *tableView;
@property (weak) IBOutlet NSTextField *macroName;
@property (weak) IBOutlet NSTextField *macroContent;

@property (weak) IBOutlet NSButton *buttonAdd;
@property (weak) IBOutlet NSButton *AutoCapsMacro;

@end

NS_ASSUME_NONNULL_END
