#include <iostream>

// Функция для ввода массива
int* InputArray(int& size) {
    std::cout << "Введите количество элементов: ";
    std::cin >> size;

    int* arr = new int[size];
    std::cout << "Введите элементы массива:\n";
    for (int i = 0; i < size; ++i) {
        std::cin >> arr[i];
    }
    return arr;
}

// Функция для вывода массива
void OutputArray(const int arr[], int size) {
    std::cout << "Массив: ";
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

// Функция для обновления массива (удаление отрицательных элементов)
int* UpdateArray(int* oldArr, int& oldSize, int& newSize) {
    // Считаем количество элементов для нового массива
    newSize = 0;
    for (int i = 0; i < oldSize; ++i) {
        if (oldArr[i] >= 0) {
            newSize++;
        }
    }

    // Создаем новый массив
    int* newArr = new int[newSize];
    for (int i = 0, j = 0; i < oldSize; ++i) {
        if (oldArr[i] >= 0) {
            newArr[j++] = oldArr[i];
        }
    }

    // Освобождаем старый массив
    delete[] oldArr;
    return newArr;
}

int main() {
    int size;
    int* array = InputArray(size);

    std::cout << "Исходный ";
    OutputArray(array, size);

    int newSize;
    array = UpdateArray(array, size, newSize); // Перезаписываем указатель
    size = newSize; // Обновляем размер

    std::cout << "Обновленный ";
    OutputArray(array, size);
    std::cout << size;
    delete[] array; // Освобождаем память
    return 0;
}