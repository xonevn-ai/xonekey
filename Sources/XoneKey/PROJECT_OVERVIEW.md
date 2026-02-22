# XoneKey - Project Overview

## Giới Thiệu

XoneKey là một ứng dụng gõ tiếng Việt mã nguồn mở, cross-platform, được thiết kế để cung cấp trải nghiệm gõ tiếng Việt tốt nhất trên nhiều hệ điều hành.

### Lịch Sử

- **18/01/2019** - Dự án XoneKey ra đời
- **01/02/2019** - Phiên bản đầu tiên cho macOS
- **31/07/2019** - Được biết đến rộng rãi qua tinhte.vn
- **19/09/2019** - Phiên bản đầu tiên cho Windows
- **2025** - Version 2.0+ với nhiều cải thiện về stability, performance, và features

### Mục Tiêu

- Cung cấp bộ gõ tiếng Việt miễn phí, mã nguồn mở
- Hoạt động mượt mà trên mọi ứng dụng, kể cả game
- Hỗ trợ nhiều kiểu gõ và bảng mã
- Cross-platform compatibility (macOS, Windows, Linux)
- Performance cao, ổn định, dễ sử dụng

## Kiến Trúc Tổng Quan

### High-Level Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    XoneKey Applications                   │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  │
│  │   macOS      │  │   Windows    │  │   Linux      │  │
│  │   (Cocoa)    │  │   (Win32)    │  │   (GTK/Qt)   │  │
│  └──────────────┘  └──────────────┘  └──────────────┘  │
│         │                  │                  │         │
│         └──────────────────┼──────────────────┘         │
│                            │                             │
│                            ▼                             │
│  ┌──────────────────────────────────────────────────┐   │
│  │         Shared Engine (C++)                      │   │
│  │  ┌────────────────────────────────────────────┐  │   │
│  │  │  Engine.cpp/h - Core typing logic         │  │   │
│  │  │  Vietnamese.cpp/h - Vietnamese rules      │  │   │
│  │  │  Macro.cpp/h - Macro handling             │  │   │
│  │  │  SmartSwitchKey.cpp/h - Auto switch        │  │   │
│  │  │  ConvertTool.cpp/h - Code conversion       │  │   │
│  │  │  EngineSafety.h - Safety helpers           │  │   │
│  │  └────────────────────────────────────────────┘  │   │
│  │  ┌────────────────────────────────────────────┐  │   │
│  │  │  platforms/ - Platform-specific headers     │  │   │
│  │  │    - win32.h, mac.h, linux.h              │  │   │
│  │  └────────────────────────────────────────────┘  │   │
│  └──────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────┘
```

### Core Principles

1. **Shared Engine** - Logic gõ tiếng Việt được chia sẻ giữa tất cả platforms
2. **Platform Abstraction** - Platform-specific code được isolate trong `platforms/`
3. **Cross-Platform Compatibility** - Engine hoàn toàn portable, không phụ thuộc platform
4. **Modular Design** - Mỗi component có trách nhiệm rõ ràng

## Cấu Trúc Dự Án

### Directory Structure

```
XoneKey/
├── engine/                    # Shared C++ engine
│   ├── Engine.cpp/h           # Core typing engine
│   ├── Vietnamese.cpp/h       # Vietnamese language rules
│   ├── Macro.cpp/h            # Macro functionality
│   ├── SmartSwitchKey.cpp/h   # Smart switch key
│   ├── ConvertTool.cpp/h      # Code conversion
│   ├── EngineSafety.h         # Safety helpers
│   ├── DataType.h             # Common data types
│   └── platforms/              # Platform-specific headers
│       ├── win32.h
│       ├── mac.h
│       └── linux.h
│
├── macOS/                      # macOS application
│   ├── ModernKey/             # Main app
│   ├── XoneKeyHelper/         # Login item helper
│   ├── XoneKey.xcodeproj/     # Xcode project
│   ├── DEVELOPER_GUIDE.md
│   ├── USER_GUIDE.md
│   └── MACOS_REVIEW_AND_IMPROVEMENTS.md
│
├── win32/                      # Windows application
│   ├── XoneKey/               # Main app
│   │   ├── XoneKey/          # Application code
│   │   └── XoneKeyUpdate/    # Update helper
│   ├── XoneKey.sln            # Visual Studio solution
│   ├── README.md
│   ├── DEVELOPER_GUIDE.md
│   └── USER_GUIDE.md
│
├── linux/                      # Linux application (in development)
│   └── README.md
│
└── icons/                      # Application icons
```

## Components

### 1. Engine (Shared C++)

**Location:** `engine/`

**Purpose:** Core Vietnamese typing logic, shared across all platforms

**Key Components:**
- **Engine.cpp/h** - Main typing engine, handles key events
- **Vietnamese.cpp/h** - Vietnamese language rules, vowel/consonant handling
- **Macro.cpp/h** - Macro (gõ tắt) functionality
- **SmartSwitchKey.cpp/h** - Automatic language switching based on application
- **ConvertTool.cpp/h** - Code table conversion utilities
- **EngineSafety.h** - Safety helpers with bounds checking

**Features:**
- ✅ Cross-platform compatible (Windows, macOS, Linux)
- ✅ Bounds checking và error handling
- ✅ Thread-safe operations
- ✅ Memory-safe operations
- ✅ Performance optimized

**Platform Abstraction:**
- Key codes abstracted through `platforms/` headers
- No platform-specific code in engine
- Pure C++ standard library

### 2. macOS Application

**Location:** `macOS/`

**Technology Stack:**
- Objective-C / Objective-C++
- Cocoa framework
- CGEventTap API
- NSStatusItem for menu bar

**Key Components:**
- **AppDelegate.m** - Application lifecycle, menu bar
- **ViewController.m** - Settings UI
- **XoneKeyManager.m** - Event tap management
- **XoneKey.mm** - Event handling bridge (C++)

**Features:**
- ✅ Menu bar integration
- ✅ Accessibility API integration
- ✅ Auto-start with login items
- ✅ Update mechanism
- ✅ Thread-safe initialization
- ✅ Memory-safe operations

**Requirements:**
- macOS 10.13+ (High Sierra or later)
- Accessibility permission

### 3. Windows Application

**Location:** `win32/`

**Technology Stack:**
- C++ / Win32 API
- Windows Hooks API
- System Tray API
- Registry for settings

**Key Components:**
- **AppDelegate.cpp** - Application lifecycle, message loop
- **XoneKey.cpp** - Keyboard hook callback
- **XoneKeyManager.cpp** - Engine initialization
- **MainControlDialog.cpp** - Settings UI
- **SystemTrayHelper.cpp** - System tray management

**Features:**
- ✅ System tray integration
- ✅ Windows Hooks for keyboard events
- ✅ Auto-start with Windows
- ✅ Administrator mode support (for games)
- ✅ Application health monitoring
- ✅ Performance logging
- ✅ Toast notifications

**Requirements:**
- Windows Vista or later (recommended Windows 10/11)
- Administrator privileges (recommended)

### 4. Linux Application

**Location:** `linux/`

**Status:** In development

**Planned Technology:**
- C++ / GTK or Qt
- X11 or Wayland input handling

## Tính Năng Chính

### Input Methods (Kiểu Gõ)

1. **Telex** (Mặc định)
   - Phổ biến nhất
   - `aa` → `â`, `oo` → `ô`, `ee` → `ê`
   - `s` → sắc, `f` → huyền, `r` → hỏi, `x` → ngã, `j` → nặng

2. **VNI**
   - Kiểu gõ số
   - `a1` → `á`, `a2` → `à`, `a6` → `â`

3. **Simple Telex 1 & 2**
   - Telex đơn giản hóa
   - Phù hợp cho người mới

### Code Tables (Bảng Mã)

1. **Unicode dựng sẵn** (Mặc định)
   - Chuẩn Unicode phổ biến
   - Hỗ trợ tốt trên mọi ứng dụng

2. **TCVN3 (ABC)**
   - Bảng mã cũ, 1 byte
   - Tương thích với hệ thống cũ

3. **VNI Windows**
   - Bảng mã VNI
   - Tương thích với font VNI

4. **Unicode Tổ hợp**
   - Unicode với combining marks

5. **Vietnamese Locale CP 1258**
   - Windows-1258 encoding

### Advanced Features

1. **Spelling Check**
   - Tự động kiểm tra chính tả
   - Tự động sửa lỗi
   - Khôi phục khi sai

2. **Macro (Gõ Tắt)**
   - Tạo shortcuts cho cụm từ dài
   - Hỗ trợ auto-caps
   - Hoạt động trong cả English mode

3. **Smart Switch Key**
   - Tự động chuyển đổi ngôn ngữ theo ứng dụng
   - Nhớ bảng mã cho từng ứng dụng
   - Configurable app rules

4. **Quick Telex**
   - Gõ nhanh: `cc` → `ch`, `gg` → `gi`, `kk` → `kh`

5. **Quick Consonant**
   - Gõ nhanh phụ âm đầu/cuối
   - `f` → `ph`, `j` → `gi`, `w` → `qu`

6. **Code Conversion**
   - Chuyển đổi giữa các bảng mã
   - Quick convert từ clipboard
   - Batch conversion

7. **Toast Notifications** (Windows)
   - Thông báo khi chuyển đổi ngôn ngữ
   - User-friendly feedback

## Technical Highlights

### Cross-Platform Engine

**Design:**
- Pure C++ standard library
- No platform dependencies
- Platform abstraction layer
- Shared codebase

**Benefits:**
- Single source of truth
- Consistent behavior across platforms
- Easier maintenance
- Faster feature development

### Safety & Reliability

**Engine Safety:**
- Comprehensive bounds checking
- Input validation
- Overflow/underflow protection
- Safe array access patterns

**Application Monitoring:**
- Health monitoring (watchdog)
- Performance logging
- Error tracking
- Automatic recovery

### Performance

**Optimizations:**
- Efficient event handling
- Minimal memory footprint
- Fast lookup tables
- Cached operations

**Monitoring:**
- Performance timers
- Slow operation detection
- Event frequency tracking
- Resource usage monitoring

### Code Quality

**Best Practices:**
- Thread safety
- Memory safety
- Error handling
- Input validation
- Documentation

**Improvements:**
- Macros → inline functions
- Better error handling
- Enhanced logging
- Comprehensive documentation

## Platform-Specific Details

### macOS

**APIs Used:**
- CGEventTap - Keyboard event interception
- NSStatusItem - Menu bar icon
- NSUserDefaults - Settings persistence
- ServiceManagement - Login items
- Accessibility API - Permission checking

**Key Features:**
- Menu bar integration
- Accessibility permission handling
- Auto-start with login items
- Update mechanism

### Windows

**APIs Used:**
- Windows Hooks (WH_KEYBOARD_LL) - Keyboard events
- System Tray API - Tray icon
- Registry - Settings persistence
- WinHttp - Network operations
- Toast Notifications - User feedback

**Key Features:**
- System tray integration
- Administrator mode
- Health monitoring
- Performance logging
- Toast notifications

### Linux

**Status:** In development

**Planned APIs:**
- X11/Wayland input handling
- GTK or Qt for UI
- Systemd for auto-start

## Development Workflow

### Adding New Features

1. **Engine Changes:**
   - Modify engine code in `engine/`
   - Ensure cross-platform compatibility
   - Add tests if applicable

2. **Platform Integration:**
   - Add platform-specific code if needed
   - Update UI if required
   - Test on target platform

3. **Documentation:**
   - Update relevant guides
   - Add code comments
   - Update changelog

### Testing

**Unit Tests:**
- Engine logic
- Utility functions
- Edge cases

**Integration Tests:**
- Full application flow
- Cross-platform compatibility
- Performance benchmarks

**Manual Testing:**
- User scenarios
- Different applications
- Edge cases

## Documentation

### User Documentation

- **README.md** - Project overview và quick start
- **USER_GUIDE.md** - Hướng dẫn sử dụng chi tiết
  - Installation
  - Basic usage
  - Features
  - Troubleshooting
  - FAQ

### Developer Documentation

- **DEVELOPER_GUIDE.md** - Hướng dẫn phát triển
  - Architecture
  - Setup & build
  - Code structure
  - Development workflow
  - Best practices

### Technical Documentation

- **ENGINE_REVIEW_AND_IMPROVEMENTS.md** - Engine review
- **MACOS_REVIEW_AND_IMPROVEMENTS.md** - macOS review
- **CROSS_PLATFORM_COMPATIBILITY.md** - Compatibility notes
- **IMPROVEMENTS_SUMMARY.md** - Summary of improvements

## Statistics

### Codebase

- **Total Lines of Code:** ~15,000+ lines
- **Engine:** ~5,000 lines (C++)
- **macOS:** ~3,000 lines (Objective-C/C++)
- **Windows:** ~4,000 lines (C++/Win32)
- **Documentation:** ~5,000+ lines (Markdown)

### Features

- **Input Methods:** 4 (Telex, VNI, Simple Telex 1 & 2)
- **Code Tables:** 5 (Unicode, TCVN3, VNI, etc.)
- **Platforms:** 3 (macOS, Windows, Linux)
- **Languages:** 3 (C++, Objective-C, Win32 C++)

### Improvements (Version 2.0+)

- **Safety Improvements:** 20+ functions enhanced
- **Thread Safety:** Critical sections added
- **Memory Management:** Leak prevention
- **Error Handling:** Comprehensive error handling
- **Documentation:** Complete guides for users and developers

## Roadmap

### Completed ✅

- ✅ Cross-platform engine
- ✅ macOS application
- ✅ Windows application
- ✅ Engine safety improvements
- ✅ Thread safety improvements
- ✅ Memory management improvements
- ✅ Comprehensive documentation
- ✅ Performance monitoring
- ✅ Health monitoring

### In Progress 🚧

- 🚧 Linux application
- 🚧 Additional input methods
- 🚧 Enhanced features

### Planned 📋

- 📋 Cloud sync (optional)
- 📋 Custom themes
- 📋 Plugin system
- 📋 Advanced analytics

## Contributing

### How to Contribute

1. **Fork Repository**
2. **Create Feature Branch**
3. **Make Changes**
4. **Test Thoroughly**
5. **Submit Pull Request**

### Areas for Contribution

- Bug fixes
- New features
- Documentation improvements
- Performance optimizations
- Cross-platform compatibility
- Testing

## License

XoneKey is released under the **GPL (GNU General Public License)**.

**What this means:**
- ✅ Free to use
- ✅ Free to modify
- ✅ Free to distribute
- ⚠️ If you distribute modified versions, you **must** publish source code and credit original XoneKey

## Support

### Getting Help

- **GitHub Issues:** [Report Issues](https://github.com/xonevn-ai/xonekey/issues)
- **Documentation:** See USER_GUIDE.md and DEVELOPER_GUIDE.md
- **Website:** [XoneKey Website]

### Reporting Bugs

- Describe issue in detail
- Include platform and version
- Steps to reproduce
- Screenshots if applicable

### Feature Requests

- Submit on GitHub Issues
- Describe desired feature
- Explain use cases

## Credits

**Developer:** Daniel Do (diep@xone.vn)

**Contributors:** See GitHub contributors list

**Special Thanks:**
- All users who provided feedback
- Contributors to the project
- Open source community

## Links

- **GitHub:** https://github.com/xonevn-ai/xonekey
- **Website:** [XoneKey Website]
- **Fanpage:** https://www.facebook.com/XoneKeyVN

## Conclusion

XoneKey là một dự án mã nguồn mở, cross-platform, được thiết kế để cung cấp trải nghiệm gõ tiếng Việt tốt nhất. Với kiến trúc modular, engine được chia sẻ, và code quality cao, XoneKey đang phát triển mạnh mẽ và sẵn sàng cho tương lai.

**Key Strengths:**
- ✅ Cross-platform compatibility
- ✅ Shared engine codebase
- ✅ High code quality
- ✅ Comprehensive documentation
- ✅ Active development
- ✅ Open source

**Future:**
- Linux support
- More features
- Better performance
- Enhanced user experience

---

**XoneKey - Gõ tiếng Việt, không giới hạn! 🇻🇳**

