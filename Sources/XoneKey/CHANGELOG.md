# Changelog

All notable changes to XoneKey will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Linux application support
- Cloud sync (optional)
- Custom themes
- Advanced text prediction
- Settings profiles
- Enhanced macro system with templates

## [1.0.4] - 2026-02-22
### Fixed
- Fixed window switching lag for improved focus detection.
- Updated internal resources and branding consistency.
- Synchronized updater with version-specific release naming.

## [1.0.2] - 2026-02-22

### 🎉 Stability & Performance Release

This release focuses on significant stability improvements, performance optimizations, and code robustness for the XoneKey application.

### Added

#### Windows Platform
- **Thread-Based Application Health Monitoring**
  - Completely refactored health monitor to use a dedicated background thread.
  - Watchdog mechanism that detects main thread freezes without blocking the UI.
  - Automatic recovery logic that attempts to re-initialize engine hooks if a hang is detected.

- **Performance Profiling and Logging**
  - Comprehensive logging system with multiple log levels (DEBUG, INFO, WARNING, ERROR)
  - Performance timer for tracking slow operations (>100ms)
  - Window event hook frequency tracking
  - System tray update success/failure tracking
  - Performance counters for key operations
  - Logs written to `%APPDATA%\XoneKey\xonekey_log.txt`
  - `PERF_TIMER(name)` macro for automatic performance tracking

- **Network Operation Improvements**
  - Replaced `URLDownloadToFile` with WinHttp API
  - Proper timeout control (configurable, default 30 seconds)
  - Better error handling and logging
  - Support for HTTP and HTTPS
  - Fallback to old method for compatibility

- **Toast Notifications**
  - Toast notification when switching typing mode via shortcut key
  - Consistent behavior with taskbar icon click
  - Shows "Đã bật chế độ gõ tiếng Việt" or "Đã tắt chế độ gõ tiếng Việt"

#### Engine (Cross-Platform)
- **EngineSafety.h** - New safety layer
  - `IsValidIndex()` - Validate array indices
  - `SafeGetTypingWord()` / `SafeSetTypingWord()` - Safe array access
  - `SafeGetKeyState()` / `SafeSetKeyState()` - Safe state management
  - `SafeIncrementIndex()` / `SafeDecrementIndex()` - Overflow protection
  - `IsIndexInWord()` - Bounds checking for word operations
  - `IsValidCharacterCode()` - Character code validation
  - `ENGINE_ASSERT()` - Debug assertions

- **Enhanced Safety in Core Functions**
  - `setKeyData()` - Enhanced bounds checking
  - `insertKey()` - Safe increment with overflow protection
  - `insertState()` - Safe write with bounds checking
  - `checkSpelling()` - Validate bounds for spelling operations
  - `getCharacterCode()` - Validate code table and array bounds
  - `findAndCalculateVowel()` - Bounds checking for vowel operations
  - `removeMark()` - Safe vowel operations
  - Delete key handling - Safe decrement with underflow protection

- **Macro System Improvements**
  - `initMacroMap()` - Input validation and size limits
  - `modifyCaseUnicode()` - Code table bounds validation
  - Buffer overflow prevention
  - Safe cursor management

#### macOS Platform
- **Thread Safety Improvements**
  - Thread-safe initialization with `NSLock`
  - Protection against race conditions in event tap initialization
  - Safe concurrent access to shared state

- **Memory Management Improvements**
  - Safe memory release patterns
  - NULL checking before `CFRelease`
  - Exception handling with `@try/@catch`
  - Proper cleanup in `stopEventTap()`

- **Error Handling Enhancements**
  - Enhanced event tap error handling
  - Network error handling with timeout (10s request, 30s resource)
  - Permission checking before operations
  - Better error messages and logging

- **Code Quality**
  - Refactored macros to inline functions for better type safety
  - Improved debuggability
  - Better code maintainability

#### Documentation
- **Comprehensive User Guides**
  - `win32/USER_GUIDE.md` - Complete Windows user guide
  - `win32/DEVELOPER_GUIDE.md` - Windows developer guide
  - `macOS/USER_GUIDE.md` - Complete macOS user guide
  - `macOS/DEVELOPER_GUIDE.md` - macOS developer guide

- **Technical Documentation**
  - `PROJECT_OVERVIEW.md` - High-level project overview
  - `engine/ENGINE_REVIEW_AND_IMPROVEMENTS.md` - Engine review
  - `engine/IMPROVEMENTS_SUMMARY.md` - Engine improvements summary
  - `engine/CROSS_PLATFORM_COMPATIBILITY.md` - Compatibility notes
  - `macOS/MACOS_REVIEW_AND_IMPROVEMENTS.md` - macOS review
  - `win32/XoneKey/CODE_REVIEW_AND_FEATURES.md` - Windows code review
  - `win32/XoneKey/IMPLEMENTATION_SUMMARY.md` - Implementation summary
  - `UI_UX_IMPROVEMENTS.md` - UI/UX improvement suggestions
  - `FEATURE_ROADMAP.md` - Feature roadmap

### Changed

#### Windows Platform
- **Thread Safety Improvements**
  - Added critical section for `getFrontMostAppExecuteName()` to prevent race conditions
  - Thread-safe process query operations
  - Proper initialization and cleanup of critical sections

- **Window Event Hook Performance**
  - Added throttling/debouncing for window change events
  - Minimum 100ms between window event processing
  - Prevents excessive processing on rapid window switches
  - Performance logging for window events

- **Network Operations**
  - `XoneKeyHelper::getContentOfUrl()` now uses `NetworkHelper::DownloadContent()`
  - Automatic fallback if WinHttp fails
  - Better error handling and timeout control

- **Update Check**
  - Moved to background thread to prevent UI blocking
  - Message handler (`WM_USER + 2020`) to show update dialog when check completes
  - Non-blocking main message loop

#### Engine
- **Code Modernization**
  - Macros refactored to inline functions (macOS)
  - Better type safety
  - Improved debuggability
  - Consistent error handling patterns

- **Cross-Platform Compatibility**
  - All safety improvements are platform-agnostic
  - Standard C++ only, no platform dependencies
  - Works on Windows, macOS, and Linux

### Fixed

#### Windows Platform
- **Critical Bug Fixes**
  - Fixed network timeout issue - `getContentOfUrl()` now has proper timeout handling
  - Fixed missing error handling in process query - `getFrontMostAppExecuteName()` now validates all operations
  - Fixed blocking network call in main thread - update check moved to background thread
  - Fixed missing toast notification when switching typing mode via shortcut key

- **Compatibility Fixes**
  - Changed `GetTempPath2` to `GetTempPath` for better compatibility
  - Fixed `std::ofstream` wide string path handling in `NetworkHelper`

#### Engine
- **Safety Fixes**
  - Fixed potential buffer overflows in array access
  - Fixed index underflow in delete key handling
  - Fixed invalid code table access crashes
  - Fixed memory leaks in macro loading
  - Fixed bounds checking issues in multiple functions

#### macOS Platform
- **Memory Management Fixes**
  - Fixed potential memory leaks in event tap cleanup
  - Fixed double-release issues in `stopEventTap()`
  - Fixed NULL pointer dereferences

- **Thread Safety Fixes**
  - Fixed race conditions in event tap initialization
  - Fixed concurrent access issues

### Security

- **Buffer Overflow Prevention**
  - All array access now has bounds checking
  - Index validation before use
  - Safe increment/decrement operations

- **Input Validation**
  - Validate all external inputs
  - Size limits for data
  - Safe defaults when invalid

- **Memory Safety**
  - Bounds checking for memory operations
  - Safe array access patterns
  - Overflow/underflow protection

### Performance

- **Optimizations**
  - Window event throttling reduces CPU usage
  - Efficient event handling
  - Minimal memory footprint
  - Fast lookup tables
  - Cached operations

- **Monitoring**
  - Performance timers
  - Slow operation detection
  - Event frequency tracking
  - Resource usage monitoring

### Removed

- Removed obsolete `win32/CodeReview.md` (replaced by `CODE_REVIEW_AND_FEATURES.md`)
- Removed Visual Studio user-specific files (`.vcxproj.user`)

## [1.0.1] - 2019-09-19

### Added
- Initial Windows release
- Basic Vietnamese input functionality
- System tray integration
- Auto-start with Windows

## [1.0.0] - 2019-02-01

### Added
- Initial macOS release
- Basic Vietnamese input functionality
- Menu bar integration
- Auto-start with login items

---

## Release Notes

### Version 1.0.2 Highlights

This is a major stability and quality release. Key improvements include:

1. **Stability**: Fixed critical bugs that could cause application hangs
2. **Performance**: Added monitoring and throttling to prevent performance issues
3. **Safety**: Comprehensive bounds checking and input validation
4. **Quality**: Better error handling, logging, and diagnostics
5. **Documentation**: Complete guides for users and developers

### Migration Notes

- **Windows**: No breaking changes. Existing settings and configurations are preserved.
- **macOS**: No breaking changes. Existing settings and configurations are preserved.
- **Engine**: All improvements are backward compatible.

### Known Issues

- Linux support is still in development
- Some advanced features (cloud sync, themes) are planned for future releases

### Contributors

Special thanks to all contributors who helped improve XoneKey!

---

**For detailed information about specific changes, please refer to:**
- `win32/XoneKey/CODE_REVIEW_AND_FEATURES.md` - Windows improvements
- `engine/IMPROVEMENTS_SUMMARY.md` - Engine improvements
- `macOS/MACOS_REVIEW_AND_IMPROVEMENTS.md` - macOS improvements
- `PROJECT_OVERVIEW.md` - Overall project information

---

[Unreleased]: https://github.com/xonevn-ai/xonekey/compare/v1.0.2...HEAD
[1.0.2]: https://github.com/xonevn-ai/xonekey/compare/v1.0.1...v1.0.2
[1.0.1]: https://github.com/xonevn-ai/xonekey/compare/v1.0.0...v1.0.1
[1.0.0]: https://github.com/xonevn-ai/xonekey/releases/tag/v1.0.0

