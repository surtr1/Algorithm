/**
 *  Lucas 定理专门用于求 binom(n,m) mod p$
 *  其中 n, m 很大（如 10^{18}），但 p 是较小的质数（通常 p < 10^5）。
 *  时间复杂度: 预处理:O(p) , 求解 O(log_p(n))
 **/


#include <vector>
#include <iostream>

using namespace std;

struct Lucas {
    long long p; // 模数 P，必须是质数
    vector<long long> fac;    // 阶乘
    vector<long long> invFac; // 阶乘逆元

    // 快速幂计算 (a^b) % p
    long long qpow(long long a, long long b) {
        long long res = 1;
        a %= p;
        while (b) {
            if (b & 1) res = res * a % p;
            a = a * a % p;
            b >>= 1;
        }
        return res;
    }

    // 构造函数：传入模数 p 并预处理阶乘和逆元
    // 时间复杂度: O(p)
    Lucas(long long _p) : p(_p) {
        init();
    }

    void init() {
        fac.resize(p);
        invFac.resize(p);

        fac[0] = 1;
        for (int i = 1; i < p; ++i) {
            fac[i] = fac[i - 1] * i % p;
        }

        // 费马小定理求逆元: a^(p-2) = inv(a) (mod p)
        invFac[p - 1] = qpow(fac[p - 1], p - 2);
        for (int i = p - 2; i >= 0; --i) {
            invFac[i] = invFac[i + 1] * (i + 1) % p;
        }
    }

    // 计算小范围组合数 C(n, m) % p，要求 n < p
    long long small_C(long long n, long long m) {
        if (m > n || m < 0) return 0;
        return fac[n] * invFac[m] % p * invFac[n - m] % p;
    }
    // Lucas 定理递归求解
    // 时间复杂度: O(log_p(n))
    long long solve(long long n, long long m) {
        if (m == 0) return 1;
        // Lucas 公式: C(n, m) % p = C(n % p, m % p) * C(n / p, m / p) % p
        return small_C(n % p, m % p) * solve(n / p, m / p) % p;
    }
};
