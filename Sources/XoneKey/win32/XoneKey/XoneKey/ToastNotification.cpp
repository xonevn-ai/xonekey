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
        
        RECT rc;
        GetClientRect(hwnd, &rc);
        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;

        // --- Oversampled Rendering (2x) ---
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width * 2, height * 2);
        HGDIOBJ hOldBitmap = SelectObject(memDC, memBitmap);

        // Fill transparent-ish base (though we have SetWindowRgn)
        HBRUSH hClearBrush = CreateSolidBrush(RGB(0, 0, 0));
        RECT rcFull = { 0, 0, width * 2, height * 2 };
        FillRect(memDC, &rcFull, hClearBrush);
        DeleteObject(hClearBrush);

        // Set colors based on type
        COLORREF bgColor, borderColor;
        switch (toastType) {
        case TOAST_SUCCESS: bgColor = RGB(39, 174, 96); borderColor = RGB(30, 132, 73); break;
        case TOAST_WARNING: bgColor = RGB(243, 156, 18); borderColor = RGB(175, 110, 11); break;
        case TOAST_ERROR:   bgColor = RGB(192, 57, 43); borderColor = RGB(146, 43, 33); break;
        case TOAST_INFO:
        default:            bgColor = RGB(41, 128, 185); borderColor = RGB(21, 67, 96); break;
        }

        HBRUSH hBrush = CreateSolidBrush(bgColor);
        HPEN hPen = CreatePen(PS_SOLID, 2, borderColor); // Thicker pen for 2x
        HGDIOBJ hOldMemBrush = SelectObject(memDC, hBrush);
        HGDIOBJ hOldMemPen = SelectObject(memDC, hPen);

        // Draw pill at 2x scale
        int radius = height * 2;
        RoundRect(memDC, 0, 0, width * 2, height * 2, radius, radius);

        // Draw text at 2x scale
        SetBkMode(memDC, TRANSPARENT);
        SetTextColor(memDC, RGB(255, 255, 255));
        HFONT hFont = CreateFont(34, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        HGDIOBJ hOldMemFont = SelectObject(memDC, hFont);

        RECT rcText = { 0, 0, width * 2, height * 2 };
        DrawText(memDC, message, -1, &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // --- Downscale to Window DC (HALFTONE for smooth edges) ---
        SetStretchBltMode(hdc, HALFTONE);
        SetBrushOrgEx(hdc, 0, 0, NULL);
        StretchBlt(hdc, 0, 0, width, height, memDC, 0, 0, width * 2, height * 2, SRCCOPY);

        // Cleanup
        SelectObject(memDC, hOldMemFont);
        DeleteObject(hFont);
        SelectObject(memDC, hOldMemBrush);
        SelectObject(memDC, hOldMemPen);
        DeleteObject(hBrush);
        DeleteObject(hPen);
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
    int width = 260;
    int height = 40;
    
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
    
    // Set pill region
    HRGN hRgn = CreateRoundRectRgn(0, 0, width, height, height, height);
    SetWindowRgn(hToastWnd, hRgn, TRUE);
    
    // Set transparency
    SetLayeredWindowAttributes(hToastWnd, 0, 235, LWA_ALPHA);
    
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