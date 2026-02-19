// https://www.luogu.com.cn/problem/P4609
#include <iostream>
#include <vector>

using namespace std;
using i64 = long long;
const int mod = 1e9 + 7;
const int N = 5e4 + 10;
const int M = 210;
i64 s[N][M], c[M][M];

void init() {
    s[0][0] = 1;
    for (int i = 1; i < N; i++) {
        for (int j = 1; j < M; j++) {
            s[i][j] = (s[i - 1][j - 1] + (i - 1) * s[i - 1][j]) % mod;
        }
    }
    for (int i = 0; i < M; i++)
        c[i][0] = 1;
    for (int i = 1; i < M; i++) {
        for (int j = 1; j <= i; j++) {
            c[i][j] = (c[i - 1][j - 1] + c[i - 1][j]) % mod;
        }
    }
}

int main() {
    init();
    int t;
    cin >> t;
    while (t--) {
        int n, a, b;
        cin >> n >> a >> b;
        cout << c[a + b - 2][a - 1] * s[n - 1][a + b - 2] % mod << '\n';
    }

    return 0;
}
