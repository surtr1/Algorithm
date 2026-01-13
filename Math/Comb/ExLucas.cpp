/**
 * ExLucas 可以解决组合数n , m较大,取模数 p 很小的情况
 * p 可以是质数也可以为非质数
 * sum O(p_i^{k_i} log n)
 */
#include <iostream>

// 扩展卢卡斯定理模板
namespace ExLucas {

    typedef long long ll;
    // 快速幂
    ll qpow(ll a, ll b, ll p) {
        ll res = 1;
        a %= p;
        while (b) {
            if (b & 1) res = (res * a) % p;
            a = (a * a) % p;
            b >>= 1;
        }
        return res;
    }

    // 扩展欧几里得求逆元
    void exgcd(ll a, ll b, ll &x, ll &y) {
        if (!b) {
            x = 1, y = 0;
            return;
        }
        exgcd(b, a % b, x, y);
        ll tmp = x;
        x = y;
        y = tmp - a / b * y;
    }

    ll inv(ll a, ll p) {
        ll x, y;
        exgcd(a, p, x, y);
        return (x % p + p) % p;
    }
    // 计算 n! mod pk，排除 p 的倍数
    // 递归公式：fac(n) = p^(n/p) * fac(n/p) * ... (这里只处理非p倍数部分)
    ll fac(ll n, ll p, ll pk) {
        if (!n) return 1;
        ll res = 1;
        // 计算循环节 (1 到 pk 中非 p 倍数的乘积)
        for (ll i = 1; i < pk; ++i) {
            if (i % p) res = res * i % pk;
        }
        res = qpow(res, n / pk, pk);
        // 计算剩余部分
        for (ll i = 1; i <= n % pk; ++i) {
            if (i % p) res = res * i % pk;
        }
        return res * fac(n / p, p, pk) % pk;
    }

    // 计算 C(n, m) % pk
    ll C_pk(ll n, ll m, ll p, ll pk) {
        if (m < 0 || m > n) return 0;
        ll f1 = fac(n, p, pk);
        ll f2 = fac(m, p, pk);
        ll f3 = fac(n - m, p, pk);
        
        // 计算 n! 中含有的质因子 p 的个数 (Legendre 定理)
        ll cnt = 0;
        for (ll i = n; i; i /= p) cnt += i / p;
        for (ll i = m; i; i /= p) cnt -= i / p;
        for (ll i = n - m; i; i /= p) cnt -= i / p;
        
        return f1 * inv(f2, pk) % pk * inv(f3, pk) % pk * qpow(p, cnt, pk) % pk;
    }

    // 主函数：计算 C(n, m) % P
    ll solve(ll n, ll m, ll P) {
        ll ans = 0;
        ll tempP = P;
        // 对 P 进行质因数分解，分别计算 CRT 分量
        for (ll i = 2; i * i <= tempP; ++i) {
            if (tempP % i == 0) {
                ll pk = 1;
                while (tempP % i == 0) {
                    pk *= i;
                    tempP /= i;
                }
                // 计算当前模数 pk 下的组合数
                ll val = C_pk(n, m, i, pk);
                // CRT 合并: ans = val (mod pk), ans = old_ans (mod M_prev)
                // 这里使用通用的 CRT 合并公式
                ll M = P / pk;
                ll t = inv(M, pk);
                ans = (ans + val * M % P * t % P) % P;
            }
        }
        // 处理剩下的最大质因子
        if (tempP > 1) {
            ll pk = tempP;
            ll val = C_pk(n, m, pk, pk);
            ll M = P / pk;
            ll t = inv(M, pk);
            ans = (ans + val * M % P * t % P) % P;
        }
        return ans;
    }
}
/*
使用方式:
ExLucas::solve(n, m, p)

*/