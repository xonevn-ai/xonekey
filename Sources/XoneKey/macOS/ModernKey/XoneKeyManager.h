//
//  XoneKeyManager.h
//  ModernKey
//
//  Created by Daniel Do on 16 Apr 2025.
//  Copyright © 2025 Daniel Do. All rights reserved.
//

#ifndef XoneKeyManager_h
#define XoneKeyManager_h

#import <Cocoa/Cocoa.h>

typedef void (^CheckNewVersionCallback)(void);

@interface XoneKeyManager : NSObject
+(BOOL)isInited;
+(BOOL)initEventTap;
+(BOOL)stopEventTap;

+(NSArray*)getTableCodes;

+(NSString*)getBuildDate;
+(void)showMessage:(NSWindow*)window message:(NSString*)msg subMsg:(NSString*)subMsg;

+(BOOL)quickConvert;

+(void)checkNewVersion:(NSWindow*)parent callbackFunc:(CheckNewVersionCallback) callback;
@end

#endif /* XoneKeyManager_h */
