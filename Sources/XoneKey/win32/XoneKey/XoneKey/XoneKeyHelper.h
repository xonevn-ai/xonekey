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

extern int CF_RTF;
extern int CF_HTML;
extern int CF_XONEKEY;

class XoneKeyHelper {
private:
	static void openKey();
public:
	static void setRegInt(LPCTSTR key, const int& val);
	static int getRegInt(LPCTSTR key, const int& defaultValue);

	static void setRegBinary(LPCTSTR key, const BYTE* pData, const int& size);
	static BYTE* getRegBinary(LPCTSTR key, DWORD& outSize);

	static void registerRunOnStartup(const int& val);

	static LPTSTR getExecutePath();

	static string getFrontMostAppExecuteName();
	static string getLastAppExecuteName();

	static wstring getFullPath();

	static wstring getClipboardText(const int& type);
	static bool setClipboardText(LPCTSTR data, const int& len, const int& type);

	static bool quickConvert();


	// Error logging
	static void LogError(const std::string& message);
	static std::string GetLastErrorAsString();

    
    // Safe versions of existing functions
    static bool SafeSetRegInt(LPCTSTR key, const int& val);
    static int SafeGetRegInt(LPCTSTR key, const int& defaultValue);
    static bool SafeSetRegBinary(LPCTSTR key, const BYTE* pData, const int& size);
    
    // Application path utilities
    static std::wstring GetAppDataPath();
    static std::wstring GetLogPath();

	static DWORD getVersionNumber();
	static wstring getVersionString();

	static void StartAsyncUpdateCheck(HWND hDlg, HWND hButton);

	static void initialize();
	static void cleanup();

	static wstring getContentOfUrl(LPCTSTR url);
};

DWORD WINAPI SimpleCheckUpdateThreadProc(LPVOID lpParam);
DWORD WINAPI CheckUpdateThreadProc(LPVOID lpParam);
