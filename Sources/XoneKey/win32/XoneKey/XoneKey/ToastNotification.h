#pragma once
#include "stdafx.h"

// Toast notification duration (in milliseconds)
#define TOAST_DURATION_SHORT 2000
#define TOAST_DURATION_MEDIUM 3500
#define TOAST_DURATION_LONG 5000

// Toast types
#define TOAST_INFO 0
#define TOAST_SUCCESS 1
#define TOAST_WARNING 2
#define TOAST_ERROR 3

class ToastNotification {
private:
    static HWND hToastWnd;
    static UINT_PTR timerID;
    static WCHAR message[256];
    static int toastType;

    static LRESULT CALLBACK ToastWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static void RegisterToastClass();
    static void SetTimer(DWORD duration);

public:
    static void Show(HWND hParent, LPCTSTR szMessage, int type = TOAST_INFO, DWORD duration = TOAST_DURATION_MEDIUM);
    static void Hide();
    static void Cleanup();
};