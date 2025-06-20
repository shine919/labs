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
    int counter = 0;
    for (int i = 0;i<n;++i){
        if (arr[i] % 2 != 0){
            std::cout << " элемент массива нечетный" << arr[i] << std::endl;
            counter+=1;
//            refactor(arr, i, n);
        }
        if (counter==3){
            refactor(arr, i-2, n);
            refactor(arr, i-2, n);
            refactor(arr, i-2, n);
            counter=0;
        }
    }
    std::cout << "Массив после удаления нечётых элементов: ";
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;


    // Освобождение памяти
    delete[] arr;

    return 0;
}