#include "stdafx.h"
#include "ToastNotification.h"

HWND ToastNotification::hToastWnd = NULL;
UINT_PTR ToastNotification::timerID = 0;
WCHAR ToastNotification::message[256] = { 0 };
int ToastNotification::toastType = TOAST_INFO;

void ToastNotification::RegisterToastClass() {
    static bool registered = false;
    if (registered) return;

    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DROPSHADOW;
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

        RECT rc;
        GetClientRect(hwnd, &rc);
        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;

        // Oversampled rendering at 2x for smooth anti-aliased edges
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width * 2, height * 2);
        HGDIOBJ hOldBitmap = SelectObject(memDC, memBitmap);

        RECT rcFull = { 0, 0, width * 2, height * 2 };
        HBRUSH hClearBrush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(memDC, &rcFull, hClearBrush);
        DeleteObject(hClearBrush);

        // Accent color per toast type (Material Design)
        COLORREF accentColor;
        switch (toastType) {
        case TOAST_SUCCESS: accentColor = RGB(76, 175, 80);  break;
        case TOAST_WARNING: accentColor = RGB(255, 152, 0);  break;
        case TOAST_ERROR:   accentColor = RGB(244, 67, 54);  break;
        case TOAST_INFO:
        default:            accentColor = RGB(33, 150, 243); break;
        }

        int W = width * 2;
        int H = height * 2;
        const int cornerR = 24; // 12px at 1x → 24px at 2x
        const int accentW = 20; // 10px accent strip at 1x → 20px at 2x

        // 1. Dark card background (rounded rectangle)
        HPEN hNoPen = (HPEN)GetStockObject(NULL_PEN);
        HBRUSH hBgBrush = CreateSolidBrush(RGB(30, 30, 30));
        HGDIOBJ hOldPen = SelectObject(memDC, hNoPen);
        HGDIOBJ hOldBrush = SelectObject(memDC, hBgBrush);
        RoundRect(memDC, 0, 0, W, H, cornerR, cornerR);
        SelectObject(memDC, hOldBrush);
        SelectObject(memDC, hOldPen);
        DeleteObject(hBgBrush);

        // 2. Left accent strip clipped to the rounded card boundary
        HRGN hRoundRgn = CreateRoundRectRgn(0, 0, W + 1, H + 1, cornerR, cornerR);
        SelectClipRgn(memDC, hRoundRgn);
        HBRUSH hAccentBrush = CreateSolidBrush(accentColor);
        RECT rcAccent = { 0, 0, accentW, H };
        FillRect(memDC, &rcAccent, hAccentBrush);
        SelectClipRgn(memDC, NULL);
        DeleteObject(hAccentBrush);
        DeleteObject(hRoundRgn);

        // 3. White text, left-aligned with padding after the accent strip
        SetBkMode(memDC, TRANSPARENT);
        SetTextColor(memDC, RGB(255, 255, 255));
        HFONT hFont = CreateFont(38, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        HGDIOBJ hOldFont = SelectObject(memDC, hFont);
        RECT rcText = { accentW + 48, 0, W - 24, H };
        DrawText(memDC, message, -1, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

        // Downscale with HALFTONE for smooth sub-pixel rendering
        SetStretchBltMode(hdc, HALFTONE);
        SetBrushOrgEx(hdc, 0, 0, NULL);
        StretchBlt(hdc, 0, 0, width, height, memDC, 0, 0, W, H, SRCCOPY);

        SelectObject(memDC, hOldFont);
        DeleteObject(hFont);
        SelectObject(memDC, hOldBitmap);
        DeleteObject(memBitmap);
        DeleteDC(memDC);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        KillTimer(hwnd, timerID);
        timerID = 0;
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
        SetTimer(duration);
        InvalidateRect(hToastWnd, NULL, TRUE);
        return;
    }
    
    // Calculate size
    int width = 320;
    int height = 48;
    
    // Screen positioning (Bottom-Center, 80px from bottom)
    int x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
    int y = GetSystemMetrics(SM_CYSCREEN) - height - 80;
    
    // Create window
    hToastWnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
        L"XoneKeyToastClass",
        NULL,
        WS_POPUP | WS_VISIBLE,
        x, y, width, height,
        NULL, NULL, GetModuleHandle(NULL), NULL
    );
    
    // Fix: CreateWindowEx with WS_POPUP needs WS_VISIBLE or ShowWindow
    // But we use AnimateWindow, so let's keep it clean.
    
    // Set rounded rectangle region (12px corner radius)
    HRGN hRgn = CreateRoundRectRgn(0, 0, width + 1, height + 1, 12, 12);
    SetWindowRgn(hToastWnd, hRgn, TRUE);

    // Near-opaque for readability; CS_DROPSHADOW provides the shadow
    SetLayeredWindowAttributes(hToastWnd, 0, 248, LWA_ALPHA);
    
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
}