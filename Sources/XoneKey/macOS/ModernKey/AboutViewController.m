//
//  AboutViewController.m
//  XoneKey
//
//  Created by Daniel Do on 16 Apr 2025.
//  Copyright © 2025 Daniel Do. All rights reserved.
//

#import "AboutViewController.h"
#import "XoneKeyManager.h"

@interface AboutViewController ()

@end

@implementation AboutViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do view setup here.
    
    self.VersionInfo.stringValue = [NSString stringWithFormat:@"Phiên bản %@ (build %@) - Ngày cập nhật %@",
                                    [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleShortVersionString"],
                                    [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleVersion"],
                                    [XoneKeyManager getBuildDate]] ;
    
    NSInteger dontCheckUpdate = [[NSUserDefaults standardUserDefaults] integerForKey:@"DontCheckUpdate"];
    self.CheckUpdateOnStatus.state = dontCheckUpdate ? NSControlStateValueOff :NSControlStateValueOn;
}

- (IBAction)onHomePage:(id)sender {
    [[NSWorkspace sharedWorkspace] openURL: [NSURL URLWithString:@"https://github.com/vituocgia/XoneKey"]];
}

- (IBAction)onFanPage:(id)sender {
    [[NSWorkspace sharedWorkspace] openURL: [NSURL URLWithString:@"https://www.facebook.com/XoneKeyVN"]];
}

- (IBAction)onLatestReleaseVersion:(id)sender {
    [[NSWorkspace sharedWorkspace] openURL: [NSURL URLWithString:@"https://github.com/vituocgia/XoneKey/releases"]];
}

- (IBAction)onCheckUpdateOnStartup:(NSButton *)sender {
    NSInteger val = sender.state == NSControlStateValueOn ? 0 : 1;
    [[NSUserDefaults standardUserDefaults] setInteger:val forKey:@"DontCheckUpdate"];
}

- (IBAction)onCheckNewVersion:(id)sender {
    
    self.CheckNewVersionButton.title = @"Đang kiểm tra...";
    self.CheckNewVersionButton.enabled = false;
    
    [XoneKeyManager checkNewVersion: self.view.window callbackFunc:^{
        self.CheckNewVersionButton.enabled = true;
        self.CheckNewVersionButton.title = @"Kiểm tra bản mới...";
    }];
}

@end
