# XoneKey Windows - Developer Guide

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

XoneKey là ứng dụng gõ tiếng Việt cho Windows, sử dụng Windows Hooks API để intercept keyboard events và xử lý qua engine C++.

### Requirements
- Windows Vista hoặc mới hơn (khuyến nghị Windows 10/11)
- Visual Studio 2017 hoặc mới hơn
- Windows SDK 10.0+
- C++ compiler với C++14 support
- Kiến thức về Win32 API và C++

### Key Features
- Vietnamese input method (Telex, VNI, Simple Telex)
- Multiple code tables (Unicode, TCVN3, VNI Windows, etc.)
- Macro support
- Smart switch key
- Code conversion tool
- Auto-update mechanism
- Application health monitoring
- Performance logging

## Architecture

### High-Level Architecture

```
┌─────────────────────────────────────┐
│         XoneKey.exe                 │
│  ┌───────────────────────────────┐  │
│  │      AppDelegate.cpp          │  │
│  │  - Application lifecycle      │  │
│  │  - Message loop               │  │
│  │  - Dialog management          │  │
│  └───────────────────────────────┘  │
│  ┌───────────────────────────────┐  │
│  │     MainControlDialog.cpp     │  │
│  │  - Settings UI                │  │
│  │  - User preferences           │  │
│  └───────────────────────────────┘  │
│  ┌───────────────────────────────┐  │
│  │     XoneKeyManager.cpp        │  │
│  │  - Engine initialization      │  │
│  │  - Hook management           │  │
│  └───────────────────────────────┘  │
│  ┌───────────────────────────────┐  │
│  │      XoneKey.cpp              │  │
│  │  - Keyboard hook callback     │  │
│  │  - Event processing           │  │
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
   - Handles message loop
   - Coordinates dialogs
   - Manages system tray

2. **XoneKeyManager** - Core functionality manager
   - Initializes engine
   - Manages keyboard hooks
   - Provides utility functions
   - Handles update checking

3. **XoneKey** - Event handling
   - Windows Hook callback implementation
   - Processes keyboard events
   - Sends processed events back

4. **MainControlDialog** - Settings UI
   - User preferences interface
   - Settings validation
   - UI state management

5. **SystemTrayHelper** - System tray management
   - Icon management
   - Context menu
   - Status updates

## Setup & Build

### Prerequisites

1. **Install Visual Studio**
   - Visual Studio 2017 hoặc mới hơn
   - C++ Desktop Development workload
   - Windows 10 SDK

2. **Install Windows SDK**
   - Windows SDK 10.0 hoặc mới hơn
   - Thường đi kèm với Visual Studio

3. **Clone Repository**
   ```bash
   git clone <repository-url>
   cd XoneKey/Sources/XoneKey/win32
   ```

### Build Configuration

1. **Open Solution**
   ```bash
   # Double-click hoặc
   start XoneKey/XoneKey.sln
   ```

2. **Select Configuration**
   - **Debug** - For development and debugging
   - **Release** - For production builds
   - **Platform:** x64 (khuyến nghị) hoặc Win32

3. **Build Settings**
   - C++ Language Standard: C++14 hoặc mới hơn
   - Character Set: Unicode
   - Runtime Library: Multi-threaded (/MT) cho Release

4. **Build**
   ```bash
   # In Visual Studio: Build > Build Solution (Ctrl+Shift+B)
   # Or command line:
   msbuild XoneKey.sln /p:Configuration=Release /p:Platform=x64
   ```

### Dependencies

- **System Libraries:**
  - user32.lib - Windows user interface
  - kernel32.lib - Windows kernel functions
  - winhttp.lib - HTTP operations
  - shell32.lib - Shell operations
  - gdi32.lib - Graphics device interface

- **Engine (C++):**
  - Shared engine code in `../../engine/`
  - Platform-specific headers in `engine/platforms/win32.h`

## Code Structure

### Directory Structure

```
win32/
├── XoneKey/
│   ├── XoneKey/              # Main application
│   │   ├── AppDelegate.cpp/h
│   │   ├── XoneKey.cpp/h
│   │   ├── XoneKeyManager.cpp/h
│   │   ├── MainControlDialog.cpp/h
│   │   ├── SystemTrayHelper.cpp/h
│   │   ├── ApplicationHealthMonitor.cpp/h
│   │   ├── PerformanceLogger.cpp/h
│   │   ├── NetworkHelper.cpp/h
│   │   ├── ToastNotification.cpp/h
│   │   └── ...
│   ├── XoneKeyUpdate/        # Update helper
│   └── XoneKey.sln           # Solution file
└── README.md
```

### Key Files

#### AppDelegate.cpp
- **Purpose:** Main application controller
- **Key Methods:**
  - `run()` - Application entry point, message loop
  - `createMainDialog()` - Create settings dialog
  - `checkUpdate()` - Check for updates
  - `onXoneKeyExit()` - Cleanup on exit

#### XoneKeyManager.cpp
- **Purpose:** Core functionality manager
- **Key Methods:**
  - `initEngine()` - Initialize C++ engine
  - `checkUpdate()` - Check for new version
  - `getContentOfUrl()` - Download content from URL

#### XoneKey.cpp
- **Purpose:** Event handling
- **Key Functions:**
  - `LowLevelKeyboardProc()` - Keyboard hook callback
  - `winEventProcCallback()` - Window event hook callback
  - `switchLanguage()` - Toggle Vietnamese/English

#### MainControlDialog.cpp
- **Purpose:** Settings UI
- **Key Methods:**
  - `fillData()` - Load and display settings
  - `saveData()` - Save settings to registry
  - Event handlers for UI controls

## Key Components

### 1. Keyboard Hook System

**Purpose:** Intercept global keyboard events

**Implementation:**
```cpp
// In XoneKey.cpp
HHOOK keyboardHook = SetWindowsHookEx(
    WH_KEYBOARD_LL,           // Low-level keyboard hook
    LowLevelKeyboardProc,     // Callback function
    GetModuleHandle(NULL),    // Module handle
    0                         // Thread ID (0 = all threads)
);
```

**Event Types:**
- `WM_KEYDOWN` - Key press
- `WM_KEYUP` - Key release
- `WM_SYSKEYDOWN` - System key press

### 2. Engine Integration

**Bridge Pattern:**
```cpp
// C++ engine functions
extern "C" {
    void* vKeyInit();
    void vKeyHandleEvent(...);
    // ... other engine functions
}

// Global variables shared between C++ and Win32
extern int vLanguage;
extern int vInputType;
extern int vCodeTable;
// ... other global variables
```

**Global Variables:**
- Defined in `AppDelegate.cpp`
- Shared between Win32 code and C++ engine
- Persisted in Windows Registry

### 3. System Tray Integration

**System Tray Icon:**
```cpp
// In SystemTrayHelper.cpp
NOTIFYICONDATA nid;
nid.hWnd = hWnd;
nid.uID = TRAY_ICON_ID;
nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
nid.hIcon = LoadIcon(...);
Shell_NotifyIcon(NIM_ADD, &nid);
```

**Context Menu:**
- Toggle Vietnamese/English
- Input type selection
- Code table selection
- Tools (Convert, Macro)
- Settings
- About
- Exit

### 4. Settings Persistence

**Registry Keys:**
- `HKEY_CURRENT_USER\Software\XoneKey\InputMethod` - Vietnamese/English (0/1)
- `HKEY_CURRENT_USER\Software\XoneKey\InputType` - Telex/VNI/Simple Telex (0-3)
- `HKEY_CURRENT_USER\Software\XoneKey\CodeTable` - Unicode/TCVN3/VNI (0-4)
- `HKEY_CURRENT_USER\Software\XoneKey\SwitchKeyStatus` - Hotkey configuration
- ... many more

**Loading:**
```cpp
// Helper macro
APP_GET_DATA(vLanguage, 1);  // Get with default value
```

**Saving:**
```cpp
// Helper macro
APP_SET_DATA("InputMethod", vLanguage);
```

### 5. Application Health Monitoring

**Purpose:** Detect and recover from hangs

**Implementation:**
```cpp
// In AppDelegate.cpp
ApplicationHealthMonitor::GetInstance()->StartMonitoring();

// In message loop
ApplicationHealthMonitor::GetInstance()->SignalHeartbeat();
```

**Features:**
- Heartbeat timer (30 seconds)
- Watchdog mechanism
- Automatic logging
- Uptime tracking

### 6. Performance Logging

**Purpose:** Track performance and debug issues

**Implementation:**
```cpp
// Logging
PerformanceLogger::LogInfo("Message");
PerformanceLogger::LogWarning("Warning message");
PerformanceLogger::LogError("Error message");

// Performance timing
PERF_TIMER("OperationName") {
    // Code to measure
}
```

**Log Location:**
- `%APPDATA%\XoneKey\xonekey_log.txt`

## Development Workflow

### 1. Making Changes

**Typical Workflow:**
1. Make code changes
2. Build project (Ctrl+Shift+B)
3. Run in debugger (F5)
4. Test functionality
5. Fix issues
6. Commit changes

### 2. Adding New Features

**Example: Adding a new setting**

1. **Add to AppDelegate.cpp:**
   ```cpp
   int vNewFeature = 0;
   ```

2. **Add to MainControlDialog.cpp:**
   ```cpp
   // UI control
   HWND hNewFeatureCheckbox = GetDlgItem(hDlg, IDC_CHECK_NEW_FEATURE);
   
   // Load value
   APP_GET_DATA(vNewFeature, 0);
   CheckDlgButton(hDlg, IDC_CHECK_NEW_FEATURE, vNewFeature ? BST_CHECKED : BST_UNCHECKED);
   
   // Save value
   case IDC_CHECK_NEW_FEATURE:
       vNewFeature = IsDlgButtonChecked(hDlg, IDC_CHECK_NEW_FEATURE) == BST_CHECKED ? 1 : 0;
       APP_SET_DATA("NewFeature", vNewFeature);
       break;
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

4. **Add to resource.h:**
   ```cpp
   #define IDC_CHECK_NEW_FEATURE 1001
   ```

5. **Add to dialog resource:**
   - Open `.rc` file
   - Add checkbox control

### 3. Debugging

**Enable Debug Logging:**
```cpp
// In any file
PerformanceLogger::LogDebug("Debug message");
```

**Breakpoints:**
- Set breakpoints in Visual Studio
- Use conditional breakpoints
- Use exception breakpoints

**Output Window:**
- View debug output in Visual Studio Output window
- Check log file: `%APPDATA%\XoneKey\xonekey_log.txt`

## Testing

### Unit Testing

**Create Test Project:**
1. File > New > Project
2. Choose "Native Unit Test Project"
3. Add test cases

**Example Test:**
```cpp
#include <CppUnitTest.h>
#include "XoneKeyManager.h"

TEST_CLASS(XoneKeyTests) {
public:
    TEST_METHOD(TestEngineInitialization) {
        bool result = XoneKeyManager::initEngine();
        Assert::IsTrue(result, L"Engine should initialize");
    }
};
```

### Manual Testing Checklist

- [ ] Application launches correctly
- [ ] System tray icon appears
- [ ] Toggle Vietnamese/English works
- [ ] Input types work (Telex, VNI, etc.)
- [ ] Code tables work
- [ ] Hotkey switching works
- [ ] Macro feature works
- [ ] Convert tool works
- [ ] Settings persist after restart
- [ ] Auto-start works
- [ ] Update check works
- [ ] Health monitoring works
- [ ] Performance logging works

## Debugging

### Common Issues

#### 1. Hook Not Working

**Symptoms:**
- Keyboard events not intercepted
- No Vietnamese input

**Debug Steps:**
1. Check hook installation:
   ```cpp
   if (keyboardHook == NULL) {
       DWORD error = GetLastError();
       // Log error
   }
   ```

2. Check hook callback:
   - Set breakpoint in `LowLevelKeyboardProc`
   - Verify callback is called

3. Check permissions:
   - Ensure running as Administrator
   - Check UAC settings

#### 2. Memory Issues

**Symptoms:**
- App crashes
- High memory usage

**Debug Steps:**
1. Use Visual Studio Diagnostic Tools
2. Check for memory leaks
3. Verify proper cleanup
4. Check for buffer overflows

#### 3. Settings Not Persisting

**Symptoms:**
- Settings reset after restart
- Default values always used

**Debug Steps:**
1. Check registry:
   ```cpp
   // Use Registry Editor
   // HKEY_CURRENT_USER\Software\XoneKey
   ```

2. Verify save calls
3. Check key names match

#### 4. Update Check Issues

**Symptoms:**
- Update check hangs
- Network errors

**Debug Steps:**
1. Check network connectivity
2. Verify URL is accessible
3. Check timeout settings
4. Review NetworkHelper logs

## Best Practices

### 1. Code Style

- Follow Windows naming conventions
- Use meaningful variable names
- Add comments for complex logic
- Keep functions focused and short

### 2. Error Handling

- Always check return values
- Use GetLastError() for Windows API errors
- Log errors appropriately
- Provide user feedback

### 3. Memory Management

- Follow RAII patterns where possible
- Check for NULL before operations
- Release resources properly
- Use smart pointers where appropriate

### 4. Thread Safety

- Document thread-safety guarantees
- Use critical sections for shared state
- Avoid race conditions
- Use proper synchronization

### 5. Performance

- Avoid blocking main thread
- Use async operations for I/O
- Profile before optimizing
- Cache expensive operations

### 6. Security

- Validate all inputs
- Use secure network connections
- Check permissions before operations
- Don't hardcode sensitive data

## Troubleshooting

### Build Issues

**Issue: "Cannot open include file"**
- Solution: Check include paths in project settings
- Verify Windows SDK is installed

**Issue: Linker errors**
- Solution: Check library paths
- Verify all required libraries are linked

**Issue: Code signing errors**
- Solution: Configure certificates in project settings
- Or disable code signing for development

### Runtime Issues

**Issue: App crashes on launch**
- Check Event Viewer for error messages
- Verify all required DLLs are present
- Check for missing dependencies

**Issue: Hook not working**
- Verify running as Administrator
- Check hook installation succeeded
- Verify hook callback is registered

**Issue: Settings not saving**
- Check registry permissions
- Verify save calls are executed
- Check for exceptions

## Additional Resources

- [Windows API Documentation](https://docs.microsoft.com/en-us/windows/win32/api/)
- [Win32 Programming Guide](https://docs.microsoft.com/en-us/windows/win32/learnwin32/)
- [Windows Hooks](https://docs.microsoft.com/en-us/windows/win32/winmsg/hooks)
- [Registry Operations](https://docs.microsoft.com/en-us/windows/win32/sysinfo/registry)

## Support

For issues or questions:
- Check existing documentation
- Review code comments
- Check GitHub Issues
- Contact development team

## Contributing

We welcome contributions! Please:
1. Fork repository
2. Create feature branch
3. Make changes
4. Test thoroughly
5. Submit Pull Request

---

**Happy coding! 🚀**

