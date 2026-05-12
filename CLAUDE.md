# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

XoneKey is an open-source Vietnamese input method application for Windows, macOS, and Linux. It uses a shared C++ engine with platform-specific UI layers.

## Build Commands

### Windows
```bash
# Open in Visual Studio
cd Sources/XoneKey/win32/XoneKey
start XoneKey.sln

# Command-line (requires MSBuild)
msbuild -m -target:Rebuild -p:Configuration=Release -p:Platform=x86 Sources/XoneKey/win32/XoneKey/XoneKey.sln
msbuild -m -target:Rebuild -p:Configuration=Release -p:Platform=x64 Sources/XoneKey/win32/XoneKey/XoneKey.sln
```

### macOS
```bash
xcodebuild -project Sources/XoneKey/macOS/XoneKey.xcodeproj -scheme XoneKey -configuration Release
```

### Linux (engine only)
```bash
cd Sources/XoneKey/engine
make          # Builds test_engine executable
make clean
```

### Running Tests
```bash
# Linux/WSL only — the only automated test suite
cd Sources/XoneKey/engine
make && ./test_engine
```

## Architecture

**Core pattern:** A single platform-agnostic C++ engine, compiled into each platform app. Platform code never imports from other platform directories — only from `engine/`.

```
engine/          ← shared C++ core (no platform APIs)
  Engine.cpp/h           # keyboard event processing & typing logic
  Vietnamese.cpp/h       # Vietnamese language rules
  Macro.cpp/h            # macro/shortcut functionality
  SmartSwitchKey.cpp/h   # per-app language switching
  ConvertTool.cpp/h      # encoding conversion utilities
  EngineSafety.h         # bounds-checking helpers (no platform deps)
  DataType.h             # shared enums and structs
  platforms/
    win32.h / mac.h / linux.h   # key-code abstraction per platform

win32/           ← Win32 API app; hook via WH_KEYBOARD_LL
macOS/           ← Cocoa app; event tap via CGEventTap API
linux/           ← in development (GTK/X11/Wayland)
```

**Platform requirements:**
- Windows: Visual Studio 2017+, Windows SDK 10.0+, C++14
- macOS: Xcode 12+, macOS 10.13+ deployment target, C++14
- Linux: g++, C++11

**Settings persistence:** Registry (Windows), NSUserDefaults plist (macOS).

## CI/CD

GitHub Actions (`.github/workflows/msbuild.yml`) builds Windows x86/x64 on every push to `master` and on pull requests. Pushing a `v*` tag triggers a draft GitHub Release with packaged binaries.

## Key Engine Concepts

- **Input methods:** Telex, VNI, Simple Telex 1 & 2
- **Code tables:** Unicode, TCVN3 (ABC), VNI Windows, Unicode Combining, CP1258
- **Quick Telex shortcuts:** `cc→ch`, `gg→gi`, `kk→kh`, `nn→ng`, `qq→qu`, etc.
- `EngineSafety.h` must be used for all buffer/array access in the engine — never raw pointer arithmetic without bounds checks.
- The engine is compiled with `LINUX`, `WIN32`, or `MAC` preprocessor flags set by each platform's build system.
