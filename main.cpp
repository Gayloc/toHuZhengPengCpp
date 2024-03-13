#include <Windows.h>
#include <gdiplus.h>
#include "resource.h"
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HBITMAP hBitmap;
HWND button;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // ��ʼ�� GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // ��������
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
    hwnd = CreateWindowEx(0, L"WindowClass", L"������С����", WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX), 100, 100, 800, 600, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);

    button = CreateWindowEx(0, L"BUTTON", L"�����ť��ʾ��������ϲ����ͼƬ", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 250, 285, 300, 30, hwnd, NULL, hInstance, NULL);

    // ����Ϣѭ��
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // �ر� GDI+
    GdiplusShutdown(gdiplusToken);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND: {
        if (LOWORD(wParam) == BN_CLICKED) {
            // ��ť����������ز���ʾͼƬ
            ShowWindow(button, SW_HIDE); // ���ذ�ť
            HINSTANCE hInstance = GetModuleHandle(NULL);
            hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1)); // ����� IDB_BITMAP1 ����Դ�ļ���ͼ��� ID
            if (hBitmap == NULL) {
                MessageBox(NULL, L"Failed to load image from resource!", L"Error", MB_ICONERROR);
                break;
            }
            InvalidateRect(hwnd, NULL, TRUE); // �����ػ洰��
        }
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // ����ͼ��
        if (hBitmap != NULL) {
            // ʹ�� GDI+ ����ԭʼͼ��
            Graphics graphics(hwnd);
            HINSTANCE hInstance = GetModuleHandle(NULL);
            HBITMAP hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
            if (hBitmap == NULL) {
                MessageBox(NULL, L"Failed to load image from resource!", L"Error", MB_ICONERROR);
                break;
            }
            Bitmap image(hBitmap, NULL);

            // ��ȡͼ��ԭʼ��С
            UINT originalWidth = image.GetWidth();
            UINT originalHeight = image.GetHeight();

            // �������ű���
            float scale = 0.5f; // ���ű���Ϊԭͼ��һ��

            // �������ź�Ĵ�С
            UINT scaledWidth = static_cast<UINT>(originalWidth * scale);
            UINT scaledHeight = static_cast<UINT>(originalHeight * scale);

            // �������ź��ͼ��
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
