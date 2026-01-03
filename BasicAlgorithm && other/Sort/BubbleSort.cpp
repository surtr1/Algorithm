#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;
/*

*/

template<class T>
void BubbleSort(vector<T> & a, int l, int r) {
    int len = r - l;
    for (int k = 1; k <= len ; k++) {
        bool swaped = false;
        for (int i = l + 1; i <= r; i++) {
            if (a[i] < a[i - 1]) {
                std::swap(a[i], a[i - 1]);
                swaped = true;
            }
        }
        if (!swaped) break;
    }
}


void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    BubbleSort(a, 1, n);
    for (int i = 1; i <= n; i++) cout << a[i] << " ";
    cout << "\n"; 
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}