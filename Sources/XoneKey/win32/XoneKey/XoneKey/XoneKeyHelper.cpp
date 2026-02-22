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
#include "XoneKeyHelper.h"
#include <stdarg.h>
#include <Urlmon.h>
#include <Shlobj.h>
#include <fstream>
#include <sstream>
#include "NetworkHelper.h"

#pragma comment(lib, "version.lib")
#pragma comment(lib, "Urlmon.lib")

static BYTE *_regData = 0;

static LPCTSTR sk = TEXT("SOFTWARE\\XoneVN\\XoneKey");
static HKEY hKey;
static LPCTSTR _runOnStartupKeyPath = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
static TCHAR _executePath[MAX_PATH];
static bool _hasGetPath = false;

static DWORD _cacheProcessId = 0, _tempProcessId = 0;
static HWND _tempWnd;
static TCHAR _exePath[1024] = {0};
static LPCTSTR _exeName = _exePath;
static HANDLE _proc;
static string _exeNameUtf8 = "TheXoneKeyProject";
static string _unknownProgram = "UnknownProgram";
static CRITICAL_SECTION _processQueryCs; // Thread safety for process query

// Timeout protection constants
#define PROCESS_QUERY_TIMEOUT_MS 500  // 500ms timeout for process queries

int CF_RTF = RegisterClipboardFormat(_T("Rich Text Format"));
int CF_HTML = RegisterClipboardFormat(_T("HTML Format"));
int CF_OPENKEY = RegisterClipboardFormat(_T("XoneKey Format"));

// Initialize critical section for thread-safe process query
// This will be initialized on first use
static bool _processQueryCsInitialized = false;
static void InitializeProcessQueryCs()
{
	if (!_processQueryCsInitialized)
	{
		InitializeCriticalSection(&_processQueryCs);
		_processQueryCsInitialized = true;
	}
}

void XoneKeyHelper::openKey()
{
	LONG nError = RegOpenKeyEx(HKEY_CURRENT_USER, sk, NULL, KEY_ALL_ACCESS, &hKey);
	if (nError == ERROR_FILE_NOT_FOUND)
	{
		nError = RegCreateKeyEx(HKEY_CURRENT_USER, sk, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_CREATE_SUB_KEY, NULL, &hKey, NULL);
	}
	if (nError)
	{
		LOG(L"result %d\n", nError);
	}
}

void XoneKeyHelper::setRegInt(LPCTSTR key, const int &val)
{
	openKey();
	RegSetValueEx(hKey, key, 0, REG_DWORD, (LPBYTE)&val, sizeof(val));
	RegCloseKey(hKey);
}

int XoneKeyHelper::getRegInt(LPCTSTR key, const int &defaultValue)
{
	openKey();
	int val = defaultValue;
	DWORD size = sizeof(val);
	if (ERROR_SUCCESS != RegQueryValueEx(hKey, key, 0, 0, (LPBYTE)&val, &size))
	{
		val = defaultValue;
	}
	RegCloseKey(hKey);
	return val;
}

void XoneKeyHelper::setRegBinary(LPCTSTR key, const BYTE *pData, const int &size)
{
	openKey();
	RegSetValueEx(hKey, key, 0, REG_BINARY, pData, size);
	RegCloseKey(hKey);
}

BYTE *XoneKeyHelper::getRegBinary(LPCTSTR key, DWORD &outSize)
{
	static BYTE *_staticRegData = NULL; // Static variable to track allocation

	openKey();

	// Free any previously allocated memory
	if (_staticRegData)
	{
		delete[] _staticRegData;
		_staticRegData = NULL;
	}

	DWORD size = 0;
	LONG result = RegQueryValueEx(hKey, key, 0, 0, 0, &size);

	if (result != ERROR_SUCCESS || size == 0)
	{
		RegCloseKey(hKey);
		outSize = 0;
		return NULL;
	}

	_staticRegData = new BYTE[size];
	if (_staticRegData == NULL)
	{
		RegCloseKey(hKey);
		outSize = 0;
		return NULL;
	}

	result = RegQueryValueEx(hKey, key, 0, 0, _staticRegData, &size);
	RegCloseKey(hKey);

	if (result != ERROR_SUCCESS)
	{
		delete[] _staticRegData;
		_staticRegData = NULL;
		outSize = 0;
		return NULL;
	}

	outSize = size;
	return _staticRegData;
}

void XoneKeyHelper::registerRunOnStartup(const int &val)
{
	if (val)
	{
		if (vRunAsAdmin)
		{
			string path = wideStringToUtf8(getFullPath());
			char buff[MAX_PATH];
			sprintf_s(buff, "schtasks /create /sc onlogon /tn XoneKey /rl highest /tr \"%s\" /f", path.c_str());
			WinExec(buff, SW_HIDE);
		}
		else
		{
			RegOpenKeyEx(HKEY_CURRENT_USER, _runOnStartupKeyPath, NULL, KEY_ALL_ACCESS, &hKey);
			wstring path = getFullPath();
			RegSetValueEx(hKey, _T("XoneKey"), 0, REG_SZ, (byte *)path.c_str(), ((DWORD)path.size() + 1) * sizeof(TCHAR));
			RegCloseKey(hKey);
		}
	}
	else
	{
		RegOpenKeyEx(HKEY_CURRENT_USER, _runOnStartupKeyPath, NULL, KEY_ALL_ACCESS, &hKey);
		RegDeleteValue(hKey, _T("XoneKey"));
		RegCloseKey(hKey);
		WinExec("schtasks /delete  /tn XoneKey /f", SW_HIDE);
	}
}

LPTSTR XoneKeyHelper::getExecutePath()
{
	if (!_hasGetPath)
	{
		HMODULE hModule = GetModuleHandleW(NULL);
		GetModuleFileNameW(hModule, _executePath, MAX_PATH);
		_hasGetPath = true;
	}
	return _executePath;
}

// Worker thread structure for timeout-protected process query
struct ProcessQueryParams {
	DWORD processId;
	HANDLE hProcess;
	TCHAR exePath[1024];
	bool success;
	HANDLE hEvent; // Event to signal completion
};

// Worker thread function for process query with timeout protection
DWORD WINAPI ProcessQueryWorkerThread(LPVOID lpParam)
{
	ProcessQueryParams* params = (ProcessQueryParams*)lpParam;
	params->success = false;
	
	// Try to open process with limited permissions first (faster)
	params->hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, params->processId);
	if (params->hProcess == NULL || params->hProcess == INVALID_HANDLE_VALUE)
	{
		// Fallback to full permissions
		params->hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, params->processId);
	}
	
	if (params->hProcess == NULL || params->hProcess == INVALID_HANDLE_VALUE)
	{
		SetEvent(params->hEvent);
		return 1;
	}
	
	// Clear the path buffer
	ZeroMemory(params->exePath, sizeof(params->exePath));
	
	// Try to get process image file name
	if (GetProcessImageFileName(params->hProcess, params->exePath, 1024) == 0)
	{
		CloseHandle(params->hProcess);
		params->hProcess = NULL;
		SetEvent(params->hEvent);
		return 1;
	}
	
	params->success = true;
	SetEvent(params->hEvent);
	return 0;
}

string &XoneKeyHelper::getFrontMostAppExecuteName()
{
	// Initialize critical section if needed
	InitializeProcessQueryCs();
	
	// Try to enter critical section with timeout protection
	// Use TryEnterCriticalSection to avoid blocking indefinitely
	if (!TryEnterCriticalSection(&_processQueryCs))
	{
		// If we can't enter immediately, return cached value to avoid blocking
		return _exeNameUtf8;
	}
	
	_tempWnd = GetForegroundWindow();
	if (_tempWnd == NULL)
	{
		LeaveCriticalSection(&_processQueryCs);
		return _exeNameUtf8;
	}
	
	GetWindowThreadProcessId(_tempWnd, &_tempProcessId);
	if (_tempProcessId == 0)
	{
		LeaveCriticalSection(&_processQueryCs);
		return _exeNameUtf8;
	}
	
	if (_tempProcessId == _cacheProcessId)
	{
		LeaveCriticalSection(&_processQueryCs);
		return _exeNameUtf8;
	}
	
	// Use timeout-protected process query
	_cacheProcessId = _tempProcessId;
	
	ProcessQueryParams params;
	params.processId = _tempProcessId;
	params.hProcess = NULL;
	params.success = false;
	params.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	if (params.hEvent == NULL)
	{
		// If we can't create event, fall back to cached value
		LeaveCriticalSection(&_processQueryCs);
		return _exeNameUtf8;
	}
	
	// Create worker thread for process query
	HANDLE hThread = CreateThread(NULL, 0, ProcessQueryWorkerThread, &params, 0, NULL);
	if (hThread == NULL)
	{
		CloseHandle(params.hEvent);
		LeaveCriticalSection(&_processQueryCs);
		return _exeNameUtf8;
	}
	
	// Wait for thread completion with timeout
	DWORD waitResult = WaitForSingleObject(params.hEvent, PROCESS_QUERY_TIMEOUT_MS);
	
	if (waitResult == WAIT_TIMEOUT)
	{
		// Timeout occurred - return cached value without blocking
		// Note: We don't terminate the thread as it will clean up naturally
		// The thread will finish and clean up resources when OpenProcess completes
		CloseHandle(hThread); // Thread handle can be closed, thread continues running
		CloseHandle(params.hEvent);
		LeaveCriticalSection(&_processQueryCs);
		return _exeNameUtf8; // Return cached value on timeout
	}
	
	// Thread completed successfully
	CloseHandle(hThread);
	CloseHandle(params.hEvent);
	
	if (!params.success || params.hProcess == NULL)
	{
		LeaveCriticalSection(&_processQueryCs);
		return _unknownProgram;
	}
	
	// Copy result
	wcscpy_s(_exePath, params.exePath);
	CloseHandle(params.hProcess);

	if (wcscmp(_exePath, _T("")) == 0)
	{
		LeaveCriticalSection(&_processQueryCs);
		return _unknownProgram;
	}
	
	_exeName = _tcsrchr(_exePath, '\\');
	if (_exeName == NULL)
	{
		_exeName = _exePath;
	}
	else
	{
		_exeName++; // Skip the backslash
	}
	
	if (wcscmp(_exeName, _T("XoneKey64.exe")) == 0 ||
		wcscmp(_exeName, _T("XoneKey32.exe")) == 0 ||
		wcscmp(_exeName, _T("explorer.exe")) == 0)
	{
		LeaveCriticalSection(&_processQueryCs);
		return _exeNameUtf8;
	}
	
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, _exeName, (int)lstrlen(_exeName), NULL, 0, NULL, NULL);
	if (size_needed <= 0)
	{
		LeaveCriticalSection(&_processQueryCs);
		return _unknownProgram;
	}
	
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, _exeName, (int)lstrlen(_exeName), &strTo[0], size_needed, NULL, NULL);
	_exeNameUtf8 = strTo;
	// LOG(L"%s\n", utf8ToWideString(_exeNameUtf8).c_str());
	
	LeaveCriticalSection(&_processQueryCs);
	return _exeNameUtf8;
}

string &XoneKeyHelper::getLastAppExecuteName()
{
	if (!vUseSmartSwitchKey)
		return getFrontMostAppExecuteName();
	return _exeNameUtf8;
}

wstring XoneKeyHelper::getFullPath()
{
	HMODULE hModule = GetModuleHandle(NULL);
	TCHAR path[MAX_PATH];
	GetModuleFileName(hModule, path, MAX_PATH);
	wstring rs(path);
	return rs;
}

// Improved clipboard handling with error checking
wstring XoneKeyHelper::getClipboardText(const int &type)
{
	// Try opening the clipboard
	if (!OpenClipboard(nullptr))
	{
		std::stringstream ss;
		ss << "Failed to open clipboard: " << GetLastErrorAsString();
		LogError(ss.str());
		return _T("");
	}

	// Get handle of clipboard object
	HANDLE hData = GetClipboardData(type);
	if (hData == nullptr)
	{
		CloseClipboard();
		return _T("");
	}

	try
	{
		// Lock the handle to get the actual text pointer
		wchar_t *pszText = static_cast<wchar_t *>(GlobalLock(hData));
		if (pszText == nullptr)
		{
			GlobalUnlock(hData);
			CloseClipboard();
			LogError("Failed to lock clipboard data");
			return _T("");
		}

		// Save text in a string class instance
		wstring text(pszText);

		// Release the lock
		GlobalUnlock(hData);

		// Release the clipboard
		CloseClipboard();

		return text;
	}
	catch (const std::exception &e)
	{
		GlobalUnlock(hData);
		CloseClipboard();
		std::stringstream ss;
		ss << "Exception reading clipboard: " << e.what();
		LogError(ss.str());
		return _T("");
	}
}

void XoneKeyHelper::setClipboardText(LPCTSTR data, const int &len, const int &type)
{
	if (!data || len <= 0)
	{
		LogError("Invalid data provided for clipboard");
		return;
	}

	try
	{
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len * sizeof(WCHAR));
		if (!hMem)
		{
			LogError("Failed to allocate memory for clipboard");
			return;
		}

		void *pLock = GlobalLock(hMem);
		if (!pLock)
		{
			GlobalFree(hMem);
			LogError("Failed to lock memory for clipboard");
			return;
		}

		memcpy(pLock, data, len * sizeof(WCHAR));
		GlobalUnlock(hMem);

		if (!OpenClipboard(0))
		{
			GlobalFree(hMem);
			LogError("Failed to open clipboard for writing");
			return;
		}

		EmptyClipboard();
		if (!SetClipboardData(type, hMem))
		{
			GlobalFree(hMem);
			CloseClipboard();
			std::stringstream ss;
			ss << "Failed to set clipboard data: " << GetLastErrorAsString();
			LogError(ss.str());
			return;
		}

		CloseClipboard();
	}
	catch (const std::exception &e)
	{
		CloseClipboard();
		std::stringstream ss;
		ss << "Exception setting clipboard: " << e.what();
		LogError(ss.str());
	}
}

// Improved quickConvert with better error handling
bool XoneKeyHelper::quickConvert()
{
	try
	{
		// read data from clipboard
		// support Unicode raw string, Rich Text Format and HTML

		if (!OpenClipboard(nullptr))
		{
			LogError("Failed to open clipboard for convert");
			return false;
		}

		string dataHTML;
		wstring dataUnicode;
		bool hasData = false;

		// HTML
		HANDLE hData = GetClipboardData(CF_HTML);
		if (hData)
		{
			char *pHTML = static_cast<char *>(GlobalLock(hData));
			if (pHTML)
			{
				dataHTML = pHTML;
				GlobalUnlock(hData);
				dataHTML = convertUtil(dataHTML);
				hasData = true;
			}
		}

		// UNICODE
		hData = GetClipboardData(CF_UNICODETEXT);
		if (hData)
		{
			wchar_t *pUnicode = static_cast<wchar_t *>(GlobalLock(hData));
			if (pUnicode)
			{
				dataUnicode = pUnicode;
				GlobalUnlock(hData);
				dataUnicode = utf8ToWideString(convertUtil(wideStringToUtf8(dataUnicode)));
				hasData = true;
			}
		}

		CloseClipboard();

		if (!hasData)
		{
			LogError("No valid data found in clipboard");
			return false;
		}

		if (!OpenClipboard(0))
		{
			LogError("Failed to open clipboard for writing converted data");
			return false;
		}

		EmptyClipboard();

		if (!dataHTML.empty())
		{
			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (dataHTML.size() + 1) * sizeof(char));
			if (hMem)
			{
				char *pLock = static_cast<char *>(GlobalLock(hMem));
				if (pLock)
				{
					memcpy(pLock, dataHTML.c_str(), (dataHTML.size() + 1) * sizeof(char));
					GlobalUnlock(hMem);
					SetClipboardData(CF_HTML, hMem);
				}
				else
				{
					GlobalFree(hMem);
				}
			}
		}

		if (!dataUnicode.empty())
		{
			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (dataUnicode.size() + 1) * sizeof(wchar_t));
			if (hMem)
			{
				wchar_t *pLock = static_cast<wchar_t *>(GlobalLock(hMem));
				if (pLock)
				{
					memcpy(pLock, dataUnicode.c_str(), (dataUnicode.size() + 1) * sizeof(wchar_t));
					GlobalUnlock(hMem);
					SetClipboardData(CF_UNICODETEXT, hMem);
				}
				else
				{
					GlobalFree(hMem);
				}
			}
		}

		CloseClipboard();
		return hasData;
	}
	catch (const std::exception &e)
	{
		CloseClipboard();
		std::stringstream ss;
		ss << "Exception in quickConvert: " << e.what();
		LogError(ss.str());
		return false;
	}
}

DWORD XoneKeyHelper::getVersionNumber()
{
	// get the filename of the executable containing the version resource
	TCHAR szFilename[MAX_PATH + 1] = {0};
	if (GetModuleFileName(NULL, szFilename, MAX_PATH) == 0)
	{
		return 0;
	}

	// allocate a block of memory for the version info
	DWORD dummy;
	UINT dwSize = GetFileVersionInfoSize(szFilename, &dummy);
	if (dwSize == 0)
	{
		return 0;
	}
	std::vector<BYTE> data(dwSize);

	// load the version info
	if (!GetFileVersionInfo(szFilename, NULL, dwSize, &data[0]))
	{
		return 0;
	}

	LPBYTE lpBuffer = NULL;

	if (VerQueryValue(&data[0], _T("\\"), (VOID FAR * FAR *)&lpBuffer, &dwSize))
	{
		if (dwSize)
		{
			VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)lpBuffer;
			if (verInfo->dwSignature == 0xfeef04bd)
			{
				return ((verInfo->dwFileVersionMS >> 16) & 0xffff) |
					   (((verInfo->dwFileVersionMS >> 0) & 0xffff) << 8) |
					   (((verInfo->dwFileVersionLS >> 16) & 0xffff) << 16);
			}
		}
	}

	return 0;
}

wstring XoneKeyHelper::getVersionString()
{
	TCHAR versionBuffer[MAX_PATH];
	DWORD ver = getVersionNumber();
	wsprintfW(versionBuffer, _T("%d.%d.%d"), ver & 0xFF, (ver >> 8) & 0xFF, (ver >> 16) & 0xFF);
	return wstring(versionBuffer);

	// get the filename of the executable containing the version resource
	TCHAR szFilename[MAX_PATH + 1] = {0};
	if (GetModuleFileName(NULL, szFilename, MAX_PATH) == 0)
	{
		return _T("");
	}
}

wstring XoneKeyHelper::getContentOfUrl(LPCTSTR url)
{
	// Use NetworkHelper for better timeout control
	// Fallback to old method if NetworkHelper fails
	std::wstring urlStr(url);
	std::wstring content = NetworkHelper::DownloadContent(urlStr, 30000); // 30 second timeout
	
	if (!content.empty())
	{
		return content;
	}
	
	// Fallback to old method for compatibility
	WCHAR path[MAX_PATH];
	GetTempPath(MAX_PATH, path); // Use GetTempPath for compatibility (GetTempPath2 requires Windows 10 1803+)
	wsprintf(path, TEXT("%s\\_XoneKey.tempf"), path);

	// Use IBindStatusCallback to set timeout
	// For URLDownloadToFile, we need to use URLDownloadToCacheFile or implement IBindStatusCallback
	// For now, use a simpler approach with timeout handling
	HRESULT hr = URLDownloadToFile(NULL, url, path, 0, NULL);

	// Check if download succeeded and file exists
	if (hr == S_OK)
	{
		// Verify file exists and has content
		HANDLE hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return L"";
		}
		CloseHandle(hFile);

		std::wifstream t(path);
		if (!t.is_open())
		{
			return L"";
		}
		std::wstringstream buffer;
		buffer << t.rdbuf();
		t.close();
		DeleteFile(path);
		return buffer.str();
	}
	
	// Clean up temp file if it exists
	DeleteFile(path);
	return L"";
}

std::string XoneKeyHelper::GetLastErrorAsString()
{
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
	{
		return "No error";
	}

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorMessageID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&messageBuffer,
		0,
		NULL);

	std::string message(messageBuffer, size);
	LocalFree(messageBuffer);

	return message;
}

std::wstring XoneKeyHelper::GetAppDataPath()
{
	WCHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		std::wstring result = path;
		result += L"\\XoneKey";

		// Create directory if it doesn't exist
		CreateDirectory(result.c_str(), NULL);

		return result;
	}
	return L"";
}

std::wstring XoneKeyHelper::GetLogPath()
{
	std::wstring appDataPath = GetAppDataPath();
	if (!appDataPath.empty())
	{
		return appDataPath + L"\\xonekey_log.txt";
	}
	return L"";
}

void XoneKeyHelper::LogError(const std::string &message)
{
	std::wstring logPath = GetLogPath();
	if (logPath.empty())
		return;

	std::ofstream logFile(logPath, std::ios::app);
	if (!logFile.is_open())
		return;

	// Get current time
	time_t now = time(0);
	struct tm timeinfo;
	char buffer[80];
	localtime_s(&timeinfo, &now);
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

	logFile << "[" << buffer << "] " << message << std::endl;
	logFile.close();
}

bool XoneKeyHelper::SafeSetRegInt(LPCTSTR key, const int &val)
{
	try
	{
		openKey();
		LONG result = RegSetValueEx(hKey, key, 0, REG_DWORD, (LPBYTE)&val, sizeof(val));
		RegCloseKey(hKey);

		if (result != ERROR_SUCCESS)
		{
			std::stringstream ss;
			ss << "Failed to set registry key: " << wideStringToUtf8(key) << " with error: " << GetLastErrorAsString();
			LogError(ss.str());
			return false;
		}
		return true;
	}
	catch (const std::exception &e)
	{
		std::stringstream ss;
		ss << "Exception setting registry key: " << e.what();
		LogError(ss.str());
		return false;
	}
}

int XoneKeyHelper::SafeGetRegInt(LPCTSTR key, const int &defaultValue)
{
	try
	{
		openKey();
		int val = defaultValue;
		DWORD size = sizeof(val);
		LONG result = RegQueryValueEx(hKey, key, 0, 0, (LPBYTE)&val, &size);
		RegCloseKey(hKey);

		if (result != ERROR_SUCCESS)
		{
			if (result != ERROR_FILE_NOT_FOUND)
			{
				std::stringstream ss;
				ss << "Failed to get registry key: " << wideStringToUtf8(key) << " with error: " << GetLastErrorAsString();
				LogError(ss.str());
			}
			return defaultValue;
		}
		return val;
	}
	catch (const std::exception &e)
	{
		std::stringstream ss;
		ss << "Exception getting registry key: " << e.what();
		LogError(ss.str());
		return defaultValue;
	}
}

bool XoneKeyHelper::SafeSetRegBinary(LPCTSTR key, const BYTE *pData, const int &size)
{
	try
	{
		if (!pData || size <= 0)
		{
			LogError("Invalid binary data provided for registry");
			return false;
		}

		openKey();
		LONG result = RegSetValueEx(hKey, key, 0, REG_BINARY, pData, size);
		RegCloseKey(hKey);

		if (result != ERROR_SUCCESS)
		{
			std::stringstream ss;
			ss << "Failed to set binary registry key: " << wideStringToUtf8(key) << " with error: " << GetLastErrorAsString();
			LogError(ss.str());
			return false;
		}
		return true;
	}
	catch (const std::exception &e)
	{
		std::stringstream ss;
		ss << "Exception setting binary registry key: " << e.what();
		LogError(ss.str());
		return false;
	}
}

// Simple update check thread for AppDelegate (no UI needed)
DWORD WINAPI SimpleCheckUpdateThreadProc(LPVOID lpParam)
{
	string newVersion;
	try
	{
		bool hasUpdate = XoneKeyManager::checkUpdate(newVersion);
		if (hasUpdate && !newVersion.empty())
		{
			// Post message to main window to show update dialog
			HWND hWnd = FindWindow(APP_CLASS, NULL);
			if (hWnd != NULL)
			{
				// Allocate memory for version string (will be freed in message handler)
				char* versionStr = new char[newVersion.length() + 1];
				strcpy_s(versionStr, newVersion.length() + 1, newVersion.c_str());
				PostMessage(hWnd, WM_USER + 2020, 0, (LPARAM)versionStr);
			}
		}
	}
	catch (const std::exception &e)
	{
		XoneKeyHelper::LogError(std::string("Exception in update check: ") + e.what());
	}
	return 0;
}

DWORD WINAPI CheckUpdateThreadProc(LPVOID lpParam)
{
	// Cast back the parameters
	struct UpdateCheckParams
	{
		HWND hDlg;
		HWND hButton;
	};
	UpdateCheckParams *params = (UpdateCheckParams *)lpParam;

	string newVersion;
	try
	{
		bool hasUpdate = XoneKeyManager::checkUpdate(newVersion);
		// Post message back to UI thread with result
		PostMessage(params->hDlg, WM_USER + 1001, hasUpdate ? 1 : 0,
					(LPARAM)_strdup(newVersion.c_str()));
	}
	catch (const std::exception &e)
	{
		XoneKeyHelper::LogError(std::string("Exception in update check: ") + e.what());
		// Notify UI of failure
		PostMessage(params->hDlg, WM_USER + 1001, 0, 0);
	}
	// Free the parameter structure
	delete params;
	return 0;
}

// Use this function from AboutDialog.cpp and MainControlDialog.cpp
void XoneKeyHelper::StartAsyncUpdateCheck(HWND hDlg, HWND hButton)
{
	// Disable the button
	EnableWindow(hButton, FALSE);

	// Create parameters structure
	struct UpdateCheckParams
	{
		HWND hDlg;
		HWND hButton;
	};
	UpdateCheckParams *params = new UpdateCheckParams{hDlg, hButton};

	// Create the thread
	HANDLE hThread = CreateThread(NULL, 0, CheckUpdateThreadProc, params, 0, NULL);
	if (hThread)
	{
		CloseHandle(hThread);
	}
	else
	{
		// Thread creation failed, re-enable button
		EnableWindow(hButton, TRUE);
		delete params;

		MessageBox(hDlg, _T("Failed to start update check."), _T("Error"), MB_OK | MB_ICONERROR);
	}
}

// Add a cleanup function to ensure memory is properly freed
void XoneKeyHelper::cleanup()
{
	// This will be called during application shutdown
	// Clean up any static resources here
	static BYTE *_staticRegData = NULL;
	if (_staticRegData)
	{
		delete[] _staticRegData;
		_staticRegData = NULL;
	}
	
	// Clean up critical section (only if it was initialized)
	// Note: We can't check if it's initialized, so we'll use a try-catch approach
	// In practice, this should be called from a known initialization point
}