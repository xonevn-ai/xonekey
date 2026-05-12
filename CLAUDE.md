# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

XoneKey is a production-grade, open-source Vietnamese input method (IME) for Windows, macOS, and Linux. The core invariant is a **single platform-agnostic C++ engine** shared across all platforms — platform code must never bleed into the engine, and the engine must never import platform headers.

Current stable: v1.0.6 (Windows), v1.0.2 (macOS). Linux is in active development.

---

## Build & Test Commands

### Linux — engine unit tests (only automated test suite)
```bash
cd Sources/XoneKey/engine
make              # builds ./test_engine
./test_engine     # run tests
make clean
```

### Windows (requires Visual Studio 2017+ or MSBuild)
```bash
msbuild -m -target:Rebuild -p:Configuration=Release -p:Platform=x86 Sources/XoneKey/win32/XoneKey/XoneKey.sln
msbuild -m -target:Rebuild -p:Configuration=Release -p:Platform=x64 Sources/XoneKey/win32/XoneKey/XoneKey.sln
```

### macOS (requires Xcode 12+)
```bash
xcodebuild -project Sources/XoneKey/macOS/XoneKey.xcodeproj -scheme XoneKey -configuration Release
```

---

## Architecture

```
engine/                        ← platform-agnostic C++ core (C++11/14)
  Engine.cpp/h                   keyboard event processing pipeline
  Vietnamese.cpp/h               vowel/tone/consonant composition rules
  Macro.cpp/h                    user-defined text expansion
  SmartSwitchKey.cpp/h           per-app language mode memory
  ConvertTool.cpp/h              encoding conversion (Unicode ↔ TCVN3/VNI/CP1258)
  EngineSafety.h                 bounds-checking helpers — mandatory for all buffer access
  DataType.h                     shared enums, structs, constants (MAX_BUFF = 32)
  platforms/
    win32.h / mac.h / linux.h    key-code abstraction (macros only, no logic)

win32/                         ← Win32 API app
  WH_KEYBOARD_LL hook → Engine → Registry settings
  Health monitor thread + performance logger + toast notifications

macOS/                         ← Cocoa/Objective-C app
  CGEventTap → XoneKey.mm (C++ bridge) → Engine → NSUserDefaults
  XoneKeyHelper/ login-item helper for launch-at-login

linux/                         ← skeleton only; GTK/X11/Wayland TBD
```

**Data flow (all platforms):** platform hook intercepts key event → calls engine → engine returns `vKeyHookState` (backspace count + replacement chars) → platform replays the edits.

---

## Key Concepts

### Input methods
| ID | Name | Example |
|----|------|---------|
| 0 | Telex (default) | `aa→â`, `s→sắc`, `f→huyền`, `r→hỏi`, `x→ngã`, `j→nặng` |
| 1 | VNI | `a1→á`, `o7→ơ`, `u8→ư` |
| 2–3 | Simple Telex 1 & 2 | simplified variants of Telex |

**Quick Telex:** `cc→ch`, `gg→gi`, `kk→kh`, `nn→ng`, `qq→qu`, `pp→ph`, `tt→th`, `uu→ư`

### Code tables
| ID | Encoding |
|----|----------|
| 0 | Unicode precomposed (default) |
| 1 | TCVN3 (ABC) |
| 2 | VNI Windows |
| 3 | Unicode Combining |
| 4 | CP1258 |

### Global engine state (prefixed `v`)
`vLanguage`, `vInputType`, `vCodeTable`, `vCheckSpelling`, `vUseModernOrthography` — all persisted per-platform (Registry on Windows, NSUserDefaults on macOS).

---

## Coding Rules & Invariants

These are non-negotiable constraints; violating them breaks safety guarantees or cross-platform builds.

### 1. Always bounds-check engine buffer access
All engine arrays are bounded by `MAX_BUFF = 32`. Use the helpers in `EngineSafety.h` (`IsValidIndex`, `IsIndexInWord`, `SafeIncrementIndex`, `SafeDecrementIndex`, `SafeSetKeyState`) instead of raw indexing:
```cpp
// WRONG
TypingWord[++_index] = data;

// RIGHT
if (SafeIncrementIndex(&_index)) TypingWord[_index] = data;
```
Never use raw pointer arithmetic or unchecked indexing on `TypingWord`, `KeyStates`, or the macro buffers.

### 2. Engine must stay platform-free
The engine may only `#include` standard C++ headers and `platforms/win32.h|mac.h|linux.h` (key-code macros only). No `<windows.h>`, `<Cocoa/Cocoa.h>`, or X11 headers inside `engine/`.

### 3. Input validation at every boundary
```cpp
if (!pData || size <= 0) return;          // null/empty check
if (count > MAX_MACRO_COUNT) return;      // limit enforcement
if (cursor + chunkSize > (Uint32)size) break;  // bounds before memcpy
```
Fail silently with a safe default — do not throw exceptions.

### 4. Thread safety
- Windows: use `CRITICAL_SECTION` (macro-gated with `SMART_SWITCH_LOCK`) for shared state in `SmartSwitchKey`.
- macOS: use `NSLock` for initialization races; `@try/@catch` around CGEvent operations.
- Keyboard hooks must return **instantly** — no blocking I/O, no allocations on the hot path.

### 5. Naming conventions
- Engine globals: `v` prefix (`vLanguage`, `vInputType`)
- Safety helpers: `Safe` prefix (`SafeGetTypingWord`, `SafeIncrementIndex`)
- Predicates: `is`/`Is` prefix (`isWordBreak`, `IsValidIndex`)
- Platform key-code macros: `KEY_` prefix (`KEY_A`, `KEY_ENTER`)
- Types: `Byte`, `Uint16`, `Uint32` (defined in `DataType.h`) — use these instead of raw `unsigned`

### 6. No blocking on the main/UI thread
Network update checks, file I/O, and logging must run on background threads. Window events throttled to ≥100 ms intervals.

---

## Adding a New Feature

1. **Engine logic** — modify `engine/` only; keep it platform-free.
2. **Platform wiring** — add hook handling and UI in the relevant platform directory.
3. **Test** — add a case to `engine/test_engine.cpp` and verify with `make && ./test_engine`.
4. **Settings** — persist via Registry (Windows) or NSUserDefaults (macOS); add to `DataType.h` if a new enum is needed.
5. Never add a feature that requires the engine to call back into platform code.

---

## CI/CD

`.github/workflows/msbuild.yml` runs on every push and PR:
- Builds Windows x86 + x64 Release
- On `v*` tags: packages binaries into `XoneKey-vX.Y.Z.zip` and creates a draft GitHub Release

Trigger a release: `git tag v1.0.7 && git push origin v1.0.7`

---

## Active Development Areas & Roadmap

| Status | Area |
|--------|------|
| In progress | Linux application (GTK/X11/Wayland) |
| Planned | Cloud settings sync |
| Planned | Custom themes |
| Planned | Settings profiles |
| Planned | Enhanced macro templates & import/export |
| Planned | Additional input methods beyond Telex/VNI |

When working on the Linux port, mirror the macOS event-loop pattern but target `platforms/linux.h` key codes; the engine already compiles with `-DLINUX`.

---

## Platform-Specific Notes

**Windows:**
- Settings key: `HKEY_CURRENT_USER\Software\XoneKey`
- Log file: `%APPDATA%\XoneKey\xonekey_log.txt`
- Health monitor: dedicated background thread with 30 s heartbeat; recovery re-initializes the hook on the UI thread (not the monitor thread).

**macOS:**
- Requires Accessibility permission (`CGEventTap` needs it at runtime).
- Universal binary (x86_64 + arm64); deployment target macOS 10.13+.
- `XoneKey.mm` is the Objective-C++/C++ bridge — keep C++ logic in the engine, not here.
- `XoneKeyHelper/` is a separate lightweight login-item target; changes there must not pull in engine symbols.
