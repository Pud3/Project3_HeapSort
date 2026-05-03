/*
  CS-221: Project 3 - Heap Sort on Arrays + Operation Counting

  Name: Jonathan Puddie
  Date: 5/3/26
*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAX_SIZE = 5000;

string ArrayPreview(int* array, int size, int limit = 20) {
    if (size == 0) {
        return "[]";
    }

    string result = "[";
    int displayCount = (size < limit) ? size : limit;

    for (int i = 0; i < displayCount; i++) {
        result += to_string(array[i]);

        if (i < displayCount - 1) {
            result += ", ";
        }
    }

    if (size > limit) {
        result += ", ...";
    }

    result += "]";
    return result;
}

bool LoadNumbers(const string& filename, int* array, int& count) {
    ifstream fin(filename);

    if (!fin) {
        return false;
    }

    count = 0;

    while (count < MAX_SIZE && fin >> array[count]) {
        count++;
    }

    fin.close();
    return true;
}

void CopyFirstN(int* source, int* destination, int n) {
    for (int i = 0; i < n; i++) {
        destination[i] = source[i];
    }
}

void Heapify(int* numbers, int size, int root, long long& comparisons, long long& moves) {
    int largest = root;
    int left = 2 * root + 1;
    int right = 2 * root + 2;

    if (left < size) {
        comparisons++;

        if (numbers[left] > numbers[largest]) {
            largest = left;
        }
    }

    if (right < size) {
        comparisons++;

        if (numbers[right] > numbers[largest]) {
            largest = right;
        }
    }

    if (largest != root) {
        int temp = numbers[root];
        numbers[root] = numbers[largest];
        numbers[largest] = temp;
        moves += 3;

        Heapify(numbers, size, largest, comparisons, moves);
    }
}
// Heap sort main function
void HeapSort(int* numbers, int size, long long& comparisons, long long& moves) {
    comparisons = 0;
    moves = 0;

    // Build max heap
    for (int i = size / 2 - 1; i >= 0; i--) {
        Heapify(numbers, size, i, comparisons, moves);
    }

    // Extract elements
    for (int i = size - 1; i > 0; i--) {
        int temp = numbers[0];
        numbers[0] = numbers[i];
        numbers[i] = temp;
        moves += 3;

        Heapify(numbers, i, 0, comparisons, moves);
    }
}

void TestHeapSort(int* allNumbers, int size) {
    int* subset = new int[size];

    CopyFirstN(allNumbers, subset, size);

    cout << "Dataset Size: " << size << endl;
    cout << "Unsorted Preview: " << ArrayPreview(subset, size) << endl;

    long long comparisons = 0;
    long long moves = 0;

    HeapSort(subset, size, comparisons, moves);

    cout << "Sorted Preview: " << ArrayPreview(subset, size) << endl;
    cout << "Comparisons: " << comparisons << endl;
    cout << "Moves: " << moves << endl;
    cout << "Total Operations: " << comparisons + moves << endl;
    cout << endl;

    delete[] subset;
}

int main() {
    int allNumbers[MAX_SIZE];
    int totalCount = 0;

    if (!LoadNumbers("numbers_dataset_5000.txt", allNumbers, totalCount)) {
        cout << "Error: could not open file." << endl;
        return 1;
    }

    TestHeapSort(allNumbers, 100);
    TestHeapSort(allNumbers, 1000);
    TestHeapSort(allNumbers, 5000);

   

    return 0;
}

/*
Reflection:

For heap sort, I counted each comparison between two array values as one
comparison. I counted each swap as three moves because a swap uses a temporary
variable (temp = a, a = b, b = temp).

Heap sort is more efficient than insertion sort for large datasets because it
runs in O(n log n), while insertion sort runs in O(n^2). This means heap sort
handles 1000 and 5000 values much better.

Compared to the linked list insertion sort from Project 2, heap sort was easier
to manage because it uses array indexes instead of pointers. The linked list
version was more complex and still slower for large inputs.

Overall, heap sort is better for large datasets because its growth rate is lower
and it performs fewer operations as size increases.
*/