#include <windows.h>

const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Настройка пера для рисования
            HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
            SelectObject(hdc, pen);

            // Крыша (треугольник)
            POINT roofPoints[] = {
                    {150, 100},
                    {300, 100},
                    {225, 50}
            };
            Polygon(hdc, roofPoints, 3);

            // Стены дома
            Rectangle(hdc, 150, 100, 300, 300);

            // Дверь
            Rectangle(hdc, 250, 250, 275, 300);

            // Окно
            Rectangle(hdc, 175, 150, 225, 200);
            // Перекрестие окна
            MoveToEx(hdc, 200, 150, NULL);
            LineTo(hdc, 200, 200);
            MoveToEx(hdc, 175, 175, NULL);
            LineTo(hdc, 225, 175);

            DeleteObject(pen);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "SimpleHouse";

    RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(
            wc.lpszClassName,
            "Простой домик",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}