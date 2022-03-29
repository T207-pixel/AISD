//swap columns in order to the max elements (increasing sequence)
//  2  3   1     1 2  3
//  3  1   1  -> 1 3  1
//  7  6   1     1 7  6
//  2 100  1     1 2 100
#include "swap_columns.h"

template<typename T>
T getValue(T &forRecord){
    std::cin >> forRecord;
    if (!std::cin.good()) {return 1;}
    return -1;
}

Column* input(int m, int n){
    // m - quantity of columns (lebgth)
    // n - quantity of rows (column)
    Column* columns = nullptr;
    columns = new Column[m];     //allocating memmory for quantity of columns
    for (int i = 0; i < m; i++){
            columns[i].a = new int[n]; //allocating memmory for quantity of elements in columns
            columns[i].y = n;
    }        
    for (int j = 0; j < m; j++){
        columns[j].key = j;
        for (int i = 0; i < n; i++){
                std::cout<< "Enter [" << i << "][" << j << "] element: ";
                int a = 0;
                getValue(a);
                columns[j].a[i] = a;
            }
        std::cout << "Enter next column:\n";
    }
    return columns;
}

void output(const char* msg, Column* a, int m){
    int i, j;
    std::cout << msg << ":\n";
    for (i = 0; i < a[i].y; ++i) {
        for (j = 0; j < m; ++j)
        {
            std::cout << a[j].a[i] << " ";
        }
        std::cout << std::endl;
    }
}

Column* erase(Column*& columns, int m)
{
    int i;
    for (i = 0; i < m; i++)
        delete[] columns[i].a;
    delete[] columns;
    return nullptr;
}

void getMaxElement(Column *columns, int m){
    for (int i = 0; i < m; i++){
        int res = columns[i].a[0];
        for (int j = 0; j < columns[i].y; j++){
            if (res < columns[i].a[j]){
                res = columns[i].a[j];
            }
            columns[i].max = res;
        }
    }
}

void swap(Column &a, Column &b){
    Column buf = a;
    a = b;
    b = buf;
}

Column* insertSort(Column *columns, int m){ // m - length of matrix
    for (int i = 0; i < m; i++){
        int j = i -1;
        while (j >= 0 && columns[j].max > columns[j + 1].max){
            swap(columns[j], columns[j + 1]);
            j--;
        }
    }
    return columns;
}

//////***ALTERNATIVE_SEARCH***//////

int binarySearch(Column *columns, int key, int m){
    int left = 0;
    int length = m;
    int right = length;
    while (left < right - 1){
        int middle = (left + right)/2;
        if (columns[middle].max < key){
            left = middle;
        } else {
            right = middle;
        }
    }
    return right;
}

Column* insertSortBinarySearch(Column *columns, int m){
    int length = m;
    for (int i = 1; i < length; i++){
        int j = i - 1;
        int k = binarySearch(columns, columns[i].max, j);
        for (int m = j; m < k; m++)
            swap(columns[m], columns[m + 1]);
    }
    return columns;
}
