#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll n, mod[100009], yu[100009];

// 要用快速乘（龟速乘），防止爆long long
ll qMul(ll a, ll b, ll mod) {
    ll an = 0;
    while (b) {
        if (b & 1)
            an = (an + a) % mod;
        a = (a + a) % mod;
        b >>= 1;
    }
    return an % mod;
}

// 扩展欧几里得算法，返回gcd(a,b),并计算出ax+by = gcd(a,b)中的x和y
ll exGcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    ll gcd = exGcd(b, a % b, y, x); // 注意x和y的顺序
    y = y - a / b * x;
    return gcd;
}

int main() {
    ios::sync_with_stdio(false), cin.tie(0);
    cin >> n;
    for (int i = 1; i <= n; i++)
        cin >> mod[i] >> yu[i];
    // ans表示前i-1个方程式的特解（余数），M为前i-1个方程式的模数的最小公倍数(i从2开始)
    ll ans = yu[1], M = mod[1], t, y;
    for (int i = 2; i <= n; i++) {
        ll mi = mod[i], res = ((yu[i] - ans) % mi + mi) % mi; // res是等式的右边部分，不能出现负数
        ll gcd = exGcd(M, mi, t, y);                          // 求出gcd(mi,M)
        if (res % gcd != 0) {
            cout << -1 << endl;
            exit(0);
        }                           // 如果等式右边不能整除gcd，方程组无解
        t = qMul(t, res / gcd, mi); // 求出t还要乘上倍数，注意是快速乘取模mi (对谁取模要分清)
        ans += t * M;               // 得到前i个方程的特解（余数）
        M = mi / gcd * M;           // M等于lcm(M,mi)，注意乘法要在除法后面做，否则会爆long long
        ans = (ans % M + M) % M;    // 让特解范围限定在0~(M-1)内，防止会出现负数
    }
    cout << ans;
    return 0;
}