#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

/*
快速排序的最优时间复杂度和平均时间复杂度为 O(nlog⁡n)
最坏时间复杂度为O(n^2)。

*/

template<class T>
void QuickSort(vector<T>& a, int low, int high) {
    if (low >= high) return;

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dis(low, high);
    T pivot = a[dis(gen)];

    //双指针划分
    int i = low, j = high;
    while (i <= j) {
        while (a[i] < pivot) i++;
        while (a[j] > pivot) j--;
        
        if (i <= j) {
            std::swap(a[i], a[j]);
            i++;
            j--;
        }
    }
    QuickSort(a, low, j);
    QuickSort(a, i, high);
}

template<class T>
void QuickSort(vector<T>& a) {
    if(!a.empty()) {
        QuickSort(a, 0, (int)a.size() - 1);
    }
}

//https://www.luogu.com.cn/problem/P1177
void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    QuickSort(a, 1, n);
    for (int i = 1; i <= n; i++) cout << a[i] << " ";
    cout << "\n"; 
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}