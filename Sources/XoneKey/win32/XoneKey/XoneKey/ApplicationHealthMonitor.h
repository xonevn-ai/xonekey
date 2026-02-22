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
#include "stdafx.h"

/**
 * Application Health Monitor
 * Monitors application health with heartbeat mechanism and watchdog
 */
class ApplicationHealthMonitor {
private:
    static ApplicationHealthMonitor* _instance;
    UINT_PTR _heartbeatTimerId;
    DWORD _lastHeartbeatTime;
    DWORD _startTime;
    bool _isMonitoring;
    mutable CRITICAL_SECTION _cs;  // Mutable to allow const member functions to lock
    
    enum {
        HEARTBEAT_INTERVAL_MS = 30000, // 30 seconds
        HEARTBEAT_TIMEOUT_MS = 60000   // 60 seconds (2 missed heartbeats)
    };
    
    ApplicationHealthMonitor();
    ~ApplicationHealthMonitor();
    
    static VOID CALLBACK HeartbeatTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
    void UpdateHeartbeat();
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

