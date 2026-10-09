#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;
using namespace std::chrono;

const int N = 1000000;
const int NUM_ARRAYS = 10;
const int NUM_RUNS = 3;

void quickSort(vector<double>& arr, int left, int right) {
    int i = left;
    int j = right;
    double pivot = arr[left + (right - left) / 2];

    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;

        if (i <= j) {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    if (left < j) quickSort(arr, left, j);
    if (i < right) quickSort(arr, i, right);
}

void wrapperQuickSort(vector<double>& arr) {
    if (!arr.empty()) {
        quickSort(arr, 0, static_cast<int>(arr.size()) - 1);
    }
}

void heapify(vector<double>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<double>& arr) {
    int n = static_cast<int>(arr.size());

    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void merge(vector<double>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<double> L(n1);
    vector<double> R(n2);

    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<double>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void wrapperMergeSort(vector<double>& arr) {
    if (!arr.empty()) {
        mergeSort(arr, 0, static_cast<int>(arr.size()) - 1);
    }
}
void wrapperStdSort(vector<double>& arr) {
    sort(arr.begin(), arr.end());
}
bool isSorted(const vector<double>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i] < arr[i - 1]) {
            return false;
        }
    }
    return true;
}

double measureTime(void (*sortFunc)(vector<double>&),
                   const vector<double>& original,
                   bool& correct) {
    vector<double> arr = original;

    auto start = steady_clock::now();
    sortFunc(arr);
    auto stop = steady_clock::now();

    correct = isSorted(arr);
    return duration<double, milli>(stop - start).count();
}

int main() {
    cout << "Dang tao 10 day du lieu, moi day co 1.000.000 so thuc...\n";

    vector<vector<double>> datasets(NUM_ARRAYS, vector<double>(N));
    mt19937_64 rng(42);
    uniform_real_distribution<double> dist(0.0, 1000000.0);

    for (int i = 0; i < NUM_ARRAYS; i++) {
        for (int j = 0; j < N; j++) {
            datasets[i][j] = dist(rng);
        }
    }
    sort(datasets[0].begin(), datasets[0].end());
    sort(datasets[1].begin(), datasets[1].end(), greater<double>());

    cout << "Tao du lieu xong.\n";
    cout << "Moi thuat toan se duoc chay " << NUM_RUNS
         << " lan tren moi day; ket qua la thoi gian trung binh (ms).\n\n";

    cout << left
         << setw(24) << "Loai day"
         << right << setw(15) << "std::sort"
         << setw(15) << "QuickSort"
         << setw(15) << "HeapSort"
         << setw(15) << "MergeSort"
         << setw(12) << "Kiem tra" << '\n';

    cout << string(96, '-') << '\n';

    for (int i = 0; i < NUM_ARRAYS; i++) {
        double totalStd = 0.0;
        double totalQuick = 0.0;
        double totalHeap = 0.0;
        double totalMerge = 0.0;
        bool allCorrect = true;

        for (int run = 0; run < NUM_RUNS; run++) {
            bool okStd = false;
            bool okQuick = false;
            bool okHeap = false;
            bool okMerge = false;

            totalStd += measureTime(wrapperStdSort, datasets[i], okStd);
            totalQuick += measureTime(wrapperQuickSort, datasets[i], okQuick);
            totalHeap += measureTime(heapSort, datasets[i], okHeap);
            totalMerge += measureTime(wrapperMergeSort, datasets[i], okMerge);

            allCorrect = allCorrect && okStd && okQuick && okHeap && okMerge;
        }

        string label;
        if (i == 0) {
            label = "Day 1 (Tang dan)";
        } else if (i == 1) {
            label = "Day 2 (Giam dan)";
        } else {
            label = "Day " + to_string(i + 1) + " (Ngau nhien)";
        }

        cout << left << setw(24) << label
             << right << fixed << setprecision(2)
             << setw(15) << totalStd / NUM_RUNS
             << setw(15) << totalQuick / NUM_RUNS
             << setw(15) << totalHeap / NUM_RUNS
             << setw(15) << totalMerge / NUM_RUNS
             << setw(12) << (allCorrect ? "Dung" : "SAI") << '\n';
    }

    cout << "\nHoan tat thu nghiem.\n";
    cout << "Luu y: cac so lieu tren la thoi gian trung binh, don vi mili-giay (ms).\n";

    return 0;
}
