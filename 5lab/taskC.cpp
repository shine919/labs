#include <iostream>


int search_number(int arr[],int number,int size){
    for (int i = 0;i<size;++i){
        if( arr[i] == number ){
            return i;
        }
    }
    return -1;
}

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
    int left = search_number(arr,min_val,n);
    int right = search_number(arr,max_val,n);
//    std::cout << "Минимальный элемент номер: " << left << std::endl;
//    std::cout << "Максимальный элемент номер: " << right << std::endl;
    std::cout << "Минимальный элемент: " << min_val << std::endl;
    std::cout << "Максимальный элемент: " << max_val << std::endl;
    // Вывод результата

    for (int i = left+1;i<right;++i){
        if (arr[i] % 2 == 0){
//            std::cout << "Чётный элемент массива " << arr[i] << std::endl;
        }
    }

    for (int i = left;i<right;){
        if (arr[i] % 2 == 0){
//            for (int i = 0; i < n; ++i) {
//                std::cout << arr[i] << " ";
//            }
            std::cout << "Чётный элемент массива " << arr[i] << std::endl;
            refactor(arr, i, n);

            right--;
        }
        else{
            i++;
        }
    }
    std::cout << "Массив после удаления четных элементов: ";
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
//    std::cout << "Минимальный элемент: " << min_val << std::endl;
//    std::cout << "Максимальный элемент: " << max_val << std::endl;

    // Освобождение памяти
    delete[] arr;

    return 0;
}