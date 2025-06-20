#include <iostream>

void refactor(int arr[],int val, int& size){
    for (int i = val;i<size-1;++i){
        arr[i] = arr[i+1];

    }
    size--;
}
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

    for (int i = n;i != 0;--i){
        if (arr[i] % 2 != 0){
            std::cout << "Нечетный элемент массива " << arr[i] << std::endl;
            refactor(arr, i, n);
            break;
        }
    }
    std::cout << "Массив после удаления нечётного элемента: ";
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;


    // Освобождение памяти
    delete[] arr;

    return 0;
}