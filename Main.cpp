/*
  CS-221: Project 3 - Heap Sort on Arrays + Operation Counting

  Name: Jonathan Puddie
  Date: 5/3/26
*/

#include <iostream>                       // Allows output with cout, this is so results can be displayed
#include <fstream>                       // Allows file reading, this is so numbers can be loaded from a dataset file
#include <string>                       // Allows string use, this is so the array preview can be returned as text
using namespace std;                   // Makes code shorter by avoiding std::, this is for easier reading

const int MAX_SIZE = 5000;            // Sets max amount of data, this is important to prevent overflow and control memory use

string ArrayPreview(int* array, int size, int limit = 20) {              // Creates a short preview of an array,this is so large datasets do not flood the screen
    if (size == 0) {                                                    // Checks if array is empty, this is to handle the no data case safely
        return "[]";
    }

    string result = "[";                                             // Starts the preview string, this is to make output look like a list
    int displayCount = (size < limit) ? size : limit;                // Picks how many values to show,this is to show only the smaller amount between size and limit

    for (int i = 0; i < displayCount; i++) {                       // Loops through preview values, a for loop was chosen because the number of items to show is known    
        result += to_string(array[i]);                            // Converts number to text, this is so it can be added to the preview string

        if (i < displayCount - 1) {                             // Checks if this is not the last preview value, this is to avoid an extra comma
            result += ", ";                                                          
        } 
    }

    if (size > limit) {                                       // Checks if more values exist, this is to show that the preview is shortened
        result += ", ...";                                   
    } 

    result += "]";                                          // Closes the preview, this to keep the list format clean
    return result;                                          // Returns preview text so it can be printed later
}

bool LoadNumbers(const string& filename, int* array, int& count) {      // Loads numbers from a file into an array, chosen to test with real data instead of typed values, uses bool as the return type so the function can tell if the file opened successfully or failed, allowing the program to safely handle errors instead of continuing with bad data
    ifstream fin(filename);                                            // Opens the file, this is so the program can read the dataset

    if (!fin) {                                                       // Checks if file failed to open this is to prevent errors
        return false;                                                 
    }

    count = 0;                                                      // Starts count at 0, this is so old values do not affect the amount loaded

    while (count < MAX_SIZE && fin >> array[count]) {              // Reads numbers while space remains,this is to avoid overflow and handle unknown file length
        count++;                                                  
    }

    fin.close();                                                      // Closes the file, this is to free resources
    return true;                                                     // Returns true, to tell main the file loaded successfully
}

void CopyFirstN(int* source, int* destination, int n) {          // Copies first n values, chosen to make test subsets without changing the original dataset,  uses void because the function does not need to return a value, it directly modifies the destination array, which makes the function simpler and avoids unnecessary return data
    for (int i = 0; i < n; i++) {                               // Loops n times,a for loop was chosen because we know exactly how many values need to be copied
        destination[i] = source[i];                           
    }
}
// Heapify restores max heap property
void Heapify(int* numbers, int size, int root, long long& comparisons, long long& moves) {    // Restores heap order from one root, this is because heap sort needs the largest value at the top
    int largest = root;                                                                      // Assumes root is largest first, chosen as the starting point before checking children
    int left = 2 * root + 1;                                                                // Finds left child index, this is because heaps stored in arrays uses this formula
    int right = 2 * root + 2;                                                              // Finds right child index, this is because heaps stored in arrays uses this formula
 
    if (left < size) {                                                                   // Checks if left child exists, this is to avoid going outside the array                                
        comparisons++;                                                                  // Counts comparison, this is to measure algorithm work

        if (numbers[left] > numbers[largest]) {                                       // Checks if left child is bigger, this is to keep the largest value on top,
            largest = left;
        }
    }

    if (right < size) {                                                           // Checks if right child exists, this is to avoid invalid index access
       comparisons++;                                                            // Counts comparison

        if (numbers[right] > numbers[largest]) {                               // Checks if right child is bigger, this is to find the true largest value          
            largest = right;
        }
    }

    if (largest != root) {                                                   // Checks if root is not largest, this is because a swap is needed to fix heap order
        int temp = numbers[root];                                           // Saves root value,this is so it is not lost during the swap

        numbers[root] = numbers[largest];                                 // Moves larger child to root, this is to restore max heap property    
        numbers[largest] = temp;                                         // Moves old root down, this is to complete the swap                                        
        moves += 3;                                                     // Adds 3 moves, this is because one swap uses three assignments

        Heapify(numbers, size, largest, comparisons, moves);           // Fixes the affected lower part, this is because swapping can break heap order below             
    }
}
// Heap sort main function                      
void HeapSort(int* numbers, int size, long long& comparisons, long long& moves) {          // Sorts array using heap sort, chosen because it is efficient for large datasets
    comparisons = 0;                                                                      // Resets comparisons, this is so each test starts fresh

    moves = 0;                                                                            // Resets moves, this so each test only counts its own work


    // Build max heap
    for (int i = size / 2 - 1; i >= 0; i--) {          // Starts from the last parent node,this is because leaf nodes are already valid heaps,a for loop is used because we know exactly which indices to process, and starting there is best since heapify works bottom up, fixing smaller subtrees first, so the whole heap builds correctly and efficiently
        Heapify(numbers, size, i, comparisons, moves);
    }

    // Extract elements
    for (int i = size - 1; i > 0; i--) {                          // Moves largest value to the end one by one,this was to build the sorted array

        int temp = numbers[0];                                  // Saves root value, this is important so it is not lost during swap
        numbers[0] = numbers[i];                               // Moves end value to root, chosen to replace the removed largest value
        numbers[i] = temp;                                    // Places largest value at end, this is because it is now in final sorted position
        moves += 3;                                           // Adds 3 moves, this is because one swap uses three assignments


        Heapify(numbers, i, 0, comparisons, moves);     // Restores heap after removal, chosen so the next largest value moves to the root
    }
}

void TestHeapSort(int* allNumbers, int size) {   // Tests heap sort on one dataset size, chosen to avoid repeating the same code for 100, 1000, and 5000, uses a separate function to keep the program organized and reusable, uses void because the function does not need to return a value, it directly prints the results to the screen
    int* subset = new int[size];                // Creates dynamic array, this is because size changes depending on the test

    CopyFirstN(allNumbers, subset, size);       // Copies test data, this important so original numbers stay unchanged

    cout << "Dataset Size: " << size << endl;                              // Prints dataset size
    cout << "Unsorted Preview: " << ArrayPreview(subset, size) << endl;   // Shows unsorted data, this is to compare before and after sorting

    long long comparisons = 0;             // Uses long long for comparisons, chosen because large datasets can create large counts
    long long moves = 0;                  // Uses long long for moves, chosen because operation counts can get large

    HeapSort(subset, size, comparisons, moves);                          // Sorts the subset, this is to test heap sort performance
    cout << "Sorted Preview: " << ArrayPreview(subset, size) << endl;   // Shows sorted data, this is to confirm the algorithm worked      
    cout << "Comparisons: " << comparisons << endl;                    // Prints comparisons
    cout << "Moves: " << moves << endl;                               // Prints moves
    cout << "Total Operations: " << comparisons + moves << endl;     // Prints total operations
    cout << endl;                                                      

    delete[] subset;                              // Frees dynamic memory, this is important to prevent memory leaks
}

int main() {     
    int allNumbers[MAX_SIZE];               // Stores all numbers from file, chosen for fast access by index
    int totalCount = 0;                    // Tracks how many numbers loaded, this is to know how much data is available

    if (!LoadNumbers("numbers_dataset_5000.txt", allNumbers, totalCount)) {  // Loads dataset, this is to use the numbers.txt file, an if statement is used to check if loading failed, which prevents the program from running with invalid or missing data and allows safe error handling
        cout << "Error: could not open file." << endl;                      // Prints error
        return 1;                                                          // Stops program
    }

    TestHeapSort(allNumbers, 100);                             // Tests small dataset, chosen to see performance on small input,  this calls the function to sort and measure operations on 100 elements, so i can observe baseline performance
    TestHeapSort(allNumbers, 1000);                           // Tests medium dataset, chosen to see how operations grow, this runs the same test on a larger set to compare how much work increases
    TestHeapSort(allNumbers, 5000);                          // Tests large dataset, chosen to test full input size, this runs the algorithm on the largest dataset to show how it performs at scale, and helps confirm time complexity behavior as input size grows

   

    return 0;                                            // Ends program successfully
}

/*
Reflection:

/*
Reflection:

As the dataset size increased, insertion sort became much slower and needed
a lot more operations. The number of comparisons and moves increased very
quickly when going from 100 to 1000 and then to 5000, showing that it is not
good for large datasets.

The linked-list insertion sort was harder to use compared to the array
insertion sort. The array version was easier because it uses simple indexing,
while the linked list needed pointers, which made it more confusing and easier
to mess up.

Heap sort worked much better than insertion sort on larger datasets.
Heap sort runs in O(n log n) and insertion sort runs in O(n^2), so
heap sort handled 1000 and 5000 values with fewer operations.

The easiest part to implement was the array-based insertion sort because it
was simple and easier to understand compared to linked lists or heap sort.

The best choice for large datasets is heap sort because it grows slower
than insertion sort. It needs fewer operations as the dataset gets bigger.

My results match O(n log n) because the number of operations in heap sort
increased more slowly than insertion sort, which is what we expect.
*/

/*
Personal Reflection:

In this project, I used arrays as the main data structure. Arrays were chosen
because heap sort works best with indexed data, where parent and child
relationships can be calculated using simple formulas like 2*i + 1 and 2*i + 2.
This makes arrays very efficient for representing a heap.

The main array (allNumbers) was used to store all values loaded from the file.
From this array, smaller subsets were created using the CopyFirstN function.
This allowed me to test heap sort on different dataset sizes (100, 1000, and 5000)
without changing the original data.

I also used a dynamic array inside the TestHeapSort function. This was chosen
because the size of the dataset changes depending on the test. Dynamic memory
allows flexibility, and deleting the array after use prevents memory leaks.

All in all, arrays were the best choice for this program because they allow fast
index access, work naturally with heap structures, and make it easy to test
performance across different input sizes.
*/
