#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;
/*
是一种不稳定的排序算法
时间复杂度为 O(nlogn)， 辅助空间复杂度: O(1)

基于 0 -idx 的堆排序
Parent(i) = (i - 1) / 2;
LeftChild(i) = 2 * i + 1;
RightChild(i) = 2 * i + 2;
*/

/*
start: 当前堆在数组中的起始绝对下标 (即 l)
end:   当前堆的结束绝对下标 (即 r 或 i-1)
parent: 当前要下沉的节点的绝对下标
*/
template<class T> 
void siftDown(vector<T> & a, int l, int r, int parent) {
    
    int child = l + 2 * (parent - l) + 1;
    while (child <= r) {
        //找最大的儿子
        if (child + 1 <= r && a[child + 1] > a[child]) {
            child++;
        } 
        if (a[child] < a[parent]) return ;
        std::swap(a[parent], a[child]);
        parent = child;
        child = l + 2 * (parent - l) + 1;
    }
}


template<class T>
void HeapSort(vector<T> & a, int l, int r) {
    // 从最后一个节点的父节点开始 sift down 以完成堆化 (heapify)
    int sz = r - l + 1;
    for (int i = sz / 2 - 1; i >= 0; i--) {
        int parent = l + i; //偏移量 + 起点 就是真实值
        siftDown(a, l, r, parent);
    }
    //先将第一个元素和已经排好的元素前一位做交换，
    //再重新调整（刚调整的元素之前的元素），直到排序完毕
    for (int i = r; i > l; i--) {
        std::swap(a[i], a[l]);
        siftDown(a, l, i - 1, l); //剩余区间 [l, i - 1]
    }

}


//https://www.luogu.com.cn/problem/P1177
void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    HeapSort(a, 1, n);
    for (int i = 1; i <= n; i++) cout << a[i] << " ";
    cout << "\n"; 
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}