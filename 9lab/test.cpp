#include <iostream>
#include <fstream>

using namespace std;

void get_row_and_col(const string& filename,int& ROWS, int& COLS){
    ifstream inFile(filename);
    inFile >> ROWS >> COLS;
}

int* ReadArrayFromFile(const string& filename, int* arr[],const int& ROWS,const int& COLS) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cerr << "Ошибка открытия файла: " << filename << endl;
        return nullptr;
    }
//    int num;
//    while (inFile >> num) {
//    }
    int dummy;
    inFile >> dummy >> dummy;
    for(int i = 0;i < ROWS;++i){
        for (int j =0;j<COLS;j++){
            inFile >> arr[i][j];

        }

    }
    return *arr;
}
void print_array(int* arr[],const int& ROWS,const int& COLS){
    cout << "Матрица " << ROWS << "x" << COLS << ":" << endl;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
}


int main() {
//    int originalSize = 0;
//    int* originalArray = ReadArrayFromFile("../9lab/input.txt", originalSize);
    int rows,cols;
    get_row_and_col("../9lab/input.txt",rows,cols);
//    cout << ROWS<< " Строки " << COLS << " Колонки " << endl;
    const int ROWS = rows;
    const int COLS = cols;
    int** arr = new int*[ROWS];
    for (int i = 0;i<ROWS;++i){
        arr[i] = new int[COLS];
    }
    ReadArrayFromFile("../9lab/input.txt",arr,ROWS,COLS);
    print_array(arr,ROWS,COLS);
//    if (!originalArray) {
//        return 1;
//    }

//    int newSize = 0;
//    int* processedArray = ProcessArray(originalArray, originalSize, newSize);

    // Очистка памяти
//    delete[] originalArray;
//    delete[] processedArray;

    return 0;
}