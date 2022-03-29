#include "swap_columns.h"

int main(){
    Column* arr = nullptr;  //array of columns
    int m;
    std::cout << "Enter quantity of columns: " << std::endl;
    getValue(m);
    int n;
    std::cout << "Enter quantity of rows: " << std::endl;
    getValue(n);
    arr = input(m, n);

    output("Entered matrix", arr, m);
    getMaxElement(arr, m);
    Column* res1Arr = nullptr;
    Column* res2Arr = nullptr;
    auto startTime1 = std::chrono::high_resolution_clock::now();
    res1Arr = insertSort(arr, m); //arr
    auto endTime1 = std::chrono::high_resolution_clock::now();
    auto startTime2 = std::chrono::high_resolution_clock::now();
    res2Arr = insertSortBinarySearch(arr, m);
    auto endTime2 = std::chrono::high_resolution_clock::now();
    output("Sourced matrix - insertion sort", res1Arr, m); //arr
    std::chrono::duration<float, std::milli> duration1 = endTime1 - startTime1;
    std::cout <<"\n"<< "Time: " << duration1.count() << "ms" << std::endl << std::endl;
    output("Sourced matrix - insertion sort with binary sort", res2Arr, m);
    std::chrono::duration<float, std::milli> duration2 = endTime2 - startTime2;
    std::cout <<"\n"<< "Time: " << duration2.count() << "ms" << std::endl << std::endl;
    erase(res1Arr, m);//arr
    system("pause");
    return 0;
}