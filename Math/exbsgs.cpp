/**
 * 以 O(sqrt(P))的复杂度求解 a ^ x == b (mod P)。其中标准 bsgs 算法不能计算 a 与 MOD 互质的情
 * 况，而 exbsgs 则可以。
 * 
 */



#include <iostream>
#include <map>
#include <cmath>
using namespace std;

namespace BSGS
{
    using LL = long long;
    LL a, b, p;
    map<LL, LL> f;
    inline LL gcd(LL a, LL b) { return b > 0 ? gcd(b, a % b) : a; }
    inline LL ps(LL n, LL k, int p) {
        LL r = 1;
        for (; k; k >>= 1) {
            if (k & 1)
                r = r * n % p;
            n = n * n % p;
        }
        return r;
    }
    void exgcd(LL a, LL b, LL &x, LL &y) {
        if (!b) {
            x = 1, y = 0;
        }
        else {
            exgcd(b, a % b, x, y);
            LL t = x;
            x = y;
            y = t - a / b * y;
        }
    }
    LL inv(LL a, LL b) {
        LL x, y;
        exgcd(a, b, x, y);
        return (x % b + b) % b;
    }
    LL bsgs(LL a, LL b, LL p) {
        f.clear();
        int m = ceil(sqrt(p));
        b %= p;
        for (int i = 1; i <= m; i++)
        {
            b = b * a % p;
            f[b] = i;
        }
        LL tmp = ps(a, m, p);
        b = 1;
        for (int i = 1; i <= m; i++)
        {
            b = b * tmp % p;
            if (f.count(b) > 0)
                return (i * m - f[b] + p) % p;
        }
        return -1;
    }
    LL exbsgs(LL a, LL b, LL p) {
        if (b == 1 || p == 1)
            return 0;
        LL g = gcd(a, p), k = 0, na = 1;
        while (g > 1) {
            if (b % g != 0)
                return -1;
            k++;
            b /= g;
            p /= g;
            na = na * (a / g) % p;
            if (na == b)
                return k;
            g = gcd(a, p);
        }
        LL f = bsgs(a, b * inv(na, p) % p, p);
        if (f == -1)
            return -1;
        return f + k;
    }
}
// namespace BSGS
using namespace BSGS;
int main() {
    cin >> p >> a >> b;
    a %= p, b %= p;
    LL ans = exbsgs(a, b, p);
    if (ans == -1)
        cout << "no solution\n";
    else
        cout << ans << "\n";
    return 0;
}
