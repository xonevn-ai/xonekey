# XoneKey macOS - Developer Guide

## Table of Contents
1. [Project Overview](#project-overview)
2. [Architecture](#architecture)
3. [Setup & Build](#setup--build)
4. [Code Structure](#code-structure)
5. [Key Components](#key-components)
6. [Development Workflow](#development-workflow)
7. [Testing](#testing)
8. [Debugging](#debugging)
9. [Best Practices](#best-practices)
10. [Troubleshooting](#troubleshooting)

## Project Overview

XoneKey là ứng dụng gõ tiếng Việt cho macOS, sử dụng CGEventTap để intercept keyboard events và xử lý qua engine C++.

### Requirements
- macOS 10.13+ (High Sierra or later)
- Xcode 12.0+
- Objective-C và C++ knowledge
- Accessibility API understanding

### Key Features
- Vietnamese input method (Telex, VNI, Simple Telex)
- Multiple code tables (Unicode, TCVN3, VNI Windows, etc.)
- Macro support
- Smart switch key
- Code conversion tool
- Auto-update mechanism

## Architecture

### High-Level Architecture

```
┌─────────────────────────────────────┐
│         XoneKey.app                 │
│  ┌───────────────────────────────┐  │
│  │      AppDelegate.m            │  │
│  │  - Application lifecycle      │  │
│  │  - Menu bar management        │  │
│  │  - Settings persistence       │  │
│  └───────────────────────────────┘  │
│  ┌───────────────────────────────┐  │
│  │     ViewController.m          │  │
│  │  - Settings UI                │  │
│  │  - User preferences           │  │
│  └───────────────────────────────┘  │
│  ┌───────────────────────────────┐  │
│  │     XoneKeyManager.m          │  │
│  │  - Event tap management       │  │
│  │  - Utilities                  │  │
│  └───────────────────────────────┘  │
│  ┌───────────────────────────────┐  │
│  │      XoneKey.mm               │  │
│  │  - CGEventTap callback        │  │
│  │  - C++ engine bridge          │  │
│  └───────────────────────────────┘  │
└─────────────────────────────────────┘
              │
              ▼
┌─────────────────────────────────────┐
│      Engine (C++)                   │
│  - Vietnamese.cpp                   │
│  - Engine.cpp                      │
│  - Macro.cpp                       │
│  - SmartSwitchKey.cpp              │
└─────────────────────────────────────┘
```

### Component Relationships

1. **AppDelegate** - Main application controller
   - Manages application lifecycle
   - Creates and manages menu bar
   - Handles settings persistence
   - Coordinates with XoneKeyManager

2. **XoneKeyManager** - Core functionality manager
   - Initializes event tap
   - Manages keyboard event interception
   - Provides utility functions
   - Handles update checking

3. **XoneKey.mm** - Event handling bridge
   - CGEventTap callback implementation
   - Bridges Objective-C and C++ code
   - Processes keyboard events
   - Sends processed events back

4. **ViewController** - Settings UI
   - User preferences interface
   - Settings validation
   - UI state management

## Setup & Build

### Prerequisites

1. **Install Xcode**
   ```bash
   # Install from App Store or developer.apple.com
   ```

2. **Install Command Line Tools**
   ```bash
   xcode-select --install
   ```

3. **Clone Repository**
   ```bash
   git clone <repository-url>
   cd XoneKey/Sources/XoneKey
   ```

### Build Configuration

1. **Open Project**
   ```bash
   open macOS/XoneKey.xcodeproj
   ```

2. **Configure Signing**
   - Select project in Xcode
   - Go to "Signing & Capabilities"
   - Configure team and certificates
   - Enable "Automatically manage signing"

3. **Build Settings**
   - Deployment Target: macOS 10.13+
   - Architecture: x86_64, arm64 (Universal)
   - C++ Standard: C++14 or later

4. **Build**
   ```bash
   # In Xcode: Product > Build (Cmd+B)
   # Or command line:
   xcodebuild -project macOS/XoneKey.xcodeproj -scheme XoneKey -configuration Release
   ```

### Dependencies

- **System Frameworks:**
  - AppKit.framework
  - Cocoa.framework
  - CoreGraphics.framework
  - Carbon.framework
  - ServiceManagement.framework
  - Foundation.framework

- **Engine (C++):**
  - Shared engine code in `../engine/`
  - Platform-specific headers in `engine/platforms/mac.h`

## Code Structure

### Directory Structure

```
macOS/
├── ModernKey/              # Main application
│   ├── AppDelegate.h/m     # Application delegate
│   ├── ViewController.h/m  # Settings view controller
│   ├── XoneKeyManager.h/m  # Core manager
│   ├── XoneKey.mm          # Event handling bridge
│   ├── MJAccessibilityUtils.h/m  # Accessibility helpers
│   ├── Resources/          # Images, icons
│   └── Base.lproj/         # Storyboards
├── XoneKeyHelper/          # Login item helper
│   └── AppDelegate.m      # Helper app delegate
├── XoneKey.xcodeproj/      # Xcode project
└── XoneKey.entitlements    # App entitlements
```

### Key Files

#### AppDelegate.m
- **Purpose:** Main application controller
- **Key Methods:**
  - `applicationDidFinishLaunching:` - App initialization
  - `createStatusBarMenu` - Menu bar setup
  - `onInputMethodSelected` - Toggle Vietnamese/English
  - `loadDefaultConfig` - First launch setup

#### XoneKeyManager.m
- **Purpose:** Core functionality manager
- **Key Methods:**
  - `initEventTap` - Initialize keyboard event tap
  - `stopEventTap` - Stop event tap
  - `checkNewVersion:` - Check for updates
  - `quickConvert` - Code conversion utility

#### XoneKey.mm
- **Purpose:** Event handling bridge
- **Key Functions:**
  - `XoneKeyInit()` - Initialize engine
  - `XoneKeyCallback()` - CGEventTap callback
  - Event processing and sending

## Key Components

### 1. Event Tap System

**Purpose:** Intercept global keyboard events

**Implementation:**
```objc
// In XoneKeyManager.m
eventTap = CGEventTapCreate(
    kCGSessionEventTap,      // Location
    kCGHeadInsertEventTap,   // Placement
    0,                       // Options
    eventMask,               // Events to capture
    XoneKeyCallback,         // Callback function
    NULL                     // User data
);
```

**Event Types:**
- `kCGEventKeyDown` - Key press
- `kCGEventKeyUp` - Key release
- `kCGEventFlagsChanged` - Modifier keys
- Mouse events (for word break detection)

### 2. Engine Integration

**Bridge Pattern:**
```objc
// Objective-C calls C++ engine
extern void XoneKeyInit(void);
extern CGEventRef XoneKeyCallback(...);

// C++ engine uses Objective-C
extern int vLanguage;
extern int vInputType;
// ... other global variables
```

**Global Variables:**
- Defined in `AppDelegate.m`
- Shared between Objective-C and C++
- Persisted in NSUserDefaults

### 3. Menu Bar Integration

**Status Item:**
```objc
NSStatusBar *statusBar = [NSStatusBar systemStatusBar];
statusItem = [statusBar statusItemWithLength:NSVariableStatusItemLength];
statusItem.button.image = [NSImage imageNamed:@"Status"];
```

**Menu Structure:**
- Toggle Vietnamese/English
- Input type selection (Telex, VNI, etc.)
- Code table selection
- Tools (Convert, Macro)
- Settings
- About
- Quit

### 4. Settings Persistence

**NSUserDefaults Keys:**
- `InputMethod` - Vietnamese/English (0/1)
- `InputType` - Telex/VNI/Simple Telex (0-3)
- `CodeTable` - Unicode/TCVN3/VNI (0-4)
- `SwitchKeyStatus` - Hotkey configuration
- `RunOnStartup` - Auto-start setting
- ... many more

**Loading:**
```objc
vLanguage = (int)[[NSUserDefaults standardUserDefaults] integerForKey:@"InputMethod"];
```

**Saving:**
```objc
[[NSUserDefaults standardUserDefaults] setInteger:vLanguage forKey:@"InputMethod"];
```

## Development Workflow

### 1. Making Changes

**Typical Workflow:**
1. Make code changes
2. Build project (Cmd+B)
3. Run in debugger (Cmd+R)
4. Test functionality
5. Fix issues
6. Commit changes

### 2. Adding New Features

**Example: Adding a new setting**

1. **Add to AppDelegate.m:**
   ```objc
   int vNewFeature = 0;
   ```

2. **Add to ViewController.m:**
   ```objc
   // UI control
   @property (weak) IBOutlet NSButton *newFeatureCheckbox;
   
   // Load value
   vNewFeature = (int)[[NSUserDefaults standardUserDefaults] integerForKey:@"NewFeature"];
   [self.newFeatureCheckbox setState:vNewFeature ? NSControlStateValueOn : NSControlStateValueOff];
   
   // Save value
   - (IBAction)onNewFeatureChanged:(id)sender {
       vNewFeature = self.newFeatureCheckbox.state == NSControlStateValueOn ? 1 : 0;
       [[NSUserDefaults standardUserDefaults] setInteger:vNewFeature forKey:@"NewFeature"];
   }
   ```

3. **Add to Engine (if needed):**
   ```cpp
   // In Engine.h
   extern int vNewFeature;
   
   // Use in Engine.cpp
   if (vNewFeature) {
       // Feature logic
   }
   ```

4. **Add to menu (if needed):**
   ```objc
   NSMenuItem *menuItem = [theMenu addItemWithTitle:@"New Feature" 
                                              action:@selector(onNewFeatureSelected) 
                                       keyEquivalent:@""];
   ```

### 3. Debugging

**Enable Debug Logging:**
```objc
// In XoneKey.mm or other files
#ifdef DEBUG
    NSLog(@"Debug message: %@", variable);
#endif
```

**Breakpoints:**
- Set breakpoints in Xcode
- Use conditional breakpoints
- Use exception breakpoints

**Instruments:**
- Time Profiler - Performance analysis
- Allocations - Memory usage
- Leaks - Memory leak detection

## Testing

### Unit Testing

**Create Test Target:**
1. File > New > Target
2. Choose "Unit Testing Bundle"
3. Add test cases

**Example Test:**
```objc
#import <XCTest/XCTest.h>
#import "XoneKeyManager.h"

@interface XoneKeyTests : XCTestCase
@end

@implementation XoneKeyTests

- (void)testEventTapInitialization {
    BOOL result = [XoneKeyManager initEventTap];
    XCTAssertTrue(result, @"Event tap should initialize");
    
    BOOL isInited = [XoneKeyManager isInited];
    XCTAssertTrue(isInited, @"Should be initialized");
    
    [XoneKeyManager stopEventTap];
}

@end
```

### Manual Testing Checklist

- [ ] Application launches correctly
- [ ] Menu bar icon appears
- [ ] Toggle Vietnamese/English works
- [ ] Input types work (Telex, VNI, etc.)
- [ ] Code tables work
- [ ] Hotkey switching works
- [ ] Macro feature works
- [ ] Convert tool works
- [ ] Settings persist after restart
- [ ] Auto-start works
- [ ] Update check works

## Debugging

### Common Issues

#### 1. Event Tap Not Working

**Symptoms:**
- Keyboard events not intercepted
- No Vietnamese input

**Debug Steps:**
1. Check Accessibility permission:
   ```objc
   BOOL enabled = MJAccessibilityIsEnabled();
   NSLog(@"Accessibility enabled: %@", enabled ? @"YES" : @"NO");
   ```

2. Check event tap creation:
   ```objc
   if (!eventTap) {
       NSLog(@"Event tap creation failed");
       // Check error
   }
   ```

3. Check event tap enabled:
   ```objc
   BOOL isEnabled = CGEventTapIsEnabled(eventTap);
   NSLog(@"Event tap enabled: %@", isEnabled ? @"YES" : @"NO");
   ```

#### 2. Memory Issues

**Symptoms:**
- App crashes
- High memory usage

**Debug Steps:**
1. Use Instruments Leaks tool
2. Check for retain cycles
3. Verify CFRelease calls
4. Check for NULL before operations

#### 3. Settings Not Persisting

**Symptoms:**
- Settings reset after restart
- Default values always used

**Debug Steps:**
1. Check NSUserDefaults:
   ```objc
   NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
   NSLog(@"InputMethod: %ld", [defaults integerForKey:@"InputMethod"]);
   ```

2. Verify save calls
3. Check key names match

## Best Practices

### 1. Code Style

- Follow Objective-C naming conventions
- Use meaningful variable names
- Add comments for complex logic
- Keep methods focused and short

### 2. Error Handling

- Always check return values
- Use NSError for error reporting
- Log errors appropriately
- Provide user feedback

### 3. Memory Management

- Follow Core Foundation rules
- Use ARC where possible
- Check for NULL before operations
- Release resources properly

### 4. Thread Safety

- Document thread-safety guarantees
- Use locks for shared state
- Use dispatch queues appropriately
- Avoid race conditions

### 5. Performance

- Avoid blocking main thread
- Use async operations for I/O
- Profile before optimizing
- Cache expensive operations

## Troubleshooting

### Build Issues

**Issue: "No such module"**
- Solution: Clean build folder (Shift+Cmd+K)
- Rebuild project

**Issue: Code signing errors**
- Solution: Configure signing in project settings
- Check certificates are valid

**Issue: Linker errors**
- Solution: Check framework search paths
- Verify all frameworks are linked

### Runtime Issues

**Issue: App crashes on launch**
- Check console for error messages
- Verify all required permissions
- Check Info.plist configuration

**Issue: Event tap not working**
- Verify Accessibility permission
- Check event tap creation succeeded
- Verify event tap is enabled

**Issue: Settings not saving**
- Check NSUserDefaults key names
- Verify save calls are executed
- Check for exceptions

## Additional Resources

- [Apple Event Tap Documentation](https://developer.apple.com/documentation/coregraphics/quartz_event_services)
- [NSUserDefaults Guide](https://developer.apple.com/documentation/foundation/nsuserdefaults)
- [Accessibility API](https://developer.apple.com/documentation/appkit/nsaccessibility)
- [macOS Human Interface Guidelines](https://developer.apple.com/design/human-interface-guidelines/macos)

## Support

For issues or questions:
- Check existing documentation
- Review code comments
- Contact development team
- Check GitHub issues

