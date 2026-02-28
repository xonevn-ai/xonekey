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
#include "AppDelegate.h"
#include <sstream>

ApplicationHealthMonitor* ApplicationHealthMonitor::_instance = nullptr;

ApplicationHealthMonitor::ApplicationHealthMonitor()
    : _hThread(NULL)
    , _hStopEvent(NULL)
    , _lastHeartbeatTime(0)
    , _startTime(GetTickCount())
    , _isMonitoring(false)
{
    _hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

ApplicationHealthMonitor::~ApplicationHealthMonitor()
{
    StopMonitoring();
    if (_hStopEvent) CloseHandle(_hStopEvent);
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
    if (_isMonitoring)
    {
        return true;
    }
    
    InterlockedExchange(&_lastHeartbeatTime, (LONG)GetTickCount());
    _startTime = GetTickCount();
    ResetEvent(_hStopEvent);
    
    _hThread = CreateThread(NULL, 0, MonitorThreadProc, this, 0, NULL);
    
    if (_hThread == NULL)
    {
        XoneKeyHelper::LogError("Failed to create health monitor thread");
        return false;
    }
    
    _isMonitoring = true;
    
    PerformanceLogger::LogInfo("Application health monitoring thread started");
    return true;
}

void ApplicationHealthMonitor::StopMonitoring()
{
    if (!_isMonitoring)
    {
        return;
    }
    
    SetEvent(_hStopEvent);
    
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
    // Use atomic exchange to avoid lock contention in the hook thread
    InterlockedExchange(&_lastHeartbeatTime, (LONG)GetTickCount());
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
    DWORD currentTime = GetTickCount();
    DWORD lastHeartbeat = (DWORD)InterlockedCompareExchange(&_lastHeartbeatTime, 0, 0);
    // If it was 0, it means no heartbeat yet, use start time
    if (lastHeartbeat == 0) lastHeartbeat = _startTime;

    DWORD timeSinceLastHeartbeat = 0;
    
    if (currentTime >= lastHeartbeat)
    {
        timeSinceLastHeartbeat = currentTime - lastHeartbeat;
    }
    else
    {
        // Overflow occurred
        timeSinceLastHeartbeat = (MAXDWORD - lastHeartbeat) + currentTime + 1;
    }
    
    if (timeSinceLastHeartbeat > HEARTBEAT_TIMEOUT_MS)
    {
        std::stringstream ss;
        ss << "CRITICAL: Application hang detected! No heartbeat for " 
           << timeSinceLastHeartbeat << "ms (Threshold: " << HEARTBEAT_TIMEOUT_MS << "ms)";
        PerformanceLogger::LogError(ss.str());
        
        // Attempt recovery: re-initialize hooks by signaling the main UI thread
        // We must do this on the UI thread because hooks require a message loop
        PerformanceLogger::LogWarning("Requesting hook recovery on UI thread via message...");
        
        // Since we don't have a reliable way to get main thread ID easily without adding it,
        // let's use the AppDelegate's main thread if possible, or post to the main window.
        // Actually, we can store the main thread ID in AppDelegate during startup.
        
        // Alternative: find the main window and post to it
        HWND hWnd = NULL;
        if (AppDelegate::getInstance()) {
            hWnd = AppDelegate::getInstance()->getMainDialogHwnd();
        }
        
        
        if (hWnd) {
            PostMessage(hWnd, WM_APP_RECOVER_HOOKS, 0, 0);
        } else if (g_mainThreadId != 0) {
            PostThreadMessage(g_mainThreadId, WM_APP_RECOVER_HOOKS, 0, 0);
        } else {
            PerformanceLogger::LogError("Recovery failed: Main window and thread ID not found");
            XoneKeyHelper::NotifyUserOfError(L"Lỗi: Không thể tự động khôi phục bộ gõ. Vui lòng khởi động lại ứng dụng.");
        }
        
        // Reset last heartbeat time to avoid immediate re-trigger
        SignalHeartbeat();
    }
}

DWORD ApplicationHealthMonitor::GetUptime() const
{
    return GetTickCount() - _startTime;
}

bool ApplicationHealthMonitor::IsHealthy() const
{
    DWORD currentTime = GetTickCount();
    DWORD lastHeartbeat = (DWORD)InterlockedCompareExchange((volatile LONG*)&_lastHeartbeatTime, 0, 0);
    if (lastHeartbeat == 0) lastHeartbeat = _startTime;
    
    DWORD timeSinceLastHeartbeat = (currentTime >= lastHeartbeat) ? 
        (currentTime - lastHeartbeat) : ((MAXDWORD - lastHeartbeat) + currentTime + 1);
        
    return timeSinceLastHeartbeat < HEARTBEAT_TIMEOUT_MS;
}

