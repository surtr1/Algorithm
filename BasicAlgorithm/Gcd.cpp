#include <algorithm>

using namespace std;
using ll = long long;

//时间复杂度 log(a + b) ，略慢于__gcd();
inline int mygcd(int a, int b) { 
    return b ? gcd(b, a % b) : a; 
}

ll gcd(ll a, ll b) { // 卡常 gcd！！
#define tz __builtin_ctzll
    if (!a || !b)
        return a | b;
    int t = tz(a | b);
    a >>= tz(a);
    while (b)
    {
        b >>= tz(b);
        if (a > b)
            swap(a, b);
        b -= a;
    }
    return a << t;
#undef tz
}