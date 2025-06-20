#include <iostream>

int main() {
    int n;
    std::cout << "Введите количество элементов массива: " << std::endl;
    std::cin >> n;

    // Выделение памяти под массив
    int* arr = new int[n];

    std::cout << "Введите элементы массива:" << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    // Инвертирование четных элементов
    for (int i = 0; i < n; ++i) {
        if (arr[i] % 2 == 0) {
            arr[i] *= -1;
        }
    }

    // Вывод результата
    std::cout << "Измененный массив:" << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << " ";
    }

    // Освобождение памяти
    delete[] arr;

    return 0;
}