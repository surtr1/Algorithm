/**
 * Z是取模类 时间复杂度O(N log N）
 * n + 1个点可以唯一确定一个最高为 n 次的多项式。
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cassert>
using namespace std;
using LL = long long;

/**   取模类
 *    根据输入内容动态修改 MOD 的方法：Z::setMod(p) 。
 *    注意只能处理 P 为质数的情况
**/

using i64 = long long;

constexpr int P = 1000000007;
using Z = MInt<P>;

struct Lagrange {
    int n;
    vector<Z> x, y, fac, invfac;
    Lagrange(int n) {
        this->n = n;
        x.resize(n + 3);
        y.resize(n + 3);
        fac.resize(n + 3);
        invfac.resize(n + 3);
        init(n);
    }
    void init(int n) {
        iota(x.begin(), x.end(), 0);
        for (int i = 1; i <= n + 2; i++) {
            Z t;
            y[i] = y[i - 1] + power((Z)i, n);
        }
        fac[0] = 1;
        for (int i = 1; i <= n + 2; i++) {
            fac[i] = fac[i - 1] * i;
        }
        invfac[n + 2] = fac[n + 2].inv();
        for (int i = n + 1; i >= 0; i--) {
            invfac[i] = invfac[i + 1] * (i + 1);
        }
    }
    Z solve(LL k) {
        if (k <= n + 2) {
            return y[k];
        }
        vector<Z> sub(n + 3);
        for (int i = 1; i <= n + 2; i++) {
            sub[i] = k - x[i];
        }
        vector<Z> mul(n + 3);
        mul[0] = 1;
        for (int i = 1; i <= n + 2; i++) {
            mul[i] = mul[i - 1] * sub[i];
        }
        Z ans = 0;
        for (int i = 1; i <= n + 2; i++) {
            ans = ans + y[i] * mul[n + 2] * sub[i].inv() * pow(-1, n + 2 - i) *
                            invfac[i - 1] *
                            invfac[n + 2 - i];
        }
        return ans;
    }
};
template<class T>
constexpr T power(T a, i64 b) {
    T res = 1;
    for (; b; b /= 2, a *= a) {
        if (b % 2) {
            res *= a;
        }
    }
    return res;
}

constexpr i64 mul(i64 a, i64 b, i64 p) {
    i64 res = a * b - i64(1.L * a * b / p) * p;
    res %= p;
    if (res < 0) {
        res += p;
    }
    return res;
}




template<int P>
struct MInt {
    int x;
    constexpr MInt() : x{} {}
    constexpr MInt(i64 x) : x{norm(x % getMod())} {}
    
    static int Mod;
    constexpr static int getMod() {
        if (P > 0) {
            return P;
        } else {
            return Mod;
        }
    }
    constexpr static void setMod(int Mod_) {
        Mod = Mod_;
    }
    constexpr int norm(int x) const {
        if (x < 0) {
            x += getMod();
        }
        if (x >= getMod()) {
            x -= getMod();
        }
        return x;
    }
    constexpr int val() const {
        return x;
    }
    explicit constexpr operator int() const {
        return x;
    }
    constexpr MInt operator-() const {
        MInt res;
        res.x = norm(getMod() - x);
        return res;
    }
    constexpr MInt inv() const {
        assert(x != 0);
        return power(*this, getMod() - 2);
    }
    constexpr MInt &operator*=(MInt rhs) & {
        x = 1LL * x * rhs.x % getMod();
        return *this;
    }
    constexpr MInt &operator+=(MInt rhs) & {
        x = norm(x + rhs.x);
        return *this;
    }
    constexpr MInt &operator-=(MInt rhs) & {
        x = norm(x - rhs.x);
        return *this;
    }
    constexpr MInt &operator/=(MInt rhs) & {
        return *this *= rhs.inv();
    }
    friend constexpr MInt operator*(MInt lhs, MInt rhs) {
        MInt res = lhs;
        res *= rhs;
        return res;
    }
    friend constexpr MInt operator+(MInt lhs, MInt rhs) {
        MInt res = lhs;
        res += rhs;
        return res;
    }
    friend constexpr MInt operator-(MInt lhs, MInt rhs) {
        MInt res = lhs;
        res -= rhs;
        return res;
    }
    friend constexpr MInt operator/(MInt lhs, MInt rhs) {
        MInt res = lhs;
        res /= rhs;
        return res;
    }
    friend constexpr std::istream &operator>>(std::istream &is, MInt &a) {
        i64 v;
        is >> v;
        a = MInt(v);
        return is;
    }
    friend constexpr std::ostream &operator<<(std::ostream &os, const MInt &a) {
        return os << a.val();
    }
    friend constexpr bool operator==(MInt lhs, MInt rhs) {
        return lhs.val() == rhs.val();
    }
    friend constexpr bool operator!=(MInt lhs, MInt rhs) {
        return lhs.val() != rhs.val();
    }
};

template<>
int MInt<0>::Mod = 998244353;

template<int V, int P>
constexpr MInt<P> CInv = MInt<P>(V).inv();

