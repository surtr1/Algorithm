#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;
/*
归并排序基于分治思想将数组分段排序后合并，
时间复杂度在最优、最坏与平均情况下均为 O(n log⁡n)
空间复杂度为 O(n)。
*/

template<class T>
void _MergeSort(vector<T>& a, int l, int r, vector<T>& tmp) {
    if (l >= r) return ;
    int mid = l + ((r - l) >> 1);
    _MergeSort(a, l, mid, tmp), _MergeSort(a, mid + 1, r, tmp);
    int i = l, j = mid + 1, k = l;
    while (i <= mid && j <= r) {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else tmp[k++] = a[j++];
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];
    for (i = l; i <= r; i++) {
        a[i] = tmp[i];
    }
}

template<class T>
void MergeSort(vector<T>& a, int l, int r) {
    if(l < r && !a.empty()) {
        vector<T> tmp(a.size());
        _MergeSort(a, l, r, tmp);
    }
}

//https://www.luogu.com.cn/problem/P1177
void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    MergeSort(a, 1, n);
    for (int i = 1; i <= n; i++) cout << a[i] << " ";
    cout << "\n"; 
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}