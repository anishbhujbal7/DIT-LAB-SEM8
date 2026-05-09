#include <iostream>
#include <vector>
#include <omp.h>     // OpenMP library
using namespace std;

// ---------------- SEQUENTIAL BUBBLE SORT ----------------
void bubbleSortSequential(vector<int> &arr) {
    int n = arr.size();

    // Outer loop for passes
    for (int i = 0; i < n - 1; i++) {

        // Inner loop for comparison
        for (int j = 0; j < n - i - 1; j++) {

            // Swap if elements are in wrong order
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ---------------- PARALLEL BUBBLE SORT ----------------
void bubbleSortParallel(vector<int> &arr) {
    int n = arr.size();

  
    for (int i = 0; i < n; i++) {

        // EVEN phase
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }

        // ODD phase
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ---------------- MERGE FUNCTION ----------------
void merge(vector<int> &arr, int left, int mid, int right) {

    // Sizes of two subarrays
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Temporary arrays
    vector<int> L(n1), R(n2);

    // Copy data
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int i = 0; i < n2; i++)
        R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;

    // Merge sorted arrays
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    // Copy remaining elements
    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];
}

// ---------------- SEQUENTIAL MERGE SORT ----------------
void mergeSortSequential(vector<int> &arr, int left, int right) {
    if (left < right) {

        int mid = (left + right) / 2;

        // Sort left half
        mergeSortSequential(arr, left, mid);

        // Sort right half
        mergeSortSequential(arr, mid + 1, right);

        // Merge both halves
        merge(arr, left, mid, right);
    }
}

// ---------------- PARALLEL MERGE SORT ----------------
void mergeSortParallel(vector<int> &arr, int left, int right) {
    if (left < right) {

        int mid = (left + right) / 2;

        // Create parallel tasks
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSortParallel(arr, left, mid);

            #pragma omp section
            mergeSortParallel(arr, mid + 1, right);
        }

        // Merge after parallel sorting
        merge(arr, left, mid, right);
    }
}

// ---------------- MAIN FUNCTION ----------------
int main() {

    // Sample array
    vector<int> arr = {10, 7, 8, 9, 1, 5};
    vector<int> arr1 = arr;
    vector<int> arr2 = arr;
    vector<int> arr3 = arr;
    vector<int> arr4 = arr;

    double start, end;

    // -------- SEQUENTIAL BUBBLE --------
    start = omp_get_wtime();
    bubbleSortSequential(arr1);
    end = omp_get_wtime();
    cout << "Sequential Bubble Sort Time: " << end - start << endl;

    // -------- PARALLEL BUBBLE --------
    start = omp_get_wtime();
    bubbleSortParallel(arr2);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sort Time: " << end - start << endl;

    // -------- SEQUENTIAL MERGE --------
    start = omp_get_wtime();
    mergeSortSequential(arr3, 0, arr3.size() - 1);
    end = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << end - start << endl;

    // -------- PARALLEL MERGE --------
    start = omp_get_wtime();
    mergeSortParallel(arr4, 0, arr4.size() - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << end - start << endl;

    return 0;
}


// g++ A2.cpp -fopenmp -o A2
// .\A2