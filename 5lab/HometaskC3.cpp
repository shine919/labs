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
    int v = 0;
    int counter = 0;
    for (int i = 0;i < n;++i){
        if (arr[i] % 2 == 0){
            v += arr[i];
            counter +=1;
        }
    }
    int arith = v / counter;
    std::cout << "Среднее арифметическое " << arith << std::endl;
    for (int i = 0;i<n;){
        if (arr[i] < arith){
            std::cout << " элемент массива который меньше среднего арифметического четных элементов " << arr[i] << std::endl;
            refactor(arr, i, n);
        }
        else{
            i++;
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