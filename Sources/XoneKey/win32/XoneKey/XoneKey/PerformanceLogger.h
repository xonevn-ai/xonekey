/*----------------------------------------------------------
XoneKey - The Cross platform Open source Vietnamese Keyboard application.

Copyright (C) 2025 Daniel Do
Contact: diep@xone.vn
Github: https://github.com/vituocgia/XoneKey
Fanpage: https://www.facebook.com/XoneKeyVN

This file is belong to the XoneKey project, Win32 version
which is released under GPL license.
You can fork, modify, improve this program. If you
redistribute your new version, it MUST be open source.
-----------------------------------------------------------*/
#pragma once
#include "stdafx.h"
#include <chrono>
#include <map>

enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    LOG_ERROR = 3
};

/**
 * Performance Logger
 * Comprehensive logging system with performance monitoring
 */
class PerformanceLogger {
private:
    static CRITICAL_SECTION _cs;
    static bool _initialized;
    static std::wstring _logPath;
    static LogLevel _minLogLevel;
    
    static void Initialize();
    static void WriteLog(LogLevel level, const std::string& message);
    static std::string GetLogLevelString(LogLevel level);
    
public:
    static void LogDebug(const std::string& message);
    static void LogInfo(const std::string& message);
    static void LogWarning(const std::string& message);
    static void LogError(const std::string& message);
    
    // Performance tracking
    class PerformanceTimer {
    private:
        std::string _operationName;
        std::chrono::high_resolution_clock::time_point _startTime;
        bool _active;
        
    public:
        PerformanceTimer(const std::string& operationName);
        ~PerformanceTimer();
        void Stop();
    };
    
    // Performance counters
    static void IncrementCounter(const std::string& counterName);
    static void SetCounter(const std::string& counterName, DWORD value);
    static DWORD GetCounter(const std::string& counterName);
    
    // Window event hook tracking
    static void LogWindowEvent();
    static DWORD GetWindowEventCount();
    static void ResetWindowEventCount();
    
    // System tray update tracking
    static void LogSystemTrayUpdate(bool success);
    static DWORD GetSystemTrayUpdateSuccessCount();
    static DWORD GetSystemTrayUpdateFailureCount();
    
    static void SetMinLogLevel(LogLevel level);
    static void Flush();
};

// RAII helper for performance timing
#define PERF_TIMER(name) PerformanceLogger::PerformanceTimer _perfTimer(name)

