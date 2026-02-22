#include "stdafx.h"
#include "ToastNotification.h"

HWND ToastNotification::hToastWnd = NULL;
UINT_PTR ToastNotification::timerID = 0;
HBRUSH ToastNotification::hBgBrush = NULL;
WCHAR ToastNotification::message[256] = { 0 };
int ToastNotification::toastType = TOAST_INFO;

void ToastNotification::RegisterToastClass() {
    static bool registered = false;
    if (registered) return;

    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = ToastWndProc;
    wcex.hInstance = GetModuleHandle(NULL);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = NULL; // We'll paint it ourselves
    wcex.lpszClassName = L"XoneKeyToastClass";

    RegisterClassEx(&wcex);
    registered = true;
}

LRESULT CALLBACK ToastNotification::ToastWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        return 0;

    case WM_TIMER:
        if (wParam == 1) {
            Hide();
        }
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        
        // Get client rect
        RECT rc;
        GetClientRect(hwnd, &rc);
        
        // Set background color based on type
        COLORREF bgColor;
        switch (toastType) {
        case TOAST_SUCCESS:
            bgColor = RGB(76, 175, 80);
            break;
        case TOAST_WARNING:
            bgColor = RGB(255, 152, 0);
            break;
        case TOAST_ERROR:
            bgColor = RGB(244, 67, 54);
            break;
        case TOAST_INFO:
        default:
            bgColor = RGB(33, 150, 243);
            break;
        }
        
        if (hBgBrush) DeleteObject(hBgBrush);
        hBgBrush = CreateSolidBrush(bgColor);
        
        // Fill background
        FillRect(hdc, &rc, hBgBrush);
        
        // Draw text
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));
        
        // Use a nicer font
        HFONT hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
        
        // Draw text centered
        DrawText(hdc, message, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        
        // Clean up
        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
        
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        KillTimer(hwnd, timerID);
        timerID = 0;
        if (hBgBrush) {
            DeleteObject(hBgBrush);
            hBgBrush = NULL;
        }
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void ToastNotification::SetTimer(DWORD duration) {
    if (timerID != 0) {
        ::KillTimer(hToastWnd, timerID);
    }
    timerID = ::SetTimer(hToastWnd, 1, duration, NULL);
}

void ToastNotification::Show(HWND hParent, LPCTSTR szMessage, int type, DWORD duration) {
    RegisterToastClass();
    
    // Save message and type
    wcscpy_s(message, szMessage);
    toastType = type;
    
    // If already showing, just update
    if (hToastWnd != NULL && IsWindow(hToastWnd)) {
        // Update timer
        SetTimer(duration);
        
        // Force repaint
        InvalidateRect(hToastWnd, NULL, TRUE);
        return;
    }
    
    // Get parent rect
    RECT rcParent;
    if (hParent) {
        GetWindowRect(hParent, &rcParent);
    }
    else {
        // Use desktop
        rcParent.left = 0;
        rcParent.top = 0;
        rcParent.right = GetSystemMetrics(SM_CXSCREEN);
        rcParent.bottom = GetSystemMetrics(SM_CYSCREEN);
    }
    
    // Calculate size
    int width = 300;
    int height = 50;
    
    // Calculate position (centered at bottom of parent)
    int x = rcParent.left + (rcParent.right - rcParent.left - width) / 2;
    int y = rcParent.bottom - height - 20; // 20px margin
    
    // Create window
    hToastWnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        L"XoneKeyToastClass",
        NULL,
        WS_POPUP,
        x, y, width, height,
        NULL, NULL, GetModuleHandle(NULL), NULL
    );
    
    // Set transparency
    SetLayeredWindowAttributes(hToastWnd, 0, 230, LWA_ALPHA); // 90% opacity
    
    // Show with animation
    AnimateWindow(hToastWnd, 200, AW_BLEND);
    
    // Set timer to hide
    SetTimer(duration);
}

void ToastNotification::Hide() {
    if (hToastWnd != NULL && IsWindow(hToastWnd)) {
        // Hide with animation
        AnimateWindow(hToastWnd, 200, AW_HIDE | AW_BLEND);
        DestroyWindow(hToastWnd);
        hToastWnd = NULL;
    }
    
    if (timerID != 0) {
        KillTimer(NULL, timerID);
        timerID = 0;
    }
}

void ToastNotification::Cleanup() {
    Hide();
    if (hBgBrush) {
        DeleteObject(hBgBrush);
        hBgBrush = NULL;
    }
}