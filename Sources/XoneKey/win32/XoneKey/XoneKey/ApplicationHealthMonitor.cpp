/*----------------------------------------------------------
XoneKey - The Cross platform Open source Vietnamese Keyboard application.

Copyright (C) 2025 Daniel Do
Contact: diep@xone.vn
Github: https://github.com/xonevn-ai/xonekey
Fanpage: https://www.facebook.com/XoneKeyVN

This file is belong to the XoneKey project, Win32 version
which is released under GPL license.
You can fork, modify, improve this program. If you
redistribute your new version, it MUST be open source.
-----------------------------------------------------------*/
#include "ApplicationHealthMonitor.h"
#include "XoneKeyHelper.h"
#include "PerformanceLogger.h"
#include <sstream>

ApplicationHealthMonitor* ApplicationHealthMonitor::_instance = nullptr;

ApplicationHealthMonitor::ApplicationHealthMonitor()
    : _heartbeatTimerId(0)
    , _lastHeartbeatTime(0)
    , _startTime(GetTickCount())
    , _isMonitoring(false)
{
    InitializeCriticalSection(&_cs);
}

ApplicationHealthMonitor::~ApplicationHealthMonitor()
{
    StopMonitoring();
    DeleteCriticalSection(&_cs);
}

ApplicationHealthMonitor* ApplicationHealthMonitor::GetInstance()
{
    if (_instance == nullptr)
    {
        _instance = new ApplicationHealthMonitor();
    }
    return _instance;
}

void ApplicationHealthMonitor::DestroyInstance()
{
    if (_instance != nullptr)
    {
        delete _instance;
        _instance = nullptr;
    }
}

bool ApplicationHealthMonitor::StartMonitoring()
{
    EnterCriticalSection(&_cs);
    
    if (_isMonitoring)
    {
        LeaveCriticalSection(&_cs);
        return true;
    }
    
    // Create a hidden window for timer messages
    HWND hWnd = FindWindow(APP_CLASS, NULL);
    if (hWnd == NULL)
    {
        LeaveCriticalSection(&_cs);
        return false;
    }
    
    _lastHeartbeatTime = GetTickCount();
    _startTime = GetTickCount();
    
    // Set up timer for heartbeat check
    _heartbeatTimerId = SetTimer(hWnd, 1, HEARTBEAT_INTERVAL_MS, HeartbeatTimerProc);
    
    if (_heartbeatTimerId == 0)
    {
        LeaveCriticalSection(&_cs);
        XoneKeyHelper::LogError("Failed to create heartbeat timer");
        return false;
    }
    
    _isMonitoring = true;
    LeaveCriticalSection(&_cs);
    
    PerformanceLogger::LogInfo("Application health monitoring started");
    return true;
}

void ApplicationHealthMonitor::StopMonitoring()
{
    EnterCriticalSection(&_cs);
    
    if (!_isMonitoring)
    {
        LeaveCriticalSection(&_cs);
        return;
    }
    
    HWND hWnd = FindWindow(APP_CLASS, NULL);
    if (hWnd != NULL && _heartbeatTimerId != 0)
    {
        KillTimer(hWnd, _heartbeatTimerId);
        _heartbeatTimerId = 0;
    }
    
    _isMonitoring = false;
    LeaveCriticalSection(&_cs);
    
    PerformanceLogger::LogInfo("Application health monitoring stopped");
}

void ApplicationHealthMonitor::SignalHeartbeat()
{
    EnterCriticalSection(&_cs);
    _lastHeartbeatTime = GetTickCount();
    LeaveCriticalSection(&_cs);
}

VOID CALLBACK ApplicationHealthMonitor::HeartbeatTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    UNREFERENCED_PARAMETER(hwnd);
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(idEvent);
    UNREFERENCED_PARAMETER(dwTime);
    
    if (_instance != nullptr)
    {
        _instance->CheckHealth();
    }
}

void ApplicationHealthMonitor::CheckHealth()
{
    EnterCriticalSection(&_cs);
    
    DWORD currentTime = GetTickCount();
    DWORD timeSinceLastHeartbeat = 0;
    
    // Safe calculation that handles GetTickCount() overflow after ~49 days
    if (currentTime >= _lastHeartbeatTime)
    {
        timeSinceLastHeartbeat = currentTime - _lastHeartbeatTime;
    }
    else
    {
        // Overflow occurred (GetTickCount wrapped around), calculate correctly
        timeSinceLastHeartbeat = (MAXDWORD - _lastHeartbeatTime) + currentTime + 1;
    }
    
    // Update heartbeat timestamp (main message loop is running)
    _lastHeartbeatTime = currentTime;
    
    // Check if we've missed heartbeats
    if (timeSinceLastHeartbeat > HEARTBEAT_TIMEOUT_MS)
    {
        std::stringstream ss;
        ss << "Application health check failed: No heartbeat for " 
           << timeSinceLastHeartbeat << "ms (threshold: " << HEARTBEAT_TIMEOUT_MS << "ms)";
        PerformanceLogger::LogError(ss.str());
        
        // Log additional diagnostic information
        PerformanceLogger::LogWarning("Application may be unresponsive - attempting recovery");
    }
    
    LeaveCriticalSection(&_cs);
}

DWORD ApplicationHealthMonitor::GetUptime() const
{
    EnterCriticalSection(&_cs);
    DWORD uptime = GetTickCount() - _startTime;
    LeaveCriticalSection(&_cs);
    return uptime;
}

bool ApplicationHealthMonitor::IsHealthy() const
{
    EnterCriticalSection(&_cs);
    DWORD currentTime = GetTickCount();
    DWORD timeSinceLastHeartbeat = currentTime - _lastHeartbeatTime;
    bool healthy = timeSinceLastHeartbeat < HEARTBEAT_TIMEOUT_MS;
    LeaveCriticalSection(&_cs);
    return healthy;
}

