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

    // Поиск минимума и максимума
    int min_val = arr[0];
    int max_val = arr[0];
    for (int i = 1; i < n; ++i) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }

    // Вывод результата
    std::cout << "Минимальный элемент: " << min_val << std::endl;
    std::cout << "Максимальный элемент: " << max_val << std::endl;

    // Освобождение памяти
    delete[] arr;

    return 0;
}