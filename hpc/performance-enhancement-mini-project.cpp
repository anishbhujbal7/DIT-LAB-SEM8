#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ---------------- SEQUENTIAL QUICK SORT ----------------
void quickSort(vector<int> &arr, int left, int right) {
    if (left >= right) return;

    int pivot = arr[right];
    int i = left - 1;

    // Partitioning
    for (int j = left; j < right; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[right]);
    int pi = i + 1;

    // Recursive calls
    quickSort(arr, left, pi - 1);
    quickSort(arr, pi + 1, right);
}

// ---------------- MAIN ----------------
int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);   // Initialize MPI

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Process ID
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Total processes

    int n = 12;  // Total elements
    vector<int> data;

    double start, end;

    // ---------------- MASTER PROCESS ----------------
    if (rank == 0) {

        // Initialize array
        data = {12, 11, 13, 5, 6, 7, 2, 9, 1, 4, 8, 10};

        cout << "Original Array: ";
        for (int x : data) cout << x << " ";
        cout << endl;

        // -------- Sequential timing --------
        vector<int> seq = data;

        start = MPI_Wtime();
        quickSort(seq, 0, seq.size() - 1);
        end = MPI_Wtime();

        cout << "Sequential Time: " << end - start << endl;
    }

    // ---------------- PARALLEL PART ----------------

    int local_n = n / size;   // Elements per process
    vector<int> local_data(local_n);

    // Scatter data to all processes
    MPI_Scatter(data.data(), local_n, MPI_INT,
                local_data.data(), local_n, MPI_INT,
                0, MPI_COMM_WORLD);

    // Start parallel timing
    start = MPI_Wtime();

    // Each process sorts its part
    quickSort(local_data, 0, local_n - 1);

    // Gather sorted parts back
    vector<int> gathered;
    if (rank == 0) gathered.resize(n);

    MPI_Gather(local_data.data(), local_n, MPI_INT,
               gathered.data(), local_n, MPI_INT,
               0, MPI_COMM_WORLD);

    // Final merge (only master)
    if (rank == 0) {

        // Sort again to fully merge
        quickSort(gathered, 0, n - 1);

        end = MPI_Wtime();

        cout << "Parallel Time: " << end - start << endl;

        cout << "Sorted Array: ";
        for (int x : gathered) cout << x << " ";
        cout << endl;

        // -------- Speedup --------
        // (You can compute manually if needed)
    }

    MPI_Finalize(); // End MPI
    return 0;
}




//g++ hpc-mini-project.cpp -I"E:\programs\mpi-sdk\Include" -L"E:\programs\mpi-sdk\Lib\x64" -lmsmpi -o hpc-mini-project
// mpiexec -n 4 hpc-mini-project