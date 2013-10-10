#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include "CDArray.h"
#define N 700

using namespace std;

template<class T>
void showCDArray (const CDArray<T>& array)
    {
    const int delimer = sqrt(array.Number());
    for (int i = 0; i < array.Number(); ++i)
        {
        if(! (i % delimer))
            cout << endl;
        cout << array[i] << ' ';
        }
    }

int main()
    {
    cout << "In CDArray will be added " << N << " elements!" << endl;
    cout << "Inpute minimum value : ";
    int min;
    cin >> min;
    cout << "Inpute maximum value : ";
    int max;
    cin >> max;
    CDArray<int> array;

    const int dMinMax = max - min;
    for (int i = 0; i < N; ++i)
        array.Add (min + rand() % dMinMax );
    cout << endl << "Array values : ";
    showCDArray(array);

    array.Delete (array.Number () / 10);
    cout << endl << "Array with deleted values : ";
    showCDArray(array);

    array.Resize (array.Number () / 2);
    cout << endl << "Array resized : ";
    showCDArray(array);

    std::sort(array.Ptr(), array.Ptr() + array.Number());
    cout << endl << "Sorted array values : ";
    showCDArray(array);

    for (int i = array.Number() / 2; i < array.Number(); ++i)
        if(array.Modify (i, -1))
            cout << endl << "Modify error : ";
    cout << endl << "Modified array values : ";
    showCDArray(array);
    return 0;
    }

