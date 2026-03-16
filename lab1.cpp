#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
#include <cstring>

using namespace std;
using namespace chrono;

// ГРУППА 1 : O(1) — константная сложность
int f01_access_middle(const int* a, int n) {
    return a[n / 2];
}

void f02_swap_ends(int* a, int n) {
    if (n < 2) return;
    int t = a[0];
    a[0] = a[n - 1];
    a[n - 1] = t;
}

// ГРУППА 2 : O(log n) — логарифмическая сложность
int f03_binary_search(const int* a, int n, int key) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int m = lo + (hi - lo) / 2;
        if (a[m] == key) return m;
        else if (a[m] < key) lo = m + 1;
        else hi = m - 1;
    }
    return -1;
}

int f04_lower_bound(const int* a, int n, int key) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int m = lo + (hi - lo) / 2;
        if (a[m] < key) lo = m + 1;
        else hi = m;
    }
    return lo;
}

// ГРУППА 3 : O(n) — линейная сложность
long long f09_sum(const int* a, int n) {
    long long s = 0;
    for (int i = 0; i < n; i++) s += a[i];
    return s;
}

int f07_find_max(const int* a, int n) {
    int mx = a[0];
    for (int i = 1; i < n; i++)
        if (a[i] > mx) mx = a[i];
    return mx;
}

// ГРУППА 4 : O(n log n) — линеарифмическая сложность
static void ms_merge(int* a, int* buf, int l, int m, int r) {
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r)
        buf[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    while (i <= m) buf[k++] = a[i++];
    while (j <= r) buf[k++] = a[j++];
    memcpy(a + l, buf + l, (r - l + 1) * sizeof(int));
}

static void ms_rec(int* a, int* buf, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    ms_rec(a, buf, l, m);
    ms_rec(a, buf, m + 1, r);
    ms_merge(a, buf, l, m, r);
}

void f14_merge_sort(int* a, int n) {
    int* buf = new int[n];
    ms_rec(a, buf, 0, n - 1);
    delete[] buf;
}

// ГРУППА 5 : O(n^2) — квадратичная сложность
void f18_bubble_sort(int* a, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (a[j] > a[j + 1]) {
                int t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
}

long long f22_count_dup_pairs(const int* a, int n) {
    long long c = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (a[i] == a[j]) c++;
    return c;
}

// ГРУППА 6 : O(n^3) — кубическая сложность
int f26_three_sum_naive(const int* a, int n, int target) {
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            for (int k = j + 1; k < n; k++)
                if (a[i] + a[j] + a[k] == target)
                    return 1;
    return 0;
}

// ГРУППА 7 : O(2^n) — экспоненциальная сложность
long long f29_fib_naive(int n) {
    if (n <= 1) return n;
    return f29_fib_naive(n - 1) + f29_fib_naive(n - 2);
}

// Генерация случайного массива
int* generate_array(int n, int max_val = 10000) {
    int* arr = new int[n];
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, max_val);
    for (int i = 0; i < n; i++) {
        arr[i] = dis(gen);
    }
    return arr;
}

// Функция для измерения времени
template<typename Func, typename... Args>
double measure_time(Func func, Args... args) {
    auto start = high_resolution_clock::now();
    func(args...);
    auto end = high_resolution_clock::now();
    return duration<double>(end - start).count();
}

// Специализация для f03_binary_search (нужен ключ)
double measure_binary_search(const int* a, int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 10000);
    int key = dis(gen);

    auto start = high_resolution_clock::now();
    f03_binary_search(a, n, key);
    auto end = high_resolution_clock::now();
    return duration<double>(end - start).count();
}

// Специализация для f26_three_sum_naive (нужен target)
double measure_three_sum(const int* a, int n) {
    int target = 15000;
    auto start = high_resolution_clock::now();
    f26_three_sum_naive(a, n, target);
    auto end = high_resolution_clock::now();
    return duration<double>(end - start).count();
}

// Главная функция
int main() {
    setlocale(LC_ALL, "Russian");
    cout << "Группа ИДБ-25-06, 16.03.2026, Лабораторная работа №1" << endl;
    cout << "Зинин П.А." << endl;
    cout << "Язык: C++17" << endl;

    // ТЕСТ 1: O(1) - f01_access_middle
    {
        cout << "\n--- Функция f01_access_middle (O(1))" << endl;
        cout << setw(12) << "Размер n" << setw(20) << "Время (сек)" << endl;
        cout << string(35, '-') << endl;

        vector<int> sizes = { 1000, 10000, 100000, 1000000, 10000000 };

        for (int n : sizes) {
            int* arr = generate_array(n);

            // Многократный вызов для точности
            int repeats = 1000000;
            auto start = high_resolution_clock::now();
            for (int i = 0; i < repeats; i++) {
                f01_access_middle(arr, n);
            }
            auto end = high_resolution_clock::now();
            double total = duration<double>(end - start).count();
            double avg_time = total / repeats;

            cout << setw(12) << n << setw(20) << scientific << setprecision(8) << avg_time << endl;

            delete[] arr;
        }
    }

    //ТЕСТ 2: O(log n) - f03_binary_search
    {
        cout << "\n--- Функция f03_binary_search (O(log n))" << endl;
        cout << setw(12) << "Размер n" << setw(20) << "Время (сек)" << endl;
        cout << string(35, '-') << endl;

        vector<int> sizes = { 1000, 10000, 100000, 1000000, 10000000, 100000000 };

        for (int n : sizes) {
            int* arr = generate_array(n);
            sort(arr, arr + n);

            int repeats = 10000;
            double total_time = 0;

            for (int i = 0; i < repeats; i++) {
                total_time += measure_binary_search(arr, n);
            }

            double avg_time = total_time / repeats;
            cout << setw(12) << n << setw(20) << scientific << setprecision(8) << avg_time << endl;

            delete[] arr;
        }
    }

    //ТЕСТ 3: O(n) - f09_sum
    {
        cout << "\n--- Функция f09_sum (O(n))" << endl;
        cout << setw(12) << "Размер n" << setw(20) << "Время (сек)" << endl;
        cout << string(35, '-') << endl;

        vector<int> sizes = { 1000, 10000, 100000, 1000000, 10000000 };

        for (int n : sizes) {
            int* arr = generate_array(n);

            double time = measure_time(f09_sum, arr, n);
            cout << setw(12) << n << setw(20) << fixed << setprecision(8) << time << endl;

            delete[] arr;
        }
    }

    //ТЕСТ 4: O(n) - f07_find_max
    {
        cout << "\n--- Функция f07_find_max (O(n))" << endl;
        cout << setw(12) << "Размер n" << setw(20) << "Время (сек)" << endl;
        cout << string(35, '-') << endl;

        vector<int> sizes = { 1000, 10000, 100000, 1000000, 10000000 };

        for (int n : sizes) {
            int* arr = generate_array(n);

            double time = measure_time(f07_find_max, arr, n);
            cout << setw(12) << n << setw(20) << fixed << setprecision(8) << time << endl;

            delete[] arr;
        }
    }

    //ТЕСТ 5: O(n log n) - f14_merge_sort
    {
        cout << "\n--- Функция f14_merge_sort (O(n log n))" << endl;
        cout << setw(12) << "Размер n" << setw(20) << "Время (сек)" << endl;
        cout << string(35, '-') << endl;

        vector<int> sizes = { 1000, 10000, 100000, 1000000 };

        for (int n : sizes) {
            int* arr = generate_array(n);

            double time = measure_time(f14_merge_sort, arr, n);
            cout << setw(12) << n << setw(20) << fixed << setprecision(6) << time << endl;

            delete[] arr;
        }
    }

    // ТЕСТ 6: O(n^2) - f18_bubble_sort 
    {
        cout << "\n--- Функция f18_bubble_sort (O(n^2))" << endl;
        cout << setw(12) << "Размер n" << setw(20) << "Время (сек)" << endl;
        cout << string(35, '-') << endl;

        vector<int> sizes = { 100, 500, 1000, 5000, 10000, 20000 };

        for (int n : sizes) {
            int* arr = generate_array(n);

            double time = measure_time(f18_bubble_sort, arr, n);
            cout << setw(12) << n << setw(20) << fixed << setprecision(6) << time << endl;

            delete[] arr;
        }
    }

    //ТЕСТ 7: O(n^2) - f22_count_dup_pairs
    {
        cout << "\n--- Функция f22_count_dup_pairs (O(n^2))" << endl;
        cout << setw(12) << "Размер n" << setw(20) << "Время (сек)" << endl;
        cout << string(35, '-') << endl;

        vector<int> sizes = { 100, 500, 1000, 5000, 10000 };

        for (int n : sizes) {
            int* arr = generate_array(n, 100);

            double time = measure_time(f22_count_dup_pairs, arr, n);
            cout << setw(12) << n << setw(20) << fixed << setprecision(6) << time << endl;

            delete[] arr;
        }
    }

    //  ТЕСТ 8: O(n^3) - f26_three_sum_naive 
    {
        cout << "\n--- Функция f26_three_sum_naive (O(n^3))" << endl;
        cout << setw(12) << "Размер n" << setw(20) << "Время (сек)" << endl;
        cout << string(35, '-') << endl;

        vector<int> sizes = { 100, 200, 400, 800, 1000 };

        for (int n : sizes) {
            int* arr = generate_array(n);

            double time = measure_three_sum(arr, n);
            cout << setw(12) << n << setw(20) << fixed << setprecision(6) << time << endl;

            delete[] arr;
        }
    }

    // ТЕСТ 9: O(2^n) - f29_fib_naive
    {
        cout << "\n--- Функция f29_fib_naive (O(2^n))" << endl;
        cout << setw(12) << "n" << setw(20) << "Время (сек)" << endl;
        cout << string(35, '-') << endl;

        vector<int> sizes = { 10, 15, 20, 25, 30, 35, 40, 45 };

        for (int n : sizes) {
            auto start = high_resolution_clock::now();
            f29_fib_naive(n);
            auto end = high_resolution_clock::now();
            double time = duration<double>(end - start).count();

            cout << setw(12) << n << setw(20) << fixed << setprecision(6) << time << endl;
        }
    }

    cout << "\n================================================" << endl;
    cout << "Измерения завершены. Для построения графиков" << endl;
    cout << "используйте полученные таблицы." << endl;

    return 0;
}
