#include <iostream>
#include <fstream>

// Функция для чтения массива из файла
int* ReadArrayFromFile(const std::string& filename, int& size) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        return nullptr;
    }

    int capacity = 10;
    int* arr = new int[capacity];
    size = 0;

    int num;
    while (inFile >> num) {
        if (size >= capacity) {
            capacity *= 2;
            int* newArr = new int[capacity];
            std::copy(arr, arr + size, newArr);
            delete[] arr;
            arr = newArr;
        }
        arr[size++] = num;
    }

    inFile.close();
    return arr;
}

// Функция для записи массива в файл
void WriteArrayToFile(const std::string& filename, const int* arr, int size) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        return;
    }

    for (int i = 0; i < size; ++i) {
        outFile << arr[i] << " ";
    }

    outFile.close();
}

// Функция для обработки массива
int* ProcessArray(const int* arr, int size, int& newSize) {
    if (size == 0) {
        newSize = 0;
        return nullptr;
    }

    // Вычисляем среднее четных элементов
    int sum = 0;
    int count = 0;
    for (int i = 0; i < size; ++i) {
        if (arr[i] % 2 == 0) {
            sum += arr[i];
            count++;
        }
    }

    double average = (count != 0) ? static_cast<double>(sum) / count : 0.0;

    // Создаем новый массив
    int capacity = size;
    int* newArr = new int[capacity];
    newSize = 0;

    for (int i = 0; i < size; ++i) {
        if (arr[i] >= average) {
            newArr[newSize++] = arr[i];
        }
    }

    return newArr;
}

int main() {
    int originalSize = 0;
    int* originalArray = ReadArrayFromFile("../8lab/input.txt", originalSize);

    if (!originalArray) {
        return 1;
    }

    int newSize = 0;
    int* processedArray = ProcessArray(originalArray, originalSize, newSize);

    WriteArrayToFile("../8lab/output.txt", processedArray, newSize);

    // Очистка памяти
    delete[] originalArray;
    delete[] processedArray;

    return 0;
}