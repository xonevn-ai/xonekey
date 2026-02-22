# XoneKey

[![MSBuild](https://github.com/xonevn-ai/xonekey/actions/workflows/msbuild.yml/badge.svg)](https://github.com/xonevn-ai/xonekey/actions/workflows/msbuild.yml)
[![GitHub release](https://img.shields.io/github/v/release/xonevn-ai/xonekey.svg)](https://github.com/xonevn-ai/xonekey/releases/latest)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

**XoneKey** is a modern, high-performance, and open-source Vietnamese input application for Windows, macOS, and Linux. It aims to provide a seamless typing experience while eliminating common issues like the underline bug on macOS and lag on Windows.

## 🚀 Key Features (v1.0.2)

-   **High Performance**: Minimal lag and low resource consumption.
-   **Stability Watchdog**: (Windows) Dedicated background thread monitors health and automatically recovers from hangs.
-   **Smart Switch**: Automatically remembers and switches your typing mode (Vietnamese/English) based on the active application.
-   **Async Engine**: (Windows) Non-blocking process queries and network updates to keep your typing fluid.
-   **Rich Engine**: Supports Telex, VNI, and Simple Telex with modern orthography (òa, úy vs oà, uý).
-   **Advanced Macros**: Unlimited length macros with support for any character or code table.
-   **Cross-Platform Core**: Shared C++ core engine ensures consistent typing behavior across Windows, macOS, and Linux.

## 📥 Download

Get the latest stable release for your platform from the [Releases](https://github.com/xonevn-ai/xonekey/releases) page.

## 🔧 Build & Development

XoneKey is built using a shared C++ core engine with platform-specific UI layers.

### Windows
-   **Requirements**: Visual Studio 2019+
-   **Solution**: `Sources/XoneKey/win32/XoneKey/XoneKey.sln`
-   **Output**: `XoneKey.exe` (x86/x64)

### macOS
-   **Requirements**: Xcode 12+
-   **Project**: `Sources/XoneKey/macOS/XoneKey.xcodeproj`
-   **Homebrew**: `brew install --cask xonekey`

### Linux (WSL/Ubuntu)
-   **Requirements**: `g++`, `make`
-   **Build**: See our [WSL Build Guide](Sources/XoneKey/WSL_BUILD_GUIDE.md) in the `engine` directory.

## 🤖 GitHub Automation

We use GitHub Actions for continuous integration and automated releases.
-   **Windows Builds**: Every push to `master` triggers a Windows build.
-   **Releases**: Pushing a version tag (e.g., `v1.0.2`) automatically creates a draft release with packaged binaries.
-   See the [GitHub Actions Guide](Sources/XoneKey/GITHUB_ACTIONS_GUIDE.md) for more details.

## 🤝 Contributing

XoneKey is open-source under the **GPL v3 License**. We welcome contributions of all forms!
-   Feel free to fork the repository and submit a Pull Request.
-   Join our [Fanpage](https://www.facebook.com/XoneKeyVN) for community support.

## ☕ Support the Author

If you find XoneKey useful, consider supporting its development:
-   [Buy me a coffee](https://xonevn-ai.github.io/donate.html)
-   [Donate via PayPal](https://paypal.me/thegioivatdung)

---
Developed by **Đỗ Tiến Điệp** (diep@xone.vn)
