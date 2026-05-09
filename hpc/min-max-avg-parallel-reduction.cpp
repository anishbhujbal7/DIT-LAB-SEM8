#include <iostream>
#include <vector>
#include <omp.h>   // OpenMP library

using namespace std;

int main() {

    // Sample data 
    vector<int> arr = {10, 20, 5, 40, 25, 15};

    int n = arr.size();

    // Initialize variables
    int sum = 0;
    int min_val = arr[0];
    int max_val = arr[0];

    // ---------------- PARALLEL REGION ----------------
    #pragma omp parallel for reduction(+:sum) reduction(min:min_val) reduction(max:max_val)
    for (int i = 0; i < n; i++) {

        // Sum reduction
        sum += arr[i];

        // Min reduction
        if (arr[i] < min_val) {
            min_val = arr[i];
        }

        // Max reduction
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }

    // Calculate average
    double avg = (double)sum / n;

    // Print results
    cout << "Sum = " << sum << endl;
    cout << "Minimum = " << min_val << endl;
    cout << "Maximum = " << max_val << endl;
    cout << "Average = " << avg << endl;

    return 0;
}