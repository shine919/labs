#include <windows.h>
#include <iostream>
#include <fstream>
#include <unordered_set>

using namespace std;

// Глобальные переменные
int** original_data = nullptr;
int** processed_data = nullptr;
int ROWS = 0;
int COLS = 0;
int PROC_ROWS = 0;
int PROC_COLS = 0;
int min_val = 0;
int max_val = 0;

// Цвета для визуализации
const COLORREF colors[] = {
        RGB(255, 0, 0),   // Красный
        RGB(0, 255, 0),   // Зеленый
        RGB(0, 0, 255),   // Синий
        RGB(255, 255, 0)  // Желтый
};

// Функция для загрузки массива из файла
bool LoadMatrixFromFile(const std::string& filename, int*** matrix, int& rows, int& cols) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) return false;

    inFile >> rows >> cols;
    *matrix = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        (*matrix)[i] = new int[cols];
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            inFile >> (*matrix)[i][j];
        }
    }

    return true;
}

// Функция для освобождения памяти
void FreeMatrix(int*** matrix, int rows) {
    if (*matrix) {
        for (int i = 0; i < rows; ++i) {
            delete[] (*matrix)[i];
        }
        delete[] *matrix;
        *matrix = nullptr;
    }
}

// Обработка массива - удаление всех строк и столбцов с минимумами
void ProcessMatrix(int** src, int** dest, int rows, int cols,
                   const unordered_set<int>& rows_to_remove,
                   const unordered_set<int>& cols_to_remove) {

    int dest_i = 0;
    for (int i = 0; i < rows; ++i) {
        if (rows_to_remove.count(i)) continue;

        int dest_j = 0;
        for (int j = 0; j < cols; ++j) {
            if (cols_to_remove.count(j)) continue;

            dest[dest_i][dest_j] = src[i][j];
            dest_j++;
        }
        dest_i++;
    }
}

// Рисование прямоугольников с цветными границами
void DrawMatrix(HDC hdc, int** matrix, int rows, int cols, int x_offset, int y_offset) {
    if (!matrix || rows <= 0 || cols <= 0) return;

    const int cell_size = 30;
    const int margin = 50;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int base_x = x_offset + margin + j * cell_size;
            int base_y = y_offset + margin + i * cell_size;

            // Определяем цвет
            int color_index = matrix[i][j] % (sizeof(colors)/sizeof(colors[0]));
            HBRUSH border_brush = CreateSolidBrush(colors[color_index]);

            // Рисуем ячейку
            RECT rect = {base_x, base_y, base_x + cell_size, base_y + cell_size};
            FrameRect(hdc, &rect, border_brush);
            DeleteObject(border_brush);

            // Выводим значение
            char buf[16];
            sprintf_s(buf, "%d", matrix[i][j]);
            SetTextColor(hdc, RGB(0, 0, 0));
            SetBkMode(hdc, TRANSPARENT);
            TextOutA(hdc, base_x + 5, base_y + 5, buf, strlen(buf));
        }
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            // Исходная матрица слева
            DrawMatrix(hdc, original_data, ROWS, COLS, 0, 0);

            // Обработанная матрица справа
            DrawMatrix(hdc, processed_data, PROC_ROWS, PROC_COLS, clientRect.right / 2, 0);

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    // Загрузка исходных данных
    if (!LoadMatrixFromFile("../9lab/input.txt", &original_data, ROWS, COLS)) {
        MessageBoxA(NULL, "Ошибка загрузки файла", "Ошибка", MB_ICONERROR);
        return 1;
    }

    // Находим минимальное значение и все его позиции
    unordered_set<int> min_rows;
    unordered_set<int> min_cols;
    min_val = original_data[0][0];
    max_val = original_data[0][0];

    // Первый проход - находим минимальное значение
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (original_data[i][j] < min_val) {
                min_val = original_data[i][j];
            }
            if (original_data[i][j] > max_val) {
                max_val = original_data[i][j];
            }
        }
    }

    // Второй проход - собираем все строки и столбцы с минимумами
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (original_data[i][j] == min_val) {
                min_rows.insert(i);
                min_cols.insert(j);
            }
        }
    }

    // Создаем обработанный массив
    PROC_ROWS = ROWS - min_rows.size();
    PROC_COLS = COLS - min_cols.size();

    if (PROC_ROWS > 0 && PROC_COLS > 0) {
        processed_data = new int*[PROC_ROWS];
        for (int i = 0; i < PROC_ROWS; ++i) {
            processed_data[i] = new int[PROC_COLS];
        }

        ProcessMatrix(original_data, processed_data, ROWS, COLS, min_rows, min_cols);
    } else {
        processed_data = nullptr;
    }

    // Создание окна
    const char CLASS_NAME[] = "MatrixVisualizer";
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
            0, CLASS_NAME, "Визуализация матрицы",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
            NULL, NULL, hInst, NULL
    );

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Очистка памяти
    FreeMatrix(&original_data, ROWS);
    FreeMatrix(&processed_data, PROC_ROWS);

    return 0;
}