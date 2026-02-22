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
#pragma once
#include <windows.h>
#include <string>

/**
 * Application Health Monitor
 * Monitors application health with heartbeat mechanism and watchdog
 */
class ApplicationHealthMonitor {
private:
    static ApplicationHealthMonitor* _instance;
    HANDLE _hThread;
    HANDLE _hStopEvent;
    DWORD _lastHeartbeatTime;
    DWORD _startTime;
    bool _isMonitoring;
    mutable CRITICAL_SECTION _cs;
    
    enum {
        CHECK_INTERVAL_MS = 5000,      // Check every 5 seconds
        HEARTBEAT_TIMEOUT_MS = 60000   // 60 seconds (hang threshold)
    };
    
    ApplicationHealthMonitor();
    ~ApplicationHealthMonitor();
    
    static DWORD WINAPI MonitorThreadProc(LPVOID lpParam);
    void CheckHealth();
    
public:
    static ApplicationHealthMonitor* GetInstance();
    static void DestroyInstance();
    
    bool StartMonitoring();
    void StopMonitoring();
    void SignalHeartbeat();
    
    DWORD GetUptime() const;
    bool IsHealthy() const;
};

