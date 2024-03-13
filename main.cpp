#include <Windows.h>
#include <gdiplus.h>
#include "resource.h"
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HBITMAP hBitmap;
HWND button;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 初始化 GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 创建窗口
    HWND hwnd;
    WNDCLASSEX wc;
    HICON icon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON1));
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass";
    wc.hIcon = icon;
    RegisterClassEx(&wc);
    hwnd = CreateWindowEx(0, L"WindowClass", L"胡振鹏小程序", WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX), 100, 100, 800, 600, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);

    button = CreateWindowEx(0, L"BUTTON", L"点击按钮显示胡振鹏最喜欢的图片", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 250, 285, 300, 30, hwnd, NULL, hInstance, NULL);

    // 主消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 关闭 GDI+
    GdiplusShutdown(gdiplusToken);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND: {
        if (LOWORD(wParam) == BN_CLICKED) {
            // 按钮被点击，加载并显示图片
            ShowWindow(button, SW_HIDE); // 隐藏按钮
            HINSTANCE hInstance = GetModuleHandle(NULL);
            hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1)); // 这里的 IDB_BITMAP1 是资源文件中图像的 ID
            if (hBitmap == NULL) {
                MessageBox(NULL, L"Failed to load image from resource!", L"Error", MB_ICONERROR);
                break;
            }
            InvalidateRect(hwnd, NULL, TRUE); // 请求重绘窗口
        }
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // 绘制图像
        if (hBitmap != NULL) {
            // 使用 GDI+ 加载原始图像
            Graphics graphics(hwnd);
            HINSTANCE hInstance = GetModuleHandle(NULL);
            HBITMAP hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
            if (hBitmap == NULL) {
                MessageBox(NULL, L"Failed to load image from resource!", L"Error", MB_ICONERROR);
                break;
            }
            Bitmap image(hBitmap, NULL);

            // 获取图像原始大小
            UINT originalWidth = image.GetWidth();
            UINT originalHeight = image.GetHeight();

            // 设置缩放比例
            float scale = 0.5f; // 缩放比例为原图的一半

            // 计算缩放后的大小
            UINT scaledWidth = static_cast<UINT>(originalWidth * scale);
            UINT scaledHeight = static_cast<UINT>(originalHeight * scale);

            // 绘制缩放后的图像
            graphics.DrawImage(&image, (800 - scaledWidth) / 2, 0, scaledWidth, scaledHeight);
        }

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
