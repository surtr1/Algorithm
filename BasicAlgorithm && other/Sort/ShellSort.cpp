#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;
/*
是一种不稳定的排序算法
平均时间复杂度为 O(n^{1.3})， 辅助空间复杂度: O(1)
*/

template<class T>
void ShellSort(vector<T> & a, int l, int r) {
    int len = r - l + 1;
    for (int gap = len >> 1; gap >= 1; gap >>= 1 ){
        for (int i = l + gap; i <= r; i++) {
            T key = a[i];
            int j = i;
            while (j >= l + gap && a[j - gap] > key) {
                a[j] = a[j - gap];
                j -= gap;
            }
            a[j] = key;
        }
    }
}


void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    ShellSort(a, 1, n);
    for (int i = 1; i <= n; i++) cout << a[i] << " ";
    cout << "\n"; 
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}