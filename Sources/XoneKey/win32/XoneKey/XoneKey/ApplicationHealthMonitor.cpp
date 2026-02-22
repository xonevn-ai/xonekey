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
#include "stdafx.h"
#include "ApplicationHealthMonitor.h"
#include "XoneKeyHelper.h"
#include "PerformanceLogger.h"
#include "XoneKeyManager.h"
#include <sstream>

ApplicationHealthMonitor* ApplicationHealthMonitor::_instance = nullptr;

ApplicationHealthMonitor::ApplicationHealthMonitor()
    : _hThread(NULL)
    , _hStopEvent(NULL)
    , _lastHeartbeatTime(0)
    , _startTime(GetTickCount())
    , _isMonitoring(false)
{
    InitializeCriticalSection(&_cs);
    _hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

ApplicationHealthMonitor::~ApplicationHealthMonitor()
{
    StopMonitoring();
    if (_hStopEvent) CloseHandle(_hStopEvent);
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
    
    _lastHeartbeatTime = GetTickCount();
    _startTime = GetTickCount();
    ResetEvent(_hStopEvent);
    
    _hThread = CreateThread(NULL, 0, MonitorThreadProc, this, 0, NULL);
    
    if (_hThread == NULL)
    {
        LeaveCriticalSection(&_cs);
        XoneKeyHelper::LogError("Failed to create health monitor thread");
        return false;
    }
    
    _isMonitoring = true;
    LeaveCriticalSection(&_cs);
    
    PerformanceLogger::LogInfo("Application health monitoring thread started");
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
    
    SetEvent(_hStopEvent);
    LeaveCriticalSection(&_cs);
    
    if (_hThread)
    {
        WaitForSingleObject(_hThread, 5000);
        CloseHandle(_hThread);
        _hThread = NULL;
    }
    
    _isMonitoring = false;
    PerformanceLogger::LogInfo("Application health monitoring thread stopped");
}

void ApplicationHealthMonitor::SignalHeartbeat()
{
    EnterCriticalSection(&_cs);
    _lastHeartbeatTime = GetTickCount();
    LeaveCriticalSection(&_cs);
}

DWORD WINAPI ApplicationHealthMonitor::MonitorThreadProc(LPVOID lpParam)
{
    ApplicationHealthMonitor* pThis = (ApplicationHealthMonitor*)lpParam;
    
    while (WaitForSingleObject(pThis->_hStopEvent, CHECK_INTERVAL_MS) == WAIT_TIMEOUT)
    {
        pThis->CheckHealth();
    }
    
    return 0;
}

void ApplicationHealthMonitor::CheckHealth()
{
    EnterCriticalSection(&_cs);
    
    DWORD currentTime = GetTickCount();
    DWORD timeSinceLastHeartbeat = 0;
    
    if (currentTime >= _lastHeartbeatTime)
    {
        timeSinceLastHeartbeat = currentTime - _lastHeartbeatTime;
    }
    else
    {
        // Overflow occurred
        timeSinceLastHeartbeat = (MAXDWORD - _lastHeartbeatTime) + currentTime + 1;
    }
    
    if (timeSinceLastHeartbeat > HEARTBEAT_TIMEOUT_MS)
    {
        std::stringstream ss;
        ss << "CRITICAL: Application hang detected! No heartbeat for " 
           << timeSinceLastHeartbeat << "ms";
        PerformanceLogger::LogError(ss.str());
        
        // Attempt recovery: re-initialize hooks if they might be frozen
        // This is a last resort. In a real scenario, we might want to restart the app
        // or signal the main thread to unfreeze.
        PerformanceLogger::LogWarning("Attempting hook recovery...");
        XoneKeyManager::freeEngine();
        XoneKeyManager::initEngine();
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
    DWORD timeSinceLastHeartbeat = (currentTime >= _lastHeartbeatTime) ? 
        (currentTime - _lastHeartbeatTime) : ((MAXDWORD - _lastHeartbeatTime) + currentTime + 1);
    bool healthy = timeSinceLastHeartbeat < HEARTBEAT_TIMEOUT_MS;
    LeaveCriticalSection(&_cs);
    return healthy;
}

