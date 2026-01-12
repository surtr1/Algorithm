/**
 * 组合数与取模
 */

#include <vector>


const int MOD = 1e9 + 7;
long long qpow(long long a, long long b) {
    long long res = 1;
    a %= MOD;
    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

struct Comb {
    int n;
    std::vector<long long> _fac;    
    std::vector<long long> _invfac; 
    std::vector<long long> _inv;    
    
    Comb() : n{0}, _fac{1}, _invfac{1}, _inv{0} {}
    Comb(int n) : Comb() {
        init(n);
    }
    
    void init(int m) {
        if (m <= n) return;
        _fac.resize(m + 1);
        _invfac.resize(m + 1);
        _inv.resize(m + 1);
        
        for (int i = n + 1; i <= m; i++) {
            _fac[i] = _fac[i - 1] * i % MOD;
        }
        
        _invfac[m] = qpow(_fac[m], MOD - 2);
        
        for (int i = m; i > n; i--) {
            _invfac[i - 1] = _invfac[i] * i % MOD;
            _inv[i] = _invfac[i] * _fac[i - 1] % MOD;
        }
        n = m;
    }
    
    long long fac(int m) {
        if (m > n) init(2 * m);
        return _fac[m];
    }
    
    long long invfac(int m) {
        if (m > n) init(2 * m);
        return _invfac[m];
    }
    
    long long inv(int m) {
        if (m > n) init(2 * m);
        return _inv[m];
    }
    
    long long binom(int n, int m) {
        if (n < m || m < 0) return 0;
        return fac(n) * invfac(m) % MOD * invfac(n - m) % MOD;
    }

    long long P(int n, int m) {
        if (n < m || m < 0) return 0;
        return fac(n) * inv(n - m) % MOD;
    }
} comb;