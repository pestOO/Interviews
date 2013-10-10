#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "CDArray.h"
#define N 700

using namespace std;

int main()
    {
    cout << "In CDArray will be added " << N << " elements!" << endl;
    cout << "Inpute minimum value!" << endl;
    int min;
    cin >> min >> endl;
    cout << "Inpute maximum value!" << endl;
    int max;
    cin >> max >> endl;
    CDArray<int> array;

    for (int i = 0; i < N; ++i)
        array.Add (rand ());
    cout << "Array values : " << endl;
    for (int i = 0; i < array.Number (); ++i)
        cout << array[i] << ' ';

    for (int i = 0; i < N/2; ++i)
        array.Delete (1);
    cout << "New array values : " << endl;
    for (int i = 0; i < array.Number(); ++i)
        cout << array[i] << ' ';

    std::sort(array.Ptr(), array.Ptr() + array.Number());
    cout << "Sorted array values : " << endl;
    for (int i = 0; i < array.Number(); ++i)
        cout << array[i] << ' ';
    return 0;
    }

