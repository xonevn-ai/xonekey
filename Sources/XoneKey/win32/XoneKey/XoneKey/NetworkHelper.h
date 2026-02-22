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
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

/**
 * Network Helper using WinHttp API
 * Provides better timeout control and cancellation support
 */
class NetworkHelper {
private:
    static const DWORD DEFAULT_CONNECT_TIMEOUT_MS = 10000;  // 10 seconds
    static const DWORD DEFAULT_SEND_TIMEOUT_MS = 10000;    // 10 seconds
    static const DWORD DEFAULT_RECEIVE_TIMEOUT_MS = 30000; // 30 seconds
    
public:
    /**
     * Download content from URL with proper timeout handling
     * @param url URL to download from
     * @param timeoutMs Timeout in milliseconds (0 = use default)
     * @return Downloaded content as wide string, empty on failure
     */
    static std::wstring DownloadContent(const std::wstring& url, DWORD timeoutMs = 0);
    
    /**
     * Download content from URL to file
     * @param url URL to download from
     * @param filePath Path to save file
     * @param timeoutMs Timeout in milliseconds (0 = use default)
     * @return true on success, false on failure
     */
    static bool DownloadToFile(const std::wstring& url, const std::wstring& filePath, DWORD timeoutMs = 0);
    
private:
    static bool ParseUrl(const std::wstring& url, std::wstring& scheme, 
                        std::wstring& host, std::wstring& path, INTERNET_PORT& port);
    static std::wstring GetLastErrorString();
};

