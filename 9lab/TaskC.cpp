#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;
// Глобальные переменные
int** original_data = nullptr;
int** processed_data = nullptr;
int ROWS = 0;
int COLS = 0;
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
    // Выделяем память для массива
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

void delete_row(int** dest,int rows, int cols, int row, int col){
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if(i == row){
                dest[i][j] = 0;
                continue;
            }
            if(j == col){
                dest[i][j] = 0;
                continue;
            }
        }
    }
}
void check_rows_cols(int** src, int** dest, int rows, int cols,int min){
    cout << min << "min" << endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if(src[i][j] == min){
                delete_row(dest,rows,cols,i,j);
                continue;
            }
        }
    }
}
// Пример обработки массива (можно заменить на свою логику)
void ProcessMatrix(int** src, int** dest, int rows, int cols, int min) {
    // Копируем исходные данные в обработанный массив
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            dest[i][j] = src[i][j];
        }
    }
    // Обрабатываем массив
    check_rows_cols(src, dest, rows, cols, min);
}

// Рисование прямоугольников с цветными границами
void DrawMatrix(HDC hdc, int** matrix, int rows, int cols, int x_offset, int y_offset) {
    if (!matrix || rows <= 0 || cols <= 0) return;

    const int cell_size = 30;
    const int margin = 50;
    const int border_thickness = 2;

    // Кисть для фона
    HBRUSH bg_brush = CreateSolidBrush(RGB(255, 255, 255));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int base_x = x_offset + margin + j * cell_size;
            int base_y = y_offset + margin + i * cell_size;

            // Рисуем рамку
            int color_index = matrix[i][j] % (sizeof(colors)/sizeof(colors[0]));
            HBRUSH border_brush = CreateSolidBrush(colors[color_index]);

            RECT border_rect = {
                    base_x,
                    base_y,
                    base_x + cell_size,
                    base_y + cell_size
            };
            FrameRect(hdc, &border_rect, border_brush);

            DeleteObject(border_brush);

            // Подпись значения
            SetTextColor(hdc, RGB(0, 0, 0));
            SetBkMode(hdc, TRANSPARENT);

            char buf[16];
            sprintf_s(buf, "%d", matrix[i][j]);
            TextOutA(hdc,
                     base_x + cell_size/2 - 8,
                     base_y + cell_size/2 - 8,
                     buf,
                     strlen(buf));
        }
    }

    DeleteObject(bg_brush);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            // Левая диаграмма - исходный массив
            DrawMatrix(hdc, original_data, ROWS, COLS, 0, 0);

            // Правая диаграмма - обработанный массив
            DrawMatrix(hdc, processed_data, ROWS, COLS, clientRect.right / 2, 0);

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
    // Загрузка исходного массива
    if (!LoadMatrixFromFile("../9lab/input.txt", &original_data, ROWS, COLS)) {
        MessageBoxA(NULL, "Ошибка загрузки файла", "Ошибка", MB_ICONERROR);
        return 1;
    }

    // Создаем обработанный массив
    processed_data = new int *[ROWS];
    for (int i = 0; i < ROWS; ++i) {
        processed_data[i] = new int[COLS];
    }
    min_val = original_data[0][0];
    max_val = original_data[0][0];

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (original_data[i][j] < min_val) min_val = original_data[i][j];
            if (original_data[i][j] > max_val) max_val = original_data[i][j];
        }
    }

    // Обработка массива
    ProcessMatrix(original_data, processed_data, ROWS, COLS,min_val);

    // Инициализация окна
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
            CW_USEDEFAULT, CW_USEDEFAULT,
            800, 600,
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
    FreeMatrix(&processed_data, ROWS);

    return 0;
}