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
#include "NetworkHelper.h"
#include "PerformanceLogger.h"
#include "XoneKeyHelper.h"
#include <sstream>

std::wstring NetworkHelper::DownloadContent(const std::wstring& url, DWORD timeoutMs)
{
    PERF_TIMER("NetworkHelper::DownloadContent");
    
    if (url.empty())
    {
        PerformanceLogger::LogError("DownloadContent: Empty URL provided");
        return L"";
    }
    
    if (timeoutMs == 0)
    {
        timeoutMs = DEFAULT_RECEIVE_TIMEOUT_MS;
    }
    
    // Parse URL
    std::wstring scheme, host, path;
    INTERNET_PORT port = INTERNET_DEFAULT_PORT;
    
    if (!ParseUrl(url, scheme, host, path, port))
    {
        PerformanceLogger::LogError("DownloadContent: Failed to parse URL");
        return L"";
    }
    
    // Only support HTTP/HTTPS
    if (scheme != L"http" && scheme != L"https")
    {
        std::stringstream ss;
        ss << "DownloadContent: Unsupported scheme: " << wideStringToUtf8(scheme);
        PerformanceLogger::LogError(ss.str());
        return L"";
    }
    
    HINTERNET hSession = NULL;
    HINTERNET hConnect = NULL;
    HINTERNET hRequest = NULL;
    std::wstring result;
    
    try
    {
        // Initialize WinHttp
        hSession = WinHttpOpen(
            L"XoneKey/1.0",
            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
            WINHTTP_NO_PROXY_NAME,
            WINHTTP_NO_PROXY_BYPASS,
            0
        );
        
        if (hSession == NULL)
        {
            PerformanceLogger::LogError("DownloadContent: WinHttpOpen failed - " + 
                                       wideStringToUtf8(GetLastErrorString()));
            return L"";
        }
        
        // Set timeouts
        DWORD connectTimeout = timeoutMs;
        DWORD sendTimeout = DEFAULT_SEND_TIMEOUT_MS;
        DWORD receiveTimeout = timeoutMs;
        
        if (!WinHttpSetTimeouts(hSession, connectTimeout, sendTimeout, receiveTimeout, receiveTimeout))
        {
            PerformanceLogger::LogWarning("DownloadContent: WinHttpSetTimeouts failed");
        }
        
        // Connect to server
        hConnect = WinHttpConnect(hSession, host.c_str(), port, 0);
        if (hConnect == NULL)
        {
            PerformanceLogger::LogError("DownloadContent: WinHttpConnect failed - " + 
                                       wideStringToUtf8(GetLastErrorString()));
            goto cleanup;
        }
        
        // Create request
        DWORD dwFlags = WINHTTP_FLAG_REFRESH;
        if (scheme == L"https")
        {
            dwFlags |= WINHTTP_FLAG_SECURE;
        }
        
        hRequest = WinHttpOpenRequest(
            hConnect,
            L"GET",
            path.empty() ? L"/" : path.c_str(),
            NULL,
            WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            dwFlags
        );
        
        if (hRequest == NULL)
        {
            PerformanceLogger::LogError("DownloadContent: WinHttpOpenRequest failed - " + 
                                       wideStringToUtf8(GetLastErrorString()));
            goto cleanup;
        }
        
        // Send request
        if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, 
                                WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
        {
            PerformanceLogger::LogError("DownloadContent: WinHttpSendRequest failed - " + 
                                       wideStringToUtf8(GetLastErrorString()));
            goto cleanup;
        }
        
        // Wait for response
        if (!WinHttpReceiveResponse(hRequest, NULL))
        {
            PerformanceLogger::LogError("DownloadContent: WinHttpReceiveResponse failed - " + 
                                       wideStringToUtf8(GetLastErrorString()));
            goto cleanup;
        }
        
        // Check status code
        DWORD dwStatusCode = 0;
        DWORD dwStatusCodeSize = sizeof(dwStatusCode);
        if (!WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                                WINHTTP_HEADER_NAME_BY_INDEX, &dwStatusCode, &dwStatusCodeSize, WINHTTP_NO_HEADER_INDEX))
        {
            PerformanceLogger::LogError("DownloadContent: WinHttpQueryHeaders failed");
            goto cleanup;
        }
        
        if (dwStatusCode != HTTP_STATUS_OK)
        {
            std::stringstream ss;
            ss << "DownloadContent: HTTP error " << dwStatusCode;
            PerformanceLogger::LogError(ss.str());
            goto cleanup;
        }
        
        // Read response data
        DWORD dwBytesAvailable = 0;
        std::vector<BYTE> buffer;
        
        do
        {
            if (!WinHttpQueryDataAvailable(hRequest, &dwBytesAvailable))
            {
                PerformanceLogger::LogError("DownloadContent: WinHttpQueryDataAvailable failed");
                break;
            }
            
            if (dwBytesAvailable == 0)
                break;
            
            size_t oldSize = buffer.size();
            buffer.resize(oldSize + dwBytesAvailable);
            
            DWORD dwBytesRead = 0;
            if (!WinHttpReadData(hRequest, &buffer[oldSize], dwBytesAvailable, &dwBytesRead))
            {
                PerformanceLogger::LogError("DownloadContent: WinHttpReadData failed");
                break;
            }
            
            if (dwBytesRead == 0)
                break;
            
            // Resize if we read less than available
            if (dwBytesRead < dwBytesAvailable)
            {
                buffer.resize(oldSize + dwBytesRead);
            }
        } while (dwBytesAvailable > 0);
        
        // Convert to wide string (assuming UTF-8 content)
        if (!buffer.empty())
        {
            std::string utf8Content(reinterpret_cast<char*>(buffer.data()), buffer.size());
            result = utf8ToWideString(utf8Content);
        }
    }
    catch (const std::exception& e)
    {
        std::stringstream ss;
        ss << "DownloadContent: Exception - " << e.what();
        PerformanceLogger::LogError(ss.str());
    }
    
cleanup:
    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
    
    return result;
}

bool NetworkHelper::DownloadToFile(const std::wstring& url, const std::wstring& filePath, DWORD timeoutMs)
{
    PERF_TIMER("NetworkHelper::DownloadToFile");
    
    std::wstring content = DownloadContent(url, timeoutMs);
    if (content.empty())
    {
        return false;
    }
    
    // Write to file
    // Convert wide string path to narrow string for ofstream
    std::string narrowPath = wideStringToUtf8(filePath);
    std::ofstream file(narrowPath, std::ios::binary);
    if (!file.is_open())
    {
        PerformanceLogger::LogError("DownloadToFile: Failed to open file for writing");
        return false;
    }
    
    std::string utf8Content = wideStringToUtf8(content);
    file.write(utf8Content.c_str(), utf8Content.size());
    file.close();
    
    return true;
}

bool NetworkHelper::ParseUrl(const std::wstring& url, std::wstring& scheme, 
                            std::wstring& host, std::wstring& path, INTERNET_PORT& port)
{
    size_t schemeEnd = url.find(L"://");
    if (schemeEnd == std::wstring::npos)
    {
        return false;
    }
    
    scheme = url.substr(0, schemeEnd);
    size_t hostStart = schemeEnd + 3;
    
    size_t pathStart = url.find(L"/", hostStart);
    if (pathStart == std::wstring::npos)
    {
        host = url.substr(hostStart);
        path = L"/";
    }
    else
    {
        host = url.substr(hostStart, pathStart - hostStart);
        path = url.substr(pathStart);
    }
    
    // Check for port in host
    size_t portPos = host.find(L":");
    if (portPos != std::wstring::npos)
    {
        std::wstring portStr = host.substr(portPos + 1);
        host = host.substr(0, portPos);
        port = (INTERNET_PORT)_wtoi(portStr.c_str());
    }
    else
    {
        port = (scheme == L"https") ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT;
    }
    
    return true;
}

std::wstring NetworkHelper::GetLastErrorString()
{
    DWORD error = GetLastError();
    if (error == 0)
        return L"No error";
    
    LPWSTR messageBuffer = nullptr;
    DWORD size = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&messageBuffer,
        0,
        NULL
    );
    
    std::wstring message(messageBuffer, size);
    LocalFree(messageBuffer);
    
    return message;
}

