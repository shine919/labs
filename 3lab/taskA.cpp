#include <windows.h>

// Константы размера окна
const int WIDTH = 600;
const int HEIGHT = 300;

// Цвета
const COLORREF FENCE_COLOR = RGB(0, 255, 0); // Зеленый

// Функция для рисования одной планки забора
void DrawFencePost(HDC hdc, int x, int y) {
    // Определение точек для планки
    POINT points[] = {
            {x, y},             // Начало планки
            {x + 5, y - 10},    // Левый верхний зубец
            {x + 15, y - 10},   // Правый верхний зубец
            {x + 20, y},        // Конец верхней части
            {x + 20, y + 100},  // Низ планки
            {x, y + 100}        // Возврат вниз
    };

    // Рисуем замкнутую фигуру
    Polygon(hdc, points, 6);
}

// Основная процедура окна
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Создаем кисть для заливки
            HBRUSH brush = CreateSolidBrush(FENCE_COLOR);
            SelectObject(hdc, brush);

            // Рисуем забор, размещая планки через шаг 20 пикселей
            int x = 0;
            while (x < WIDTH) {
                DrawFencePost(hdc, x, 50);
                x += 20;
            }

            DeleteObject(brush);
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
    wc.lpszClassName = "Заборчик";

    RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(
            wc.lpszClassName,
            "Забор",
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