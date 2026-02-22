# XoneKey

[![GitHub release](https://img.shields.io/github/v/release/vituocgia/XoneKey.svg)](https://github.com/vituocgia/XoneKey/releases/latest)
[![License: GPL](https://img.shields.io/badge/License-GPL-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

**XoneKey** - Bộ gõ tiếng Việt miễn phí, mã nguồn mở, cross-platform cho macOS, Windows, và Linux.

## 🌟 Giới Thiệu

XoneKey là ứng dụng gõ tiếng Việt được thiết kế để cung cấp trải nghiệm gõ tốt nhất trên mọi hệ điều hành. Với engine được chia sẻ giữa các platforms, XoneKey đảm bảo tính nhất quán và chất lượng cao.

### ✨ Tính Năng Nổi Bật

- ✅ **Nhiều kiểu gõ:** Telex, VNI, Simple Telex 1 & 2
- ✅ **Nhiều bảng mã:** Unicode, TCVN3, VNI Windows, Unicode Tổ hợp, CP 1258
- ✅ **Gõ tắt (Macro):** Tạo shortcuts cho cụm từ dài
- ✅ **Smart Switch Key:** Tự động chuyển đổi ngôn ngữ theo ứng dụng
- ✅ **Kiểm tra chính tả:** Tự động sửa lỗi chính tả
- ✅ **Chuyển mã:** Chuyển đổi giữa các bảng mã
- ✅ **Cross-platform:** macOS, Windows, Linux (in development)
- ✅ **Mã nguồn mở:** GPL license, hoàn toàn minh bạch

## 📦 Platforms

### 🍎 macOS
- **Status:** ✅ Stable
- **Requirements:** macOS 10.13+
- **Download:** [GitHub Releases](https://github.com/vituocgia/XoneKey/releases)
- **Documentation:** [macOS USER_GUIDE.md](macOS/USER_GUIDE.md) | [DEVELOPER_GUIDE.md](macOS/DEVELOPER_GUIDE.md)

### 🪟 Windows
- **Status:** ✅ Stable
- **Requirements:** Windows Vista+
- **Download:** [GitHub Releases](https://github.com/vituocgia/XoneKey/releases)
- **Documentation:** [Windows USER_GUIDE.md](win32/USER_GUIDE.md) | [DEVELOPER_GUIDE.md](win32/DEVELOPER_GUIDE.md)

### 🐧 Linux
- **Status:** 🚧 In Development
- **Documentation:** [Linux README.md](linux/README.md)

## 🚀 Quick Start

### macOS

1. Download từ [GitHub Releases](https://github.com/vituocgia/XoneKey/releases)
2. Mở `XoneKey.dmg` và kéo vào Applications
3. Mở XoneKey và cấp quyền Accessibility
4. Bắt đầu gõ tiếng Việt!

Xem [macOS USER_GUIDE.md](macOS/USER_GUIDE.md) để biết chi tiết.

### Windows

1. Download từ [GitHub Releases](https://github.com/vituocgia/XoneKey/releases)
2. Giải nén và chạy `XoneKey.exe`
3. Cho phép chạy với quyền Administrator
4. Bắt đầu gõ tiếng Việt!

Xem [Windows USER_GUIDE.md](win32/USER_GUIDE.md) để biết chi tiết.

## 📚 Documentation

### For Users
- **[PROJECT_OVERVIEW.md](PROJECT_OVERVIEW.md)** - Tổng quan dự án
- **[macOS USER_GUIDE.md](macOS/USER_GUIDE.md)** - Hướng dẫn sử dụng macOS
- **[Windows USER_GUIDE.md](win32/USER_GUIDE.md)** - Hướng dẫn sử dụng Windows

### For Developers
- **[macOS DEVELOPER_GUIDE.md](macOS/DEVELOPER_GUIDE.md)** - Hướng dẫn phát triển macOS
- **[Windows DEVELOPER_GUIDE.md](win32/DEVELOPER_GUIDE.md)** - Hướng dẫn phát triển Windows
- **[Engine Documentation](engine/ENGINE_REVIEW_AND_IMPROVEMENTS.md)** - Engine review và improvements

### Technical Documentation
- **[Engine Improvements](engine/IMPROVEMENTS_SUMMARY.md)** - Engine improvements summary
- **[Cross-Platform Compatibility](engine/CROSS_PLATFORM_COMPATIBILITY.md)** - Compatibility notes
- **[macOS Review](macOS/MACOS_REVIEW_AND_IMPROVEMENTS.md)** - macOS code review
- **[Windows Review](win32/XoneKey/CODE_REVIEW_AND_FEATURES.md)** - Windows code review

## 🏗️ Architecture

XoneKey sử dụng kiến trúc **shared engine** với platform-specific implementations:

```
┌─────────────────────────────────────┐
│   Platform Applications              │
│   (macOS / Windows / Linux)         │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│   Shared Engine (C++)                │
│   - Engine.cpp                       │
│   - Vietnamese.cpp                   │
│   - Macro.cpp                        │
│   - SmartSwitchKey.cpp               │
└─────────────────────────────────────┘
```

**Key Benefits:**
- ✅ Single source of truth
- ✅ Consistent behavior
- ✅ Easier maintenance
- ✅ Cross-platform compatibility

## 🛠️ Development

### Prerequisites

**macOS:**
- Xcode 12.0+
- macOS 10.13+

**Windows:**
- Visual Studio 2017+
- Windows SDK 10.0+

**All Platforms:**
- C++ compiler với C++14 support

### Build

**macOS:**
```bash
cd macOS
open XoneKey.xcodeproj
# Build in Xcode (Cmd+B)
```

**Windows:**
```bash
cd win32/XoneKey
start XoneKey.sln
# Build in Visual Studio (Ctrl+Shift+B)
```

Xem [DEVELOPER_GUIDE.md](macOS/DEVELOPER_GUIDE.md) hoặc [DEVELOPER_GUIDE.md](win32/DEVELOPER_GUIDE.md) để biết chi tiết.

## 📊 Project Statistics

- **Total Lines of Code:** ~15,000+
- **Engine:** ~5,000 lines (C++)
- **macOS:** ~3,000 lines (Objective-C/C++)
- **Windows:** ~4,000 lines (C++/Win32)
- **Documentation:** ~5,000+ lines

## 🎯 Features

### Input Methods
- **Telex** - Kiểu gõ phổ biến nhất
- **VNI** - Kiểu gõ số
- **Simple Telex 1 & 2** - Telex đơn giản hóa

### Code Tables
- **Unicode dựng sẵn** (Mặc định)
- **TCVN3 (ABC)**
- **VNI Windows**
- **Unicode Tổ hợp**
- **Vietnamese Locale CP 1258**

### Advanced Features
- Spelling check với auto-correction
- Macro (gõ tắt) với auto-caps
- Smart switch key (tự động chuyển đổi)
- Quick Telex (cc→ch, gg→gi, etc.)
- Quick Consonant (f→ph, j→gi, etc.)
- Code conversion tool
- Remember code per application
- Toast notifications (Windows)

## 🔧 Recent Improvements (v2.0+)

### Engine
- ✅ Comprehensive bounds checking
- ✅ Input validation
- ✅ Memory safety improvements
- ✅ Cross-platform compatibility verified

### macOS
- ✅ Thread-safe initialization
- ✅ Enhanced memory management
- ✅ Macros refactored to inline functions
- ✅ Better error handling

### Windows
- ✅ Application health monitoring
- ✅ Performance logging
- ✅ Network improvements với timeout
- ✅ Toast notifications
- ✅ Enhanced error handling

## 🤝 Contributing

Chúng tôi hoan nghênh mọi đóng góp! Vui lòng:

1. Fork repository
2. Tạo feature branch
3. Commit changes
4. Push và tạo Pull Request

Xem [DEVELOPER_GUIDE.md](macOS/DEVELOPER_GUIDE.md) để biết chi tiết về development workflow.

## 📝 License

XoneKey được phát hành dưới giấy phép **GPL (GNU General Public License)**.

**Điều này có nghĩa:**
- ✅ Bạn có thể tự do sử dụng
- ✅ Bạn có thể tự do chỉnh sửa mã nguồn
- ✅ Bạn có thể tự do phân phối
- ⚠️ Nếu bạn phân phối bản chỉnh sửa, bạn **phải** công bố mã nguồn và credit XoneKey bản gốc

## 💬 Support

- **GitHub Issues:** [Report Issues](https://github.com/vituocgia/XoneKey/issues)
- **Documentation:** Xem USER_GUIDE.md và DEVELOPER_GUIDE.md
- **Website:** [XoneKey Website]

## 🙏 Credits

**Developer:** Daniel Do (diep@xone.vn)

**Contributors:** See [GitHub Contributors](https://github.com/vituocgia/XoneKey/graphs/contributors)

## 📖 Learn More

- **[PROJECT_OVERVIEW.md](PROJECT_OVERVIEW.md)** - Tổng quan chi tiết về dự án
- **[macOS Documentation](macOS/)** - Tài liệu cho macOS
- **[Windows Documentation](win32/)** - Tài liệu cho Windows
- **[Engine Documentation](engine/)** - Tài liệu về engine

## 🌟 Star History

Nếu bạn thấy XoneKey hữu ích, hãy ⭐ star repository này!

---

**XoneKey - Gõ tiếng Việt, không giới hạn! 🇻🇳**

Made with ❤️ by Daniel Do and contributors

