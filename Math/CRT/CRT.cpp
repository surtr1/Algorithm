#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;
// 需要 mi 两两互质
const int N = 15;
long long n, a[N], m[N], w[N], M = 1, ans;
inline void exgcd(long long a, long long b, long long &x, long long &y) { // 扩展欧几里得求乘法逆元
    if (b == 0) {
        x = 1, y = 0;
        return;
    }
    exgcd(b, a % b, x, y);
    int z = x;
    x = y, y = z - y * (a / b);
}

int main() {
    scanf("%lld", &n);
    for (int i = 1; i <= n; i++) {
        int b;
        scanf("%d", &b);
        m[i] = b, M *= b;
        scanf("%lld", &a[i]);
    }

    for (int i = 1; i <= n; i++) {
        w[i] = M / m[i];
        long long x = 0, y = 0;
        exgcd(w[i], m[i], x, y);
        ans += a[i] * w[i] * (x < 0 ? x + m[i] : x);
    }
    printf("%lld", ans % M);
    return 0;
}