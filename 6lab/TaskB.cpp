#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// Глобальные переменные
int* original_arr = nullptr;
int* processed_arr = nullptr;
int original_size = 0;
int processed_size = 0;
bool has_odd = false;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void refactor(int arr[], int val, int& size) {
    if (val < 0 || val >= size) return;
    for (int i = val; i < size - 1; ++i) {
        arr[i] = arr[i + 1];
    }
    size--;
}

bool loadArrayFromFile(const std::string& filename, std::vector<int>& data) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) return false;

    int num;
    while (inFile >> num) {
        data.push_back(num);
    }
    return !data.empty();
}

void DrawDiagrams(HDC hdc, int width, int height) {
    // Отрисовка левой диаграммы (оригинал)
    if (original_arr && original_size > 0) {
        int x_start = 40;
        int y_start = 40;
        int diagram_width = (width - 120) / 2;
        int diagram_height = height - 80;

        TextOutA(hdc, x_start, y_start - 25, "ИСХОДНЫЕ ДАННЫЕ", 15);

        // Оси
        MoveToEx(hdc, x_start, y_start + diagram_height, nullptr);
        LineTo(hdc, x_start, y_start);
        LineTo(hdc, x_start + diagram_width, y_start);

        int max_val = *std::max_element(original_arr, original_arr + original_size);
        float scale = max_val > 0 ? (float)diagram_height / max_val : 0;
        int bar_width = diagram_width / original_size - 2;

        for (int i = 0; i < original_size; ++i) {
            int x = x_start + i * (bar_width + 2);
            int bar_height = (int)(original_arr[i] * scale);

            RECT rect = { x, y_start + diagram_height - bar_height,
                          x + bar_width, y_start + diagram_height };
            FillRect(hdc, &rect, (HBRUSH)(COLOR_HIGHLIGHT + 1));

            char buf[16];
            sprintf_s(buf, "%d", original_arr[i]);
            TextOutA(hdc, x, y_start + diagram_height + 5, buf, strlen(buf));
        }
    }

    // Отрисовка правой диаграммы (обработанные данные)
    if (processed_arr && processed_size > 0 && has_odd) {
        int x_start = width / 2 + 20;
        int y_start = 40;
        int diagram_width = (width - 120) / 2;
        int diagram_height = height - 80;

        TextOutA(hdc, x_start, y_start - 25, "ПОСЛЕ УДАЛЕНИЯ", 15);

        // Оси
        MoveToEx(hdc, x_start, y_start + diagram_height, nullptr);
        LineTo(hdc, x_start, y_start);
        LineTo(hdc, x_start + diagram_width, y_start);

        int max_val = *std::max_element(processed_arr, processed_arr + processed_size);
        float scale = max_val > 0 ? (float)diagram_height / max_val : 0;
        int bar_width = diagram_width / processed_size - 2;

        for (int i = 0; i < processed_size; ++i) {
            int x = x_start + i * (bar_width + 2);
            int bar_height = (int)(processed_arr[i] * scale);

            RECT rect = { x, y_start + diagram_height - bar_height,
                          x + bar_width, y_start + diagram_height };
            FillRect(hdc, &rect, (HBRUSH)(COLOR_HIGHLIGHT + 1));

            char buf[16];
            sprintf_s(buf, "%d", processed_arr[i]);
            TextOutA(hdc, x, y_start + diagram_height + 5, buf, strlen(buf));
        }
    } else if (!has_odd) {
        TextOutA(hdc, width/2 + 50, height/2,
                 "Нечетных элементов не найдено", 28);
    }
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) {
    // Загрузка данных из файла
    std::vector<int> temp_data;
    if (!loadArrayFromFile("../6lab/data.txt", temp_data)) {
        MessageBoxA(NULL, "Не удалось загрузить файл data.txt", "Ошибка", MB_ICONERROR);
        return 1;
    }

    original_size = temp_data.size();
    original_arr = new int[original_size];
    std::copy(temp_data.begin(), temp_data.end(), original_arr);

    // Создаем копию для обработки
    processed_size = original_size;
    processed_arr = new int[processed_size];
    std::copy(original_arr, original_arr + original_size, processed_arr);

    // Поиск последнего нечетного элемента
    int last_odd_index = -1;
    for (int i = processed_size - 1; i >= 0; --i) {
        if (processed_arr[i] % 2 != 0) {
            last_odd_index = i;
            break;
        }
    }

    if (last_odd_index != -1) {
        refactor(processed_arr, last_odd_index, processed_size);
        has_odd = true;
    }

    // Регистрация класса окна
    const char CLASS_NAME[] = "FileDiagramWindow";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
            0, CLASS_NAME, "Диаграммы из файла",
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 600,
            NULL, NULL, hInst, NULL);

    ShowWindow(hwnd, nCmdShow);

    // Запуск цикла сообщений
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    delete[] original_arr;
    delete[] processed_arr;
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rect;
            GetClientRect(hwnd, &rect);
            DrawDiagrams(hdc, rect.right, rect.bottom);

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}