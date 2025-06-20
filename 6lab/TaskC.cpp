#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// Глобальные переменные
std::vector<int> original_data;
std::vector<int> processed_data;
double average = 0.0;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

bool loadArrayFromFile(const std::string& filename, std::vector<int>& data) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) return false;
    int num;
    while (inFile >> num) {
        data.push_back(num);
    }
    return !data.empty();
}

void ProcessData() {
    processed_data.clear();
    average = 0.0;

    int sum = 0;
    int count = 0;
    for (int num : original_data) {
        if (num % 2 == 0) {
            sum += num;
            count++;
        }
    }

    if (count == 0) {
        processed_data = original_data;
        return;
    }

    average = static_cast<double>(sum) / count;

    for (int num : original_data) {
        if (num >= average) {
            processed_data.push_back(num);
        }
    }
}

void DrawDiagram(HDC hdc, const std::vector<int>& data, int x_offset, const char* title) {
    if (data.empty()) return;

    const int margin = 40;
    int width = 500, height = 400;
    int max_val = *std::max_element(data.begin(), data.end());
    float scale = max_val > 0 ? (height - 2 * margin) / (float)max_val : 1;

    TextOutA(hdc, x_offset + 150, margin - 30, title, strlen(title));

    // Оси координат
    MoveToEx(hdc, x_offset + margin, margin + height - 2 * margin, nullptr);
    LineTo(hdc, x_offset + margin, margin);
    LineTo(hdc, x_offset + margin + width - 2 * margin, margin);

    int bar_width = (width - 2 * margin) / data.size() - 2;
    for (size_t i = 0; i < data.size(); ++i) {
        int x = x_offset + margin + i * (bar_width + 2);
        int bar_height = (int)((data[i]) * scale);

        RECT rect = { x, margin + height - 2 * margin - bar_height,
                      x + bar_width, margin + height - 2 * margin };

        if (!processed_data.empty()) {
            FillRect(hdc, &rect, (HBRUSH)(COLOR_HIGHLIGHT + 1));
        }

        char buf[16];
        sprintf_s(buf, "%d", data[i]);
        TextOutA(hdc, x, margin + height - 2 * margin + 5, buf, strlen(buf));
    }

}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    if (!loadArrayFromFile("../6lab/input.txt", original_data)) {
        MessageBoxA(NULL, "Не удалось загрузить файл", "Ошибка", MB_ICONERROR);
        return 1;
    }

    ProcessData();

    const char CLASS_NAME[] = "DiagramWindow";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
            0, CLASS_NAME, "Удаление элементов ниже среднего",
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 600,
            NULL, NULL, hInst, NULL);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rect;
            GetClientRect(hwnd, &rect);

            DrawDiagram(hdc, original_data, 50, "Исходный массив");
            DrawDiagram(hdc, processed_data, 600, "После удаления");

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}