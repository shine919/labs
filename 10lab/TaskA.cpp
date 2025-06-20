#include <windows.h>
#include <vector>
#include <fstream>

using namespace std;

const int CELL_SIZE = 40; // Размер клетки в пикселях
const COLORREF WALL_COLOR = RGB(0, 0, 0);    // Черный
const COLORREF PATH_COLOR = RGB(255, 255, 255); // Белый
const COLORREF PLAYER_COLOR = RGB(255, 0, 0);  // Красный
const COLORREF FINISH_COLOR = RGB(255, 20, 147); // Фиол

struct Position {
    int x, y;
};

vector<vector<char>> maze;
Position player;
int mazeWidth, mazeHeight;

// Загрузка лабиринта из файла
void LoadMaze(const string& filename) {
    ifstream file(filename);
    if (!file) {
        MessageBoxA(NULL, "Ошибка загрузки файла", "Ошибка", MB_ICONERROR);
        exit(1);
    }

    file >> mazeHeight >> mazeWidth;
    maze.resize(mazeHeight, vector<char>(mazeWidth));

    for (int i = 0; i < mazeHeight; ++i) {
        for (int j = 0; j < mazeWidth; ++j) {
            file >> maze[i][j];
        }
    }

    file >> player.x >> player.y;
}

// Проверка допустимости хода
bool IsValidMove(int newX, int newY) {
    return newX >= 0 && newX < mazeHeight &&
           newY >= 0 && newY < mazeWidth &&
           maze[newX][newY] != '#';
}

// Обработчик окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hwnd;
    PAINTSTRUCT ps;
    HDC hdc;

    switch (msg) {
        case WM_CREATE:
            hwnd = hWnd;
            break;

        case WM_PAINT: {
            hdc = BeginPaint(hWnd, &ps);

            // Рисуем лабиринт
            for (int i = 0; i < mazeHeight; ++i) {
                for (int j = 0; j < mazeWidth; ++j) {
                    RECT rect = {
                            j * CELL_SIZE,
                            i * CELL_SIZE,
                            (j +1) * CELL_SIZE,
                            (i +1) * CELL_SIZE
                    };
                    COLORREF color;
                    if (maze[i][j] == '#'){
                         color =  WALL_COLOR;
                    }
                    else if(maze[i][j] == '$'){
                         color = FINISH_COLOR;
                    }
                    else{
                         color = PATH_COLOR;
                    }

                    HBRUSH hBrush = CreateSolidBrush(color);
                    FillRect(hdc, &rect, hBrush);
                    DeleteObject(hBrush);
                }
            }

            // Рисуем игрока
            RECT playerRect = {
                    player.y * CELL_SIZE,
                    player.x * CELL_SIZE,
                    (player.y + 1) * CELL_SIZE,
                    (player.x + 1) * CELL_SIZE
            };
            HBRUSH hPlayerBrush = CreateSolidBrush(PLAYER_COLOR);
            FillRect(hdc, &playerRect, hPlayerBrush);
            DeleteObject(hPlayerBrush);

            EndPaint(hWnd, &ps);
            break;
        }

        case WM_KEYDOWN: {
            int newX = player.x;
            int newY = player.y;

            switch (wParam) {
                case VK_UP:    newX--; break;
                case VK_DOWN:  newX++; break;
                case VK_LEFT:  newY--; break;
                case VK_RIGHT: newY++; break;
                case VK_ESCAPE: PostQuitMessage(0); return 0;
            }

            if (IsValidMove(newX, newY)) {
                player.x = newX;
                player.y = newY;
                InvalidateRect(hWnd, NULL, TRUE); // Перерисовать окно
                if (maze[player.x][player.y] == '$') {
                    MessageBoxA(hwnd, "ПОБЕДА", "WIN",  MB_OK | MB_ICONINFORMATION);
                    PostQuitMessage(0); // Закрыть приложение
                }
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Загрузка лабиринта
    LoadMaze("../10lab/maze.txt");

    // Регистрация класса окна
    const char CLASS_NAME[] = "MazeWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance    = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor      = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    // Создание окна
    HWND hwnd = CreateWindowEx(
            0,
            CLASS_NAME,
            "Лабиринт",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            mazeWidth * CELL_SIZE + 16, // Ширина с учетом рамки
            mazeHeight * CELL_SIZE + 39, // Высота с учетом заголовка
            NULL,
            NULL,
            hInstance,
            NULL
    );

    ShowWindow(hwnd, nCmdShow);

    // Цикл обработки сообщений
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}