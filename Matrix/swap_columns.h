#include <iostream>
#include <chrono>

typedef struct sColumn{
    int y;  //quantity of elements in column
    int *a; //array of elements
    int key = 0; //unic code
    int max = 0; //max element in column
} Column;

template<typename T> T getValue(T &forRecord);
Column* input(int m, int n);
void output(const char* msg, Column* a, int m);
Column* erase(Column*& columns, int m);
void getMaxElement(Column *columns, int m);
Column* insertSort(Column *columns, int m);
void swap(Column &a, Column &b);
int binarySearch(Column *columns, int key, int m);
Column* insertSortBinarySearch(Column *columns, int m);

//***DOWNSIDES***///
// 1) no save input
// 2) output is shifting
// 3) simple "keys" and field "max" is surplus
