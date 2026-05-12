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
#include "AppDelegate.h"
#include "ToastNotification.h"
#include "ApplicationHealthMonitor.h"
#include "PerformanceLogger.h"
#include "XoneKeyManager.h"
#include "XoneKeyHelper.h"
#include "SystemTrayHelper.h"
#include "MainControlDialog.h"
#include "MacroDialog.h"
#include "ConvertToolDialog.h"
#include "AboutDialog.h"

static AppDelegate* _instance;
DWORD g_mainThreadId = 0;

//see document in Engine.h
int vLanguage = 1;
int vInputType = 0;
int vFreeMark = 0;
int vCodeTable = 0;
int vCheckSpelling = 1;
int vUseModernOrthography = 1;
int vQuickTelex = 0;
#define DEFAULT_SWITCH_STATUS 0x5A00025A //default option + z
int vSwitchKeyStatus = DEFAULT_SWITCH_STATUS;
int vRestoreIfWrongSpelling = 1;
int vFixRecommendBrowser = 0;
int vUseMacro = 1;
int vUseMacroInEnglishMode = 1;
int vAutoCapsMacro = 0;
int vSendKeyStepByStep = 1;
int vUseSmartSwitchKey = 1;
int vUpperCaseFirstChar = 0;
int vTempOffSpelling = 0;
int vAllowConsonantZFWJ = 0;
int vQuickStartConsonant = 0;
int vQuickEndConsonant = 0;
int vOtherLanguage = 1;
int vRememberCode = 1;
int vTempOffXoneKey = 0;

int vUseGrayIcon = 0;
int vShowOnStartUp = 0;
int vRunWithWindows = 1;

int vSupportMetroApp = 1;
int vCreateDesktopShortcut = 0;
int vRunAsAdmin = 0;
int vCheckNewVersion = 0;
//beta feature
int vFixChromiumBrowser = 0; //new on version 2.0

bool AppDelegate::isDialogMsg(MSG & msg) const {
	return (mainDialog != NULL && IsDialogMessage(mainDialog->getHwnd(), &msg)) ||
		(macroDialog != NULL && IsDialogMessage(macroDialog->getHwnd(), &msg)) || 
		(convertDialog != NULL && IsDialogMessage(convertDialog->getHwnd(), &msg)) || 
		(aboutDialog != NULL && IsDialogMessage(aboutDialog->getHwnd(), &msg));
}

void AppDelegate::checkUpdate() {
	string newVersion;
	if (XoneKeyManager::checkUpdate(newVersion)) {
		WCHAR msg[512];
		wsprintf(msg,
			TEXT("Phiên bản mới: %s đã sẵn sàng (hiện tại: %s). Bạn có muốn cập nhật không?"),
			utf8ToWideString(newVersion).c_str(),
			XoneKeyHelper::getVersionString().c_str());

		int msgboxID = MessageBox(
			0,
			msg,
			_T("XoneKey Update"),
			MB_ICONEXCLAMATION | MB_YESNO
		);
		if (msgboxID == IDYES) {
			//Call XoneKeyUpdate
			WCHAR path[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, path);
			wsprintf(path, TEXT("%s\\XoneKeyUpdate.exe"), path);
			ShellExecute(0, L"", path, 0, 0, SW_SHOWNORMAL);
			AppDelegate::getInstance()->onXoneKeyExit();
		}

	}
}

AppDelegate::AppDelegate() {
	_instance = this;
}

AppDelegate * AppDelegate::getInstance() {
	return _instance;
}

int AppDelegate::run(HINSTANCE hInstance) {
	this->hInstance = hInstance;
	g_mainThreadId = GetCurrentThreadId();

	//check app has already run or not
	HWND previousInstance = FindWindow(APP_CLASS, NULL);
	if (previousInstance) {
		MessageBeep(MB_ICONEXCLAMATION);
		SendMessage(previousInstance, WM_USER + 2019, 0, 0);
		PostQuitMessage(0);
		return 0;
	}

	// Initialize performance logger
	PerformanceLogger::LogInfo("XoneKey application starting");

	//init XoneKey Engine
	XoneKeyHelper::initialize();
	XoneKeyManager::initEngine();

	// Start application health monitoring
	ApplicationHealthMonitor::GetInstance()->StartMonitoring();

	//create system tray
	SystemTrayHelper::createSystemTrayIcon(hInstance);
	SystemTrayHelper::updateData();

	//create main control
	if (vShowOnStartUp)
		createMainDialog();

	//check update - moved to background to prevent blocking
	if (vCheckNewVersion)
	{
		// Create a thread for update check to avoid blocking the main message loop
		HANDLE hThread = CreateThread(NULL, 0, SimpleCheckUpdateThreadProc, NULL, 0, NULL);
		if (hThread != NULL)
		{
			CloseHandle(hThread); // We don't need to wait for it
		}
	}

#define APP_TIMER_HEARTBEAT 1001

	MSG msg;
	// Create a periodic timer to wake up the message loop every 30 seconds
	// This prevents false positive heartbeat timeouts when there's no user activity
	SetTimer(NULL, APP_TIMER_HEARTBEAT, 30000, NULL);

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))	{
		// Signal heartbeat for health monitoring
		ApplicationHealthMonitor::GetInstance()->SignalHeartbeat();
		
		if (msg.message == WM_TIMER && msg.wParam == APP_TIMER_HEARTBEAT) {
			// Timer message just to wake up GetMessage, nothing to do
			continue;
		}

		if (msg.message == WM_APP_RECOVER_HOOKS) {
			PerformanceLogger::LogWarning("Executing hook recovery on UI thread...");
			XoneKeyManager::freeEngine();
			XoneKeyManager::initEngine();
			
			// Reset heartbeat timer in the monitor to avoid immediate re-trigger
			ApplicationHealthMonitor::GetInstance()->SignalHeartbeat();

			// Show toast to acknowledge recovery to user
			ToastNotification::Show(mainDialog ? mainDialog->getHwnd() : NULL, 
							   _T("Hệ thống vừa tự động khôi phục bộ gõ để đảm bảo ổn định."), TOAST_INFO);
			continue;
		}

		if (msg.message == WM_APP_SHOW_ERROR) {
			std::wstring* pMsg = (std::wstring*)msg.lParam;
			if (pMsg) {
				ToastNotification::Show(mainDialog ? mainDialog->getHwnd() : NULL, 
								   pMsg->c_str(), TOAST_ERROR, TOAST_DURATION_LONG);
				delete pMsg;
			}
			continue;
		}

		if (msg.message == WM_KEYDOWN) {
			XoneKeyManager::_lastKeyCode = (UINT16)msg.wParam;
		}
		if (!isDialogMsg(msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	// Cleanup
	KillTimer(NULL, APP_TIMER_HEARTBEAT);
	ApplicationHealthMonitor::DestroyInstance();
	PerformanceLogger::LogInfo("XoneKey application shutting down");
	
	return 0;
}

HWND AppDelegate::getMainDialogHwnd() const {
	return mainDialog ? mainDialog->getHwnd() : NULL;
}

void AppDelegate::createMainDialog() {
	if (mainDialog == NULL) {
		mainDialog = new MainControlDialog(hInstance, IDD_DIALOG_MAIN);
		mainDialog->show();
	} else {
		mainDialog->bringOnTop();
	}
}

void AppDelegate::closeDialog(BaseDialog * dialog) {
	dialog->closeDialog();
	if (mainDialog == dialog) {
		delete mainDialog;
		mainDialog = NULL;
	} else if (aboutDialog == dialog) {
		delete aboutDialog;
		aboutDialog = NULL;
	} else if (macroDialog == dialog) {
		delete macroDialog;
		macroDialog = NULL;
	} else if (convertDialog == dialog) {
		delete convertDialog;
		convertDialog = NULL;
	}
}

void AppDelegate::onInputMethodChangedFromHotKey(bool showToast) {
	APP_SET_DATA(vLanguage, vLanguage);
	if (mainDialog) {
		mainDialog->fillData();
	}
	
	// Only show toast notification for user-initiated changes (hotkey or taskbar click)
	// Don't show toast for automatic Smart Switch Key changes to avoid annoying users
	if (showToast) {
		if (vLanguage) {
			ToastNotification::Show(mainDialog ? mainDialog->getHwnd() : NULL, 
							   _T("Đã bật chế độ gõ tiếng Việt"), TOAST_SUCCESS);
		} else {
			ToastNotification::Show(mainDialog ? mainDialog->getHwnd() : NULL, 
							   _T("Đã tắt chế độ gõ tiếng Việt"), TOAST_INFO);
		}
	}
	
	SystemTrayHelper::updateData();
}

void AppDelegate::onDefaultConfig() {
    APP_SET_DATA(vLanguage, 1);
    APP_SET_DATA(vInputType, 0);
    vFreeMark = 0;
    APP_SET_DATA(vCodeTable, 0);
    APP_SET_DATA(vCheckSpelling, 1);
    APP_SET_DATA(vUseModernOrthography, 0);
    APP_SET_DATA(vQuickTelex, 0);
    APP_SET_DATA(vSwitchKeyStatus, DEFAULT_SWITCH_STATUS);
    APP_SET_DATA(vRestoreIfWrongSpelling, 1);
    APP_SET_DATA(vFixRecommendBrowser, 1);
    APP_SET_DATA(vUseMacro, 0);
    APP_SET_DATA(vUseMacroInEnglishMode, 0);
    APP_SET_DATA(vSendKeyStepByStep, 1);
    APP_SET_DATA(vUseSmartSwitchKey, 1);
    APP_SET_DATA(vUpperCaseFirstChar, 0);
    APP_SET_DATA(vAllowConsonantZFWJ, 0);
    APP_SET_DATA(vTempOffSpelling, 0);

    APP_SET_DATA(vUseGrayIcon, 0);
    APP_SET_DATA(vShowOnStartUp, 1);
    APP_SET_DATA(vRunWithWindows, 1);

    APP_SET_DATA(vSupportMetroApp, 1);
    APP_SET_DATA(vRememberCode, 1);
    APP_SET_DATA(vOtherLanguage, 1);
    APP_SET_DATA(vTempOffXoneKey, 0);
    APP_SET_DATA(vFixChromiumBrowser, 0);

    if (mainDialog) {
        mainDialog->fillData();
    }
    
    // Show balloon notification for reset
    SystemTrayHelper::ShowBalloon(_T("XoneKey"), _T("Đã khôi phục cài đặt gốc"), NIIF_INFO);
    
    SystemTrayHelper::updateData();
}


void AppDelegate::onToggleVietnamese() {
    APP_SET_DATA(vLanguage, vLanguage ? 0 : 1);
    if (mainDialog) {
        mainDialog->fillData();
    }
    
    // Show balloon notification
    if (vLanguage) {
        SystemTrayHelper::ShowBalloon(_T("XoneKey"), _T("Đã bật chế độ gõ tiếng Việt"), NIIF_INFO);
    } else {
        SystemTrayHelper::ShowBalloon(_T("XoneKey"), _T("Đã tắt chế độ gõ tiếng Việt"), NIIF_INFO);
    }
    
    if (vUseSmartSwitchKey) {
        string exe = XoneKeyHelper::getLastAppExecuteName();
        setAppInputMethodStatus(exe, vLanguage | (vCodeTable << 1));
        saveSmartSwitchKeyData();
    }
}

void AppDelegate::onToggleCheckSpelling() {
    APP_SET_DATA(vCheckSpelling, vCheckSpelling ? 0 : 1);
    if (mainDialog) {
        mainDialog->fillData();
    }
    
    // Show toast with status
    if (vCheckSpelling) {
        ToastNotification::Show(mainDialog ? mainDialog->getHwnd() : NULL, 
                           _T("Đã bật kiểm tra chính tả"), TOAST_SUCCESS);
    } else {
        ToastNotification::Show(mainDialog ? mainDialog->getHwnd() : NULL, 
                           _T("Đã tắt kiểm tra chính tả"), TOAST_INFO);
    }
    
    vSetCheckSpelling();
}

void AppDelegate::onToggleUseSmartSwitchKey() {
    APP_SET_DATA(vUseSmartSwitchKey, vUseSmartSwitchKey ? 0 : 1);
    if (mainDialog) {
        mainDialog->fillData();
    }
    
    // Show toast with status
    if (vUseSmartSwitchKey) {
        ToastNotification::Show(mainDialog ? mainDialog->getHwnd() : NULL, 
                           _T("Đã bật loại trừ ứng dụng thông minh"), TOAST_SUCCESS);
    } else {
        ToastNotification::Show(mainDialog ? mainDialog->getHwnd() : NULL, 
                           _T("Đã tắt loại trừ ứng dụng thông minh"), TOAST_INFO);
    }
}

void AppDelegate::onToggleUseMacro() {
    APP_SET_DATA(vUseMacro, vUseMacro ? 0 : 1);
    if (mainDialog) {
        mainDialog->fillData();
    }
    
    // Show toast with status
    if (vUseMacro) {
        ToastNotification::Show(mainDialog ? mainDialog->getHwnd() : NULL, 
                           _T("Đã bật gõ tắt"), TOAST_SUCCESS);
    } else {
        ToastNotification::Show(mainDialog ? mainDialog->getHwnd() : NULL, 
                           _T("Đã tắt gõ tắt"), TOAST_INFO);
    }
}

void AppDelegate::onMacroTable() {
	if (macroDialog == NULL) {
		macroDialog = new MacroDialog(hInstance, IDD_DIALOG_MACRO);
		macroDialog->show();
	} else {
		macroDialog->bringOnTop();
	}
}

void AppDelegate::onConvertTool() {
	if (convertDialog == NULL) {
		convertDialog = new ConvertToolDialog(hInstance, IDD_DIALOG_CONVERT_TOOL);
		convertDialog->show();
	} else {
		convertDialog->bringOnTop();
	}
}

void AppDelegate::onQuickConvert() {
    if (XoneKeyHelper::quickConvert()) {
        //alert when complete
        if (!convertToolDontAlertWhenCompleted) {
            // Use toast notification instead of message box
            ToastNotification::Show(mainDialog ? mainDialog->getHwnd() : NULL, 
                               _T("Chuyển mã thành công!\nKết quả đã được lưu trong Clipboard."), TOAST_SUCCESS);
        }
    } else {
        // Show error toast
        ToastNotification::Show(mainDialog ? mainDialog->getHwnd() : NULL, 
                           _T("Không thể chuyển mã. Vui lòng kiểm tra clipboard."), TOAST_ERROR);
    }
}

void AppDelegate::onInputType(const int & type) {
    APP_SET_DATA(vInputType, type);
    if (mainDialog) {
        mainDialog->fillData();
    }
    
    // Show toast notification about input type
    LPCTSTR inputTypeName = XoneKeyManager::getInputType()[type];
    TCHAR message[128];
    wsprintf(message, _T("Đã đổi sang kiểu gõ %s"), inputTypeName);
    ToastNotification::Show(mainDialog ? mainDialog->getHwnd() : NULL, message, TOAST_INFO);
}

void AppDelegate::onTableCode(const int & code, bool showToast) {
    APP_SET_DATA(vCodeTable, code);
    if (mainDialog) {
        mainDialog->fillData();
    }
    
    // Only show toast for user-initiated changes, not from hook thread
    if (showToast) {
        LPCTSTR tableCodeName = XoneKeyManager::getTableCode()[code];
        TCHAR message[128];
        wsprintf(message, _T("Đã đổi sang bảng mã %s"), tableCodeName);
        ToastNotification::Show(mainDialog ? mainDialog->getHwnd() : NULL, message, TOAST_INFO);
    }
    
    if (vRememberCode) {
        setAppInputMethodStatus(XoneKeyHelper::getFrontMostAppExecuteName(), vLanguage | (vCodeTable << 1));
        saveSmartSwitchKeyData();
    }
}

void AppDelegate::onControlPanel() {
	createMainDialog();
}

void AppDelegate::onXoneKeyAbout() {
	if (aboutDialog == NULL) {
		aboutDialog = new AboutDialog(hInstance, IDD_ABOUTBOX);
		aboutDialog->show();
	} else {
		aboutDialog->bringOnTop();
	}
}

void AppDelegate::onXoneKeyRestart() {
	PerformanceLogger::LogInfo("XoneKey restart requested by user");
	wstring path = XoneKeyHelper::getFullPath();
	ShellExecute(NULL, L"open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
	onXoneKeyExit();
}

// Clean up toast resources on exit
void AppDelegate::onXoneKeyExit() {
    PerformanceLogger::LogInfo("XoneKey exit requested by user");
    ToastNotification::Cleanup();
    ApplicationHealthMonitor::GetInstance()->StopMonitoring();
    XoneKeyManager::freeEngine();
    XoneKeyHelper::cleanup();
    SystemTrayHelper::removeSystemTray();
    PostQuitMessage(0);
}
