#include <windows.h>
#include <cmath>
#include <ctime>

// Константы окна
const int WIDTH = 600;
const int HEIGHT = 600;

// Цветовые константы
const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF BLUE = RGB(0, 0, 255);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF GRAY = RGB(128, 128, 128);

// Основная процедура окна
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Фон
            HBRUSH backbrush = CreateSolidBrush(BLACK);
            SelectObject(hdc, backbrush);
            Rectangle(hdc, 0, 0, WIDTH, HEIGHT);
            DeleteObject(backbrush);

            // Центральный круг с градиентом
            HPEN pen = CreatePen(PS_SOLID, 2, WHITE);
            SelectObject(hdc, pen);
            for (int i = 0; i < 50; ++i) {
                int r = 100 + i * 2;
                HBRUSH brush = CreateSolidBrush(RGB(i * 2, i * 2, 255 - i * 2));
                SelectObject(hdc, brush);
                Ellipse(hdc, 300 - r, 300 - r, 300 + r, 300 + r);
                DeleteObject(brush);
            }

            // Концентрические круги
            for (int i = 0; i < 10; ++i) {
                int r = 50 + i * 10;
                HPEN circlepen = CreatePen(PS_SOLID, 1, WHITE);
                SelectObject(hdc, circlepen);
                Ellipse(hdc, 300 - r, 300 - r, 300 + r, 300 + r);
                DeleteObject(circlepen);
            }

            // Геометрические фигуры
            POINT triangle[] = { {200, 100}, {400, 100}, {300, 300} };
            HBRUSH tribrush = CreateSolidBrush(YELLOW);
            SelectObject(hdc, tribrush);
            Polygon(hdc, triangle, 3);
            DeleteObject(tribrush);

            POINT rhombus[] = { {250, 200}, {300, 150}, {350, 200}, {300, 250} };
            HBRUSH rhombrush = CreateSolidBrush(GREEN);
            SelectObject(hdc, rhombrush);
            Polygon(hdc, rhombus, 4);
            DeleteObject(rhombrush);

            // Прямоугольники
            HBRUSH rectbrush = CreateSolidBrush(RED);
            SelectObject(hdc, rectbrush);
            Rectangle(hdc, 100, 100, 200, 200);
            Rectangle(hdc, 400, 100, 500, 200);
            Rectangle(hdc, 100, 400, 200, 500);
            Rectangle(hdc, 400, 400, 500, 500);
            DeleteObject(rectbrush);

            // Линии
            MoveToEx(hdc, 0, 0, NULL);
            LineTo(hdc, 600, 600);
            MoveToEx(hdc, 0, 600, NULL);
            LineTo(hdc, 600, 0);

            // Текстуры
            for (int x = 0; x < 600; x += 10) {
                MoveToEx(hdc, x, 0, NULL);
                LineTo(hdc, x + 5, 30);
            }

            // Случайные элементы
            srand(GetTickCount());
            for (int i = 0; i < 10; ++i) {
                int x = rand() % 500;
                int y = rand() % 500;
                int size = rand() % 50 + 10;
                HBRUSH randbrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
                SelectObject(hdc, randbrush);
                RoundRect(hdc, x, y, x + size, y + size, 10, 10);
                DeleteObject(randbrush);
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
    wc.lpszClassName = "GraphicsDemo";

    RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(
            wc.lpszClassName,
            "Графический образ",
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