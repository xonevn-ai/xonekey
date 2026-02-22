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
#include "PerformanceLogger.h"
#include "XoneKeyHelper.h"
#include <fstream>
#include <sstream>
#include <iomanip>

CRITICAL_SECTION PerformanceLogger::_cs;
bool PerformanceLogger::_initialized = false;
std::wstring PerformanceLogger::_logPath;
LogLevel PerformanceLogger::_minLogLevel = LogLevel::INFO;

// Performance counters
static std::map<std::string, DWORD> _performanceCounters;
static DWORD _windowEventCount = 0;
static DWORD _systemTrayUpdateSuccessCount = 0;
static DWORD _systemTrayUpdateFailureCount = 0;

void PerformanceLogger::Initialize()
{
    if (_initialized)
        return;
    
    InitializeCriticalSection(&_cs);
    _logPath = XoneKeyHelper::GetLogPath();
    _initialized = true;
    
    // Log initialization
    LogInfo("Performance logger initialized");
}

void PerformanceLogger::WriteLog(LogLevel level, const std::string& message)
{
    if (!_initialized)
        Initialize();
    
    if (level < _minLogLevel)
        return;
    
    EnterCriticalSection(&_cs);
    
    if (_logPath.empty())
    {
        LeaveCriticalSection(&_cs);
        return;
    }
    
    std::ofstream logFile(_logPath, std::ios::app);
    if (!logFile.is_open())
    {
        LeaveCriticalSection(&_cs);
        return;
    }
    
    // Get current time
    time_t now = time(0);
    struct tm timeinfo;
    char buffer[80];
    localtime_s(&timeinfo, &now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    
    // Write log entry
    logFile << "[" << buffer << "] [" << GetLogLevelString(level) << "] " 
            << message << std::endl;
    
    logFile.close();
    LeaveCriticalSection(&_cs);
}

std::string PerformanceLogger::GetLogLevelString(LogLevel level)
{
    switch (level)
    {
    case LogLevel::DEBUG: return "DEBUG";
    case LogLevel::INFO: return "INFO";
    case LogLevel::WARNING: return "WARNING";
    case LogLevel::LOG_ERROR: return "ERROR";
    default: return "UNKNOWN";
    }
}

void PerformanceLogger::LogDebug(const std::string& message)
{
    WriteLog(LogLevel::DEBUG, message);
}

void PerformanceLogger::LogInfo(const std::string& message)
{
    WriteLog(LogLevel::INFO, message);
}

void PerformanceLogger::LogWarning(const std::string& message)
{
    WriteLog(LogLevel::WARNING, message);
}

void PerformanceLogger::LogError(const std::string& message)
{
    WriteLog(LogLevel::LOG_ERROR, message);
}

// PerformanceTimer implementation
PerformanceLogger::PerformanceTimer::PerformanceTimer(const std::string& operationName)
    : _operationName(operationName)
    , _startTime(std::chrono::high_resolution_clock::now())
    , _active(true)
{
}

PerformanceLogger::PerformanceTimer::~PerformanceTimer()
{
    if (_active)
    {
        Stop();
    }
}

void PerformanceLogger::PerformanceTimer::Stop()
{
    if (!_active)
        return;
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - _startTime);
    
    _active = false;
    
    // Log if operation took more than 100ms
    if (duration.count() > 100)
    {
        std::stringstream ss;
        ss << "Slow operation detected: " << _operationName 
           << " took " << duration.count() << "ms";
        LogWarning(ss.str());
    }
    
#ifdef _DEBUG
    // Always log in debug builds
    std::stringstream ss;
    ss << "Performance: " << _operationName << " took " << duration.count() << "ms";
    LogDebug(ss.str());
#endif
}

void PerformanceLogger::IncrementCounter(const std::string& counterName)
{
    if (!_initialized)
        Initialize();
    
    EnterCriticalSection(&_cs);
    _performanceCounters[counterName]++;
    LeaveCriticalSection(&_cs);
}

void PerformanceLogger::SetCounter(const std::string& counterName, DWORD value)
{
    if (!_initialized)
        Initialize();
    
    EnterCriticalSection(&_cs);
    _performanceCounters[counterName] = value;
    LeaveCriticalSection(&_cs);
}

DWORD PerformanceLogger::GetCounter(const std::string& counterName)
{
    if (!_initialized)
        Initialize();
    
    EnterCriticalSection(&_cs);
    DWORD value = _performanceCounters[counterName];
    LeaveCriticalSection(&_cs);
    return value;
}

void PerformanceLogger::LogWindowEvent()
{
    if (!_initialized)
        Initialize();
    
    EnterCriticalSection(&_cs);
    _windowEventCount++;
    LeaveCriticalSection(&_cs);
}

DWORD PerformanceLogger::GetWindowEventCount()
{
    if (!_initialized)
        Initialize();
    
    EnterCriticalSection(&_cs);
    DWORD count = _windowEventCount;
    LeaveCriticalSection(&_cs);
    return count;
}

void PerformanceLogger::ResetWindowEventCount()
{
    if (!_initialized)
        Initialize();
    
    EnterCriticalSection(&_cs);
    _windowEventCount = 0;
    LeaveCriticalSection(&_cs);
}

void PerformanceLogger::LogSystemTrayUpdate(bool success)
{
    if (!_initialized)
        Initialize();
    
    EnterCriticalSection(&_cs);
    if (success)
    {
        _systemTrayUpdateSuccessCount++;
    }
    else
    {
        _systemTrayUpdateFailureCount++;
        std::stringstream ss;
        ss << "System tray icon update failed (total failures: " 
           << _systemTrayUpdateFailureCount << ")";
        LeaveCriticalSection(&_cs);
        LogWarning(ss.str());
        return;
    }
    LeaveCriticalSection(&_cs);
}

DWORD PerformanceLogger::GetSystemTrayUpdateSuccessCount()
{
    if (!_initialized)
        Initialize();
    
    EnterCriticalSection(&_cs);
    DWORD count = _systemTrayUpdateSuccessCount;
    LeaveCriticalSection(&_cs);
    return count;
}

DWORD PerformanceLogger::GetSystemTrayUpdateFailureCount()
{
    if (!_initialized)
        Initialize();
    
    EnterCriticalSection(&_cs);
    DWORD count = _systemTrayUpdateFailureCount;
    LeaveCriticalSection(&_cs);
    return count;
}

void PerformanceLogger::SetMinLogLevel(LogLevel level)
{
    if (!_initialized)
        Initialize();
    
    EnterCriticalSection(&_cs);
    _minLogLevel = level;
    LeaveCriticalSection(&_cs);
}

void PerformanceLogger::Flush()
{
    if (!_initialized)
        return;
    
    // Force flush any buffered operations
    EnterCriticalSection(&_cs);
    LeaveCriticalSection(&_cs);
}

