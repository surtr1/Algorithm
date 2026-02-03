#include <vector>
#include <cmath>
#include <map>
#include <algorithm>
using namespace std;

// 试除法求约数
vector<int> get_divide(int n) {
    vector<int> divides;

    int t = sqrt(n);
    for (int i = 1; i <= t; i++) {
        if (n % i == 0) {
            divides.push_back(i);
            if (i != n / i)
                divides.push_back(n / i);
        }
    }
    sort(divides.begin(), divides.end());
    return divides;
}

// 分解质因数然后求约数之和约数个数
map<int, int> mp;
void get_divide_cnt(int n) {
    for (int i = 2; i <= n / i; i++) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                cnt++;
                n /= i;
            }
            mp[i] += cnt;
        }
    }
    if (n > 1)
        mp[n]++;
}
// N = (p1^x1)(p^x2)(p3^x3)…(pk^xk)
// N的约数个数为：(x1+1)(x2+1)(x3+1)…(xk+1)
// N的约数之和为；(p1^0+p1^1+...+p1^x1)*...*(pk^0*pk^1*...*pk^xk)
// ll t = 1;
// 求和：a是质因子，b 是幂次
// while (b--)
    // t = (t * a + 1) % mod;
