# XoneKey Windows - User Guide

## Table of Contents
1. [Introduction](#introduction)
2. [Installation](#installation)
3. [First Time Setup](#first-time-setup)
4. [Basic Usage](#basic-usage)
5. [Features](#features)
6. [Settings](#settings)
7. [Troubleshooting](#troubleshooting)
8. [FAQ](#faq)

## Introduction

XoneKey là ứng dụng gõ tiếng Việt miễn phí cho Windows, hỗ trợ nhiều kiểu gõ và bảng mã khác nhau.

### Key Features
- ✅ Gõ tiếng Việt với nhiều kiểu gõ (Telex, VNI, Simple Telex)
- ✅ Hỗ trợ nhiều bảng mã (Unicode, TCVN3, VNI Windows, etc.)
- ✅ Chức năng gõ tắt (Macro)
- ✅ Chuyển đổi mã nhanh
- ✅ Tự động chuyển đổi ngôn ngữ thông minh
- ✅ Hoạt động với mọi ứng dụng, kể cả game

## Installation

### System Requirements
- Windows Vista hoặc mới hơn (khuyến nghị Windows 10/11)
- Quyền Administrator (khuyến nghị)

### Download & Install

1. **Download XoneKey**
   - Vào [GitHub Releases](https://github.com/vituocgia/XoneKey/releases/latest)
   - Tải file `.zip` phiên bản mới nhất

2. **Extract và Install**
   - Giải nén file `.zip` vào thư mục bất kỳ
   - Không cần cài đặt, chỉ cần chạy `XoneKey.exe`

3. **First Launch**
   - Double-click `XoneKey.exe`
   - Windows sẽ hỏi quyền Administrator
   - Click **Yes** để cho phép

## First Time Setup

### Step 1: Verify Installation

Sau khi chạy XoneKey, bạn sẽ thấy:
- Icon **V** (màu xanh) trên system tray = Tiếng Việt đang bật
- Icon **E** (màu xám) trên system tray = Tiếng Anh đang bật

### Step 2: Open Settings

**Cách 1: Double-click icon**
- Double-click icon trên system tray

**Cách 2: Right-click menu**
- Right-click icon > **Bảng điều khiển**

### Step 3: Configure Settings

1. **Chọn Kiểu Gõ:**
   - Settings > Primary tab > Input Type
   - Chọn: Telex, VNI, Simple Telex 1, hoặc Simple Telex 2

2. **Chọn Bảng Mã:**
   - Settings > Primary tab > Code Table
   - Khuyến nghị: **Unicode dựng sẵn**

3. **Cấu Hình Phím Tắt:**
   - Settings > Primary tab > Switch Key
   - Mặc định: **Alt + Z**
   - Có thể tùy chỉnh: Control, Alt, Shift, Win + Key

4. **Các Tùy Chọn Khác:**
   - Bật/tắt kiểm tra chính tả
   - Bật/tắt các tính năng nâng cao
   - Cấu hình auto-start

### Step 4: Test Input

1. Mở bất kỳ ứng dụng nào (Notepad, Word, etc.)
2. Đảm bảo icon system tray là chữ **V** màu xanh
3. Gõ thử: `xin chao` → sẽ thành `xin chào`

## Basic Usage

### Toggle Vietnamese/English

**Cách 1: Hotkey (Mặc định: Alt + Z)**
- Nhấn phím tắt để chuyển đổi nhanh
- Có thể tùy chỉnh trong Settings

**Cách 2: System Tray Menu**
- Right-click icon trên system tray
- Click **Bật Tiếng Việt** hoặc **Tắt Tiếng Việt**

**Cách 3: Settings Dialog**
- Mở Settings > Primary tab
- Toggle **Vietnamese/English** checkbox

### System Tray Icon

**Icon States:**
- 🟢 **V (Xanh)** = Tiếng Việt đang bật
- ⚪ **E (Xám)** = Tiếng Anh đang bật

**Right-Click Menu:**
- Bật Tiếng Việt / Tắt Tiếng Việt
- Kiểu gõ - Chọn Telex, VNI, Simple Telex
- Bảng mã - Chọn Unicode, TCVN3, VNI Windows, etc.
- Công cụ chuyển mã - Chuyển đổi mã văn bản
- Chuyển mã nhanh - Chuyển mã từ clipboard
- Bảng điều khiển - Mở Settings
- Gõ tắt - Quản lý macros
- Giới thiệu - Thông tin ứng dụng
- Thoát - Đóng ứng dụng

## Features

### 1. Input Methods (Kiểu Gõ)

#### Telex (Mặc định)
- `aa` → `â`
- `oo` → `ô`
- `ee` → `ê`
- `aw` → `ă`
- `ow` → `ơ`
- `uw` → `ư`
- `dd` → `đ`
- `s` → dấu sắc (á, é, í, ó, ú, ý)
- `f` → dấu huyền (à, è, ì, ò, ù, ỳ)
- `r` → dấu hỏi (ả, ẻ, ỉ, ỏ, ủ, ỷ)
- `x` → dấu ngã (ã, ẽ, ĩ, õ, ũ, ỹ)
- `j` → dấu nặng (ạ, ẹ, ị, ọ, ụ, ỵ)

**Ví dụ:**
- `xin chao` → `xin chào`
- `toi ten la Nam` → `tôi tên là Nam`
- `ban khoe khong` → `bạn khỏe không`

#### VNI
- `a1` → `á`
- `a2` → `à`
- `a3` → `ả`
- `a4` → `ã`
- `a5` → `ạ`
- `a6` → `â`
- `a61` → `ấ`
- `a62` → `ầ`
- `a63` → `ẩ`
- `a64` → `ẫ`
- `a65` → `ậ`
- `a8` → `ă`
- `a81` → `ắ`
- `a82` → `ằ`
- `a83` → `ẳ`
- `a84` → `ẵ`
- `a85` → `ặ`
- `d9` → `đ`

**Ví dụ:**
- `xin chao1` → `xin chào`
- `toi6 ten2 la8 Nam` → `tôi tên là Nam`

#### Simple Telex 1 & 2
- Tương tự Telex nhưng đơn giản hóa một số quy tắc
- Phù hợp cho người mới bắt đầu

### 2. Code Tables (Bảng Mã)

#### Unicode dựng sẵn (Mặc định)
- Chuẩn Unicode phổ biến nhất
- Hỗ trợ tốt trên mọi ứng dụng
- **Khuyến nghị sử dụng**

#### TCVN3 (ABC)
- Bảng mã cũ, 1 byte
- Dùng cho các hệ thống cũ
- Không khuyến nghị cho ứng dụng mới

#### VNI Windows
- Bảng mã VNI
- Tương thích với font VNI

#### Unicode tổ hợp
- Unicode với combining marks
- Một số ứng dụng có thể không hiển thị đúng

#### Vietnamese Locale CP 1258
- Bảng mã Windows-1258
- Ít được sử dụng

### 3. Smart Switch Key

Tự động chuyển đổi giữa Tiếng Việt và Tiếng Anh dựa trên ứng dụng đang dùng.

**Cách hoạt động:**
- XoneKey nhận diện ứng dụng frontmost
- Tự động chuyển sang Tiếng Anh cho các ứng dụng cần (Terminal, Code editors, etc.)
- Tự động chuyển về Tiếng Việt cho các ứng dụng khác

**Enable/Disable:**
- Settings > System > Smart Switch Key

### 4. Macro (Gõ Tắt)

Tạo shortcuts để gõ nhanh các cụm từ dài.

**Ví dụ:**
- `btw` → `by the way`
- `tc` → `trân trọng cảm ơn`
- `ks` → `kính thưa`

**Cách tạo Macro:**
1. System tray menu > **Gõ tắt**
   - Hoặc Settings > Macro tab
2. Click nút **Thêm** để thêm mới
3. Nhập từ viết tắt (ví dụ: `tc`)
4. Nhập nội dung đầy đủ (ví dụ: `trân trọng cảm ơn`)
5. Click **Lưu**

**Sử dụng:**
- Gõ từ viết tắt
- Nhấn Space hoặc Enter
- Tự động thay thế

### 5. Quick Convert (Chuyển Mã Nhanh)

Chuyển đổi mã văn bản từ clipboard.

**Cách dùng:**
1. Copy văn bản cần chuyển mã
2. System tray menu > **Chuyển mã nhanh**
   - Hoặc dùng hotkey (nếu đã cấu hình)
3. Văn bản đã chuyển mã sẽ được copy vào clipboard
4. Paste vào nơi cần dùng

**Hỗ trợ:**
- HTML text
- Plain text
- Tự động detect và convert

### 6. Toast Notifications

XoneKey hiển thị thông báo khi:
- Chuyển đổi ngôn ngữ bằng hotkey
- Chuyển đổi ngôn ngữ từ system tray menu

**Tùy chỉnh:**
- Có thể tắt trong Settings (nếu cần)

## Settings

### Mở Settings

**Cách 1: Double-click icon**
- Double-click icon XoneKey trên system tray

**Cách 2: Right-click menu**
- Right-click icon > **Bảng điều khiển**

**Cách 3: Hotkey**
- Có thể cấu hình hotkey trong Settings

### Settings Tabs

#### 1. Primary (Cơ Bản)

**Input Method:**
- Toggle Vietnamese/English
- Default: Vietnamese

**Input Type:**
- Telex
- VNI
- Simple Telex 1
- Simple Telex 2

**Code Table:**
- Unicode dựng sẵn
- TCVN3 (ABC)
- VNI Windows
- Unicode tổ hợp
- Vietnamese Locale CP 1258

**Switch Key:**
- Cấu hình hotkey để chuyển đổi ngôn ngữ
- Mặc định: Alt + Z
- Có thể tùy chỉnh: Control, Alt, Shift, Win + Key

**Beep Sound:**
- Phát tiếng beep khi chuyển đổi ngôn ngữ

#### 2. Advanced (Nâng Cao)

**Spelling Check:**
- Kiểm tra chính tả tiếng Việt
- Tự động sửa lỗi

**Restore If Wrong Spelling:**
- Tự động khôi phục nếu từ sai chính tả

**Quick Telex:**
- Gõ nhanh: `cc` → `ch`, `gg` → `gi`, etc.

**Quick Consonant:**
- Gõ nhanh phụ âm đầu/cuối

**Modern Orthography:**
- Sử dụng chính tả hiện đại
- Ví dụ: `òa` thay vì `oà`

**Fix Recommend Browser:**
- Sửa lỗi gợi ý trên thanh địa chỉ trình duyệt

**Fix Chromium Browser:**
- Sửa lỗi trên Chromium browsers

#### 3. Macro (Gõ Tắt)

**Use Macro:**
- Bật/tắt chức năng macro

**Use Macro In English Mode:**
- Cho phép dùng macro khi ở chế độ Tiếng Anh

**Auto Caps Macro:**
- Tự động viết hoa chữ cái đầu

**Macro Management:**
- Thêm, sửa, xóa macros

#### 4. System (Hệ Thống)

**Run On Startup:**
- Tự động chạy khi khởi động Windows

**Show Icon On Dock:**
- Hiển thị icon trên system tray

**Smart Switch Key:**
- Tự động chuyển đổi ngôn ngữ theo ứng dụng

**Remember Code:**
- Nhớ bảng mã cho từng ứng dụng

**Other Language:**
- Hỗ trợ ngôn ngữ khác

**Temp Off XoneKey:**
- Tạm thời tắt XoneKey (Alt key)

**Show UI On Startup:**
- Hiển thị Settings khi khởi động

**Check Update:**
- Tự động kiểm tra cập nhật

#### 5. Info (Thông Tin)

- Version information
- Build date
- Copyright

## Troubleshooting

### Issue: XoneKey không hoạt động

**Symptoms:**
- Gõ không ra tiếng Việt
- Icon system tray không xuất hiện

**Solutions:**
1. **Check Administrator Permission:**
   - Right-click `XoneKey.exe` > Run as Administrator
   - Hoặc cấu hình để luôn chạy với quyền Admin

2. **Restart XoneKey:**
   - Quit XoneKey (system tray > Thoát)
   - Mở lại từ file `.exe`

3. **Check if Running:**
   - Task Manager > Tìm XoneKey process
   - Nếu có nhiều instances, end tất cả và mở lại

### Issue: Không thể chạy với quyền Administrator

**Solutions:**
1. **Right-click Run as Administrator:**
   - Right-click `XoneKey.exe`
   - Chọn "Run as administrator"

2. **Configure Compatibility:**
   - Right-click `XoneKey.exe` > Properties
   - Compatibility tab > Tick "Run this program as an administrator"

### Issue: Gõ không đúng

**Symptoms:**
- Gõ `aa` không ra `â`
- Dấu không đúng

**Solutions:**
1. **Check Input Type:**
   - Settings > Primary > Input Type
   - Chọn đúng (Telex/VNI)

2. **Check Code Table:**
   - Settings > Primary > Code Table
   - Chọn Unicode dựng sẵn

3. **Check Spelling:**
   - Settings > Advanced > Spelling Check
   - Tắt nếu gây vấn đề

### Issue: Hotkey không hoạt động

**Solutions:**
1. **Check Conflict:**
   - Windows Settings > Keyboard > Shortcuts
   - Tìm xem có conflict không

2. **Reconfigure:**
   - Settings > Primary > Switch Key
   - Chọn lại hotkey khác

### Issue: App crash

**Solutions:**
1. **Check Log File:**
   - Tìm file log trong thư mục ứng dụng
   - Xem error messages

2. **Reset Settings:**
   - Quit XoneKey
   - Xóa registry keys (HKEY_CURRENT_USER\Software\XoneKey)
   - Mở lại XoneKey

3. **Reinstall:**
   - Quit XoneKey
   - Xóa thư mục ứng dụng
   - Download và cài đặt lại

### Issue: Không hoạt động trong game

**Solutions:**
1. **Run as Administrator:**
   - Đảm bảo XoneKey chạy với quyền Admin
   - Game cũng nên chạy với quyền Admin

2. **Check Game Compatibility:**
   - Một số game có thể block input methods
   - Thử với game khác để verify

3. **Disable Fullscreen Optimization:**
   - Right-click game exe > Properties
   - Compatibility > Disable fullscreen optimizations

## FAQ

### Q: XoneKey có miễn phí không?
**A:** Có, XoneKey hoàn toàn miễn phí và mã nguồn mở.

### Q: Có cần internet để dùng không?
**A:** Không, XoneKey hoạt động offline. Chỉ cần internet để check updates.

### Q: Có thể dùng với tất cả ứng dụng không?
**A:** Có, XoneKey hoạt động với mọi ứng dụng trên Windows, kể cả game.

### Q: Có ảnh hưởng đến hiệu năng không?
**A:** Không đáng kể. XoneKey được tối ưu để sử dụng ít tài nguyên.

### Q: Có thể tùy chỉnh hotkey không?
**A:** Có, trong Settings > Primary > Switch Key.

### Q: Làm sao để tắt XoneKey tạm thời?
**A:** Settings > System > Temp Off XoneKey, hoặc nhấn Alt key.

### Q: Có hỗ trợ các ngôn ngữ khác không?
**A:** Hiện tại chỉ hỗ trợ tiếng Việt, nhưng có thể mở rộng trong tương lai.

### Q: Làm sao để update?
**A:** XoneKey tự động check updates. Hoặc download từ GitHub Releases.

### Q: Có thể backup settings không?
**A:** Settings được lưu trong Windows Registry. Có thể backup bằng Registry Editor.

### Q: Có hỗ trợ Windows mới nhất không?
**A:** Có, XoneKey được update thường xuyên để hỗ trợ Windows mới.

## Support

### Getting Help
- **GitHub Issues:** [Report Issues](https://github.com/vituocgia/XoneKey/issues)
- **Documentation:** Xem DEVELOPER_GUIDE.md cho technical details

### Reporting Bugs
- Mô tả chi tiết vấn đề
- Windows version
- XoneKey version
- Steps to reproduce
- Screenshots nếu có

### Feature Requests
- Submit trên GitHub Issues
- Mô tả feature mong muốn
- Use cases

## License

XoneKey is free and open source software. See LICENSE file for details.

## Credits

Developed by Daniel Do and contributors.

---

**Enjoy typing Vietnamese on Windows! 🇻🇳**

