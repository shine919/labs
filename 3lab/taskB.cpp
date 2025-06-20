#include <windows.h>

// Константы размера окна
const int WIDTH = 400;
const int HEIGHT = 400;
//ВАРИАНТ-3
// Основная процедура окна
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Создаем красное перо
            HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
            SelectObject(hdc, pen);

            // Рисуем узор из линий
            for (int i = 0; i < 60; ++i) {
                // Координаты начальной точки (левый нижний угол)
                int startX = 0 + i * 8;               // X всегда 0 (левый край)
                int startY = HEIGHT;  // Y уменьшается на 8 пикселей

                // Координаты конечной точки (правый верхний -> правый нижний)
                int endX = WIDTH;             // X всегда ширина окна (правый край)
                int endY = i * 24;             // Y увеличивается на 8 пикселей

                // Рисуем линию
                MoveToEx(hdc, startX, startY, NULL);
                LineTo(hdc, endX, endY);
            }
            DeleteObject(pen);
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// Точка входа
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "LinePattern";

    RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(
            wc.lpszClassName,
            "Узор из линий",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            WIDTH, HEIGHT,
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