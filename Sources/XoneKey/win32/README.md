# XoneKey for Windows

[![GitHub release](https://img.shields.io/github/v/release/vituocgia/XoneKey.svg)](https://github.com/vituocgia/XoneKey/releases/latest)

XoneKey là ứng dụng gõ tiếng Việt miễn phí và mã nguồn mở cho Windows, được phát triển từ phiên bản macOS với đầy đủ tính năng.

![Main UI](https://raw.githubusercontent.com/vituocgia/vituocgia.github.io/master/images/win32_2.0_1.png)

## Tính Năng Chính

### Kiểu Gõ
- ✅ **Telex** - Kiểu gõ phổ biến nhất
- ✅ **VNI** - Kiểu gõ số
- ✅ **Simple Telex 1 & 2** - Telex đơn giản hóa

### Bảng Mã
- ✅ **Unicode dựng sẵn** (Mặc định) - Chuẩn Unicode phổ biến
- ✅ **TCVN3 (ABC)** - Bảng mã cũ
- ✅ **VNI Windows** - Bảng mã VNI
- ✅ **Unicode Tổ hợp** - Unicode với combining marks
- ✅ **Vietnamese Locale CP 1258** - Windows-1258

### Tính Năng Nâng Cao
- ✅ **Kiểm tra chính tả** - Tự động sửa lỗi chính tả
- ✅ **Gõ tắt (Macro)** - Tạo shortcuts cho cụm từ dài
- ✅ **Chuyển mã** - Chuyển đổi giữa các bảng mã
- ✅ **Smart Switch Key** - Tự động chuyển đổi ngôn ngữ theo ứng dụng
- ✅ **Quick Telex** - Gõ nhanh: `cc`→`ch`, `gg`→`gi`, etc.
- ✅ **Quick Consonant** - Gõ nhanh phụ âm đầu/cuối
- ✅ **Remember Code** - Nhớ bảng mã cho từng ứng dụng
- ✅ **Toast Notifications** - Thông báo khi chuyển đổi ngôn ngữ

### Tùy Chọn
- ✅ Tự cấu hình phím tắt chuyển Anh-Việt
- ✅ Bỏ dấu kiểu mới (oà, uý) và kiểu cũ (òa, úy)
- ✅ Sửa lỗi gợi ý trên trình duyệt, Excel
- ✅ Khởi động cùng Windows
- ✅ Chạy với quyền Administrator (cho game)
- ✅ Sửa lỗi trên Chromium browsers
- ✅ Hỗ trợ ứng dụng Metro trên Windows 10

## Yêu Cầu Hệ Thống

- **Windows Vista** trở lên (khuyến nghị Windows 10/11)
- **Quyền Administrator** (khuyến nghị để hoạt động tốt với game)
- **.NET Framework** (nếu cần, sẽ được cài tự động)

## Cài Đặt

### Cách 1: Download từ GitHub Releases

1. Vào [GitHub Releases](https://github.com/vituocgia/XoneKey/releases/latest)
2. Tải file `.zip` phiên bản mới nhất
3. Giải nén vào thư mục bất kỳ
4. Chạy `XoneKey.exe`
5. Cho phép chạy với quyền Administrator khi được hỏi

### Cách 2: Build từ Source

Xem [DEVELOPER_GUIDE.md](DEVELOPER_GUIDE.md) để biết cách build từ source code.

## Sử Dụng

### Lần Đầu Sử Dụng

1. **Chạy ứng dụng:**
   - Double-click `XoneKey.exe`
   - Cho phép chạy với quyền Administrator

2. **Kiểm tra System Tray:**
   - Icon chữ **V** (màu xanh) = Tiếng Việt đang bật
   - Icon chữ **E** (màu xám) = Tiếng Anh đang bật

3. **Mở Settings:**
   - Double-click icon trên system tray
   - Hoặc right-click > Bảng điều khiển

4. **Cấu hình:**
   - Chọn kiểu gõ (Telex/VNI/Simple Telex)
   - Chọn bảng mã (Unicode/TCVN3/VNI Windows)
   - Cấu hình phím tắt chuyển đổi
   - Bật/tắt các tính năng cần thiết

### Chuyển Đổi Ngôn Ngữ

**Cách 1: Phím Tắt (Mặc định: Alt + Z)**
- Nhấn phím tắt để chuyển đổi nhanh
- Có thể tùy chỉnh trong Settings

**Cách 2: System Tray Menu**
- Right-click icon > Bật Tiếng Việt / Tắt Tiếng Việt

**Cách 3: Settings Dialog**
- Mở Settings > Primary tab > Toggle Vietnamese/English

### Gõ Tiếng Việt

**Ví dụ với Telex:**
- `xin chao` → `xin chào`
- `toi ten la Nam` → `tôi tên là Nam`
- `ban khoe khong` → `bạn khỏe không`

**Ví dụ với VNI:**
- `xin chao1` → `xin chào`
- `toi6 ten2 la8 Nam` → `tôi tên là Nam`

Xem [USER_GUIDE.md](USER_GUIDE.md) để biết chi tiết về cách sử dụng.

## Hình Ảnh

![Menu](https://raw.githubusercontent.com/vituocgia/vituocgia.github.io/master/images/win32_5.png "Menu")
![Macro Table](https://raw.githubusercontent.com/vituocgia/vituocgia.github.io/master/images/win32_2.png "Macro table")
![Convert Tool](https://raw.githubusercontent.com/vituocgia/vituocgia.github.io/master/images/win32_3.png "Convert tool")

## Tính Năng Mới

### Version 2.0+
- ✅ **Application Health Monitoring** - Tự động phát hiện và khôi phục lỗi
- ✅ **Performance Logging** - Theo dõi hiệu năng và debug
- ✅ **Network Helper** - Cải thiện network operations với timeout
- ✅ **Thread Safety** - Cải thiện thread safety cho global state
- ✅ **Toast Notifications** - Thông báo khi chuyển đổi ngôn ngữ
- ✅ **Enhanced Error Handling** - Xử lý lỗi tốt hơn
- ✅ **Memory Management** - Tối ưu memory và tránh leaks

## Troubleshooting

### Vấn Đề Thường Gặp

**Q: XoneKey không hoạt động?**
- A: Đảm bảo đã chạy với quyền Administrator
- A: Kiểm tra icon trên system tray có xuất hiện không
- A: Restart XoneKey

**Q: Không gõ được tiếng Việt?**
- A: Kiểm tra icon system tray - phải là chữ V màu xanh
- A: Kiểm tra kiểu gõ trong Settings
- A: Thử gõ lại

**Q: Phím tắt không hoạt động?**
- A: Kiểm tra conflict với phím tắt khác
- A: Cấu hình lại phím tắt trong Settings

**Q: App crash?**
- A: Check log file trong thư mục ứng dụng
- A: Restart XoneKey
- A: Reinstall nếu cần

Xem [USER_GUIDE.md](USER_GUIDE.md) để biết thêm chi tiết troubleshooting.

## Development

### Build Requirements
- Visual Studio 2017 hoặc mới hơn
- Windows SDK 10.0+
- C++ compiler với C++14 support

### Build Instructions
Xem [DEVELOPER_GUIDE.md](DEVELOPER_GUIDE.md) để biết chi tiết.

### Project Structure
```
win32/
├── XoneKey/
│   ├── XoneKey/          # Main application
│   │   ├── AppDelegate.cpp/h
│   │   ├── XoneKey.cpp/h
│   │   ├── MainControlDialog.cpp/h
│   │   └── ...
│   ├── XoneKeyUpdate/    # Update helper
│   └── XoneKey.sln       # Solution file
└── README.md
```

## Lịch Sử

- **18/01/2019** - Dự án XoneKey ra đời
- **01/02/2019** - Phiên bản đầu tiên cho macOS
- **31/07/2019** - Được biết đến qua tinhte.vn
- **19/09/2019** - Phiên bản đầu tiên cho Windows
- **2025** - Version 2.0+ với nhiều cải thiện

## License

XoneKey được phát hành dưới giấy phép **GPL (GNU General Public License)**.

Điều này có nghĩa:
- ✅ Bạn có thể tự do sử dụng
- ✅ Bạn có thể tự do chỉnh sửa mã nguồn
- ✅ Bạn có thể tự do phân phối
- ⚠️ Nếu bạn phân phối bản chỉnh sửa, bạn **phải** công bố mã nguồn và credit XoneKey bản gốc

## Contributing

Chúng tôi hoan nghênh mọi đóng góp! Vui lòng:
1. Fork repository
2. Tạo feature branch
3. Commit changes
4. Push và tạo Pull Request

## Support

- **GitHub Issues:** [Report Issues](https://github.com/vituocgia/XoneKey/issues)
- **Documentation:** Xem USER_GUIDE.md và DEVELOPER_GUIDE.md
- **Website:** [XoneKey Website]

## Donate

Nếu bạn thấy XoneKey hữu ích, hãy ủng hộ tác giả:

- **PayPal:** [Gửi cafe](https://paypal.me/thegioivatdung)
- **MoMo:** 
  ![Donate by momo](https://vituocgia.github.io/images/momo.png "Momo")

Cảm ơn bạn đã sử dụng XoneKey! 🇻🇳
