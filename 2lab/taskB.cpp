#include <windows.h>

// Константы размера окна
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;

// Оконная процедура обработки сообщений
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Создаем красную кисть для заливки
            HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
            SelectObject(hdc, hBrush);

            // Верхний ромб
            POINT topRhombus[] = {
                    {180, 70},   // верх
                    {220, 130},   // правая
                    {180, 190},   // низ
                    {145, 130}    // левая
            };
            Polygon(hdc, topRhombus, 4);

            // Средний ромб
            POINT middleRhombus[] = {
                    {180, 190},   // верх
                    {140, 250},   // правая
                    {80, 250},   // низ
                    {120, 190}    // Левая
            };
            Polygon(hdc, middleRhombus, 4);

            // Нижний ромб
            POINT bottomRhombus[] = {
                    {180, 190},   // верх
                    {220, 250},   // правая
                    {280, 250},   // низ
                    {240, 190}   // Левая нижняя
            };
            Polygon(hdc, bottomRhombus, 4);

            // Очистка ресурсов
            DeleteObject(hBrush);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// Точка входа в приложение
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    // Регистрация класса окна
    WNDCLASSEX wc = {0};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance    = hInstance;
    wc.hCursor      = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "MitsubishiLogo";

    RegisterClassEx(&wc);

    // Создание окна
    HWND hwnd = CreateWindow(
            wc.lpszClassName,
            "Mitsubishi Logo",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            NULL, NULL, hInstance, NULL
    );

    // Отображение окна
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Цикл обработки сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}