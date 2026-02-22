# XoneKey macOS - User Guide

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

XoneKey là ứng dụng gõ tiếng Việt miễn phí cho macOS, hỗ trợ nhiều kiểu gõ và bảng mã khác nhau.

### Key Features
- ✅ Gõ tiếng Việt với nhiều kiểu gõ (Telex, VNI, Simple Telex)
- ✅ Hỗ trợ nhiều bảng mã (Unicode, TCVN3, VNI Windows, etc.)
- ✅ Chức năng gõ tắt (Macro)
- ✅ Chuyển đổi mã nhanh
- ✅ Tự động chuyển đổi ngôn ngữ thông minh
- ✅ Hoạt động với mọi ứng dụng

## Installation

### System Requirements
- macOS 10.13 (High Sierra) hoặc mới hơn
- Quyền Accessibility (cần thiết để hoạt động)

### Download & Install

1. **Download XoneKey**
   - Tải từ website chính thức hoặc App Store
   - File: `XoneKey.dmg`

2. **Install Application**
   - Mở file `.dmg` đã tải
   - Kéo `XoneKey.app` vào thư mục Applications
   - Hoặc double-click để cài đặt

3. **First Launch**
   - Mở Applications folder
   - Double-click `XoneKey.app`
   - macOS sẽ yêu cầu cấp quyền

## First Time Setup

### Step 1: Grant Accessibility Permission

XoneKey cần quyền Accessibility để có thể intercept keyboard events.

1. **Khi lần đầu mở:**
   - macOS sẽ hiện dialog yêu cầu cấp quyền
   - Click "Cấp quyền" hoặc "Open System Preferences"

2. **Nếu đã bỏ qua:**
   - Mở System Preferences (System Settings trên macOS Ventura+)
   - Vào Security & Privacy (Privacy & Security)
   - Chọn Accessibility
   - Tìm XoneKey trong danh sách
   - Tick vào checkbox để enable

3. **Restart XoneKey:**
   - Quit XoneKey (click icon menu bar > Thoát)
   - Mở lại XoneKey

### Step 2: Verify Installation

Sau khi cấp quyền, bạn sẽ thấy:
- Icon XoneKey xuất hiện trên menu bar (phía trên bên phải)
- Icon màu xanh = Tiếng Việt đang bật
- Icon màu xám = Tiếng Anh đang bật

### Step 3: Test Input

1. Mở bất kỳ ứng dụng nào (TextEdit, Notes, etc.)
2. Click vào icon XoneKey trên menu bar
3. Chọn "Bật Tiếng Việt" nếu chưa bật
4. Gõ thử: `xin chao` → sẽ thành `xin chào`

## Basic Usage

### Toggle Vietnamese/English

**Cách 1: Menu Bar**
- Click icon XoneKey trên menu bar
- Click "Bật Tiếng Việt" để toggle

**Cách 2: Hotkey (mặc định: Option + Z)**
- Nhấn phím tắt để chuyển đổi nhanh
- Có thể tùy chỉnh trong Settings

### Menu Bar Icon

**Icon States:**
- 🟢 **Xanh** = Tiếng Việt đang bật
- ⚪ **Xám** = Tiếng Anh đang bật

**Menu Options:**
- Bật Tiếng Việt - Toggle Vietnamese/English
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
1. Click icon menu bar > Gõ tắt
2. Click nút "+" để thêm mới
3. Nhập từ viết tắt (ví dụ: `tc`)
4. Nhập nội dung đầy đủ (ví dụ: `trân trọng cảm ơn`)
5. Click Save

**Sử dụng:**
- Gõ từ viết tắt
- Nhấn Space hoặc Enter
- Tự động thay thế

### 5. Quick Convert (Chuyển Mã Nhanh)

Chuyển đổi mã văn bản từ clipboard.

**Cách dùng:**
1. Copy văn bản cần chuyển mã
2. Click icon menu bar > Chuyển mã nhanh
   - Hoặc dùng hotkey (nếu đã cấu hình)
3. Văn bản đã chuyển mã sẽ được copy vào clipboard
4. Paste vào nơi cần dùng

**Hỗ trợ:**
- HTML text
- Plain text
- Tự động detect và convert

## Settings

### Mở Settings

**Cách 1: Menu Bar**
- Click icon XoneKey > Bảng điều khiển

**Cách 2: Hotkey**
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
- Mặc định: Option + Z
- Có thể tùy chỉnh: Control, Option, Command, Shift + Key

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
- Sửa lỗi gợi ý trên trình duyệt

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
- Tự động chạy khi khởi động Mac

**Show Icon On Dock:**
- Hiển thị icon trên Dock

**Smart Switch Key:**
- Tự động chuyển đổi ngôn ngữ theo ứng dụng

**Remember Code:**
- Nhớ bảng mã cho từng ứng dụng

**Other Language:**
- Hỗ trợ ngôn ngữ khác

**Temp Off XoneKey:**
- Tạm thời tắt XoneKey

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
- Icon menu bar không xuất hiện

**Solutions:**
1. **Check Accessibility Permission:**
   - System Preferences > Security & Privacy > Accessibility
   - Đảm bảo XoneKey được tick
   - Nếu chưa, tick và restart XoneKey

2. **Restart XoneKey:**
   - Quit XoneKey (menu bar > Thoát)
   - Mở lại từ Applications

3. **Check if Running:**
   - Activity Monitor > Tìm XoneKey
   - Nếu có nhiều instances, quit tất cả và mở lại

### Issue: Không thể cấp quyền Accessibility

**Solutions:**
1. **Manual Grant:**
   - System Preferences > Security & Privacy > Accessibility
   - Click lock để unlock
   - Tìm XoneKey và tick

2. **Reset Permissions:**
   - Quit XoneKey
   - Terminal: `tccutil reset Accessibility com.xonevn.xonekey`
   - Mở lại XoneKey và cấp quyền

### Issue: Gõ không đúng

**Symptoms:**
- Gõ `aa` không ra `â`
- Dấu không đúng

**Solutions:**
1. **Check Input Type:**
   - Menu bar > Kiểu gõ > Chọn đúng (Telex/VNI)

2. **Check Code Table:**
   - Menu bar > Bảng mã > Chọn Unicode dựng sẵn

3. **Check Spelling:**
   - Settings > Advanced > Spelling Check
   - Tắt nếu gây vấn đề

### Issue: Hotkey không hoạt động

**Solutions:**
1. **Check Conflict:**
   - System Preferences > Keyboard > Shortcuts
   - Tìm xem có conflict không

2. **Reconfigure:**
   - Settings > Primary > Switch Key
   - Chọn lại hotkey khác

### Issue: App crash

**Solutions:**
1. **Check Console:**
   - Console.app > Tìm error messages

2. **Reset Settings:**
   - Quit XoneKey
   - Terminal: `defaults delete com.xonevn.xonekey`
   - Mở lại XoneKey

3. **Reinstall:**
   - Quit XoneKey
   - Xóa từ Applications
   - Download và cài đặt lại

### Issue: Menu bar icon không hiện

**Solutions:**
1. **Check Menu Bar Space:**
   - Có thể menu bar đầy
   - Ẩn một số icons khác

2. **Restart:**
   - Quit và mở lại XoneKey

3. **Check Settings:**
   - Settings > System > Show Icon On Dock
   - Đảm bảo không bị ẩn

## FAQ

### Q: XoneKey có miễn phí không?
**A:** Có, XoneKey hoàn toàn miễn phí và mã nguồn mở.

### Q: Có cần internet để dùng không?
**A:** Không, XoneKey hoạt động offline. Chỉ cần internet để check updates.

### Q: Có thể dùng với tất cả ứng dụng không?
**A:** Có, XoneKey hoạt động với mọi ứng dụng trên macOS.

### Q: Có ảnh hưởng đến hiệu năng không?
**A:** Không đáng kể. XoneKey được tối ưu để sử dụng ít tài nguyên.

### Q: Có thể tùy chỉnh hotkey không?
**A:** Có, trong Settings > Primary > Switch Key.

### Q: Làm sao để tắt XoneKey tạm thời?
**A:** Settings > System > Temp Off XoneKey, hoặc click menu bar > Bật Tiếng Việt để tắt.

### Q: Có hỗ trợ các ngôn ngữ khác không?
**A:** Hiện tại chỉ hỗ trợ tiếng Việt, nhưng có thể mở rộng trong tương lai.

### Q: Làm sao để update?
**A:** XoneKey tự động check updates. Hoặc download từ website chính thức.

### Q: Có thể backup settings không?
**A:** Settings được lưu trong NSUserDefaults. Có thể backup bằng Time Machine hoặc export manually.

### Q: Có hỗ trợ macOS mới nhất không?
**A:** Có, XoneKey được update thường xuyên để hỗ trợ macOS mới.

## Support

### Getting Help
- **Website:** [XoneKey website]
- **GitHub:** [GitHub repository]
- **Issues:** [GitHub Issues]

### Reporting Bugs
- Mô tả chi tiết vấn đề
- macOS version
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

**Enjoy typing Vietnamese on macOS! 🇻🇳**

