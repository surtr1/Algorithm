/**
 * 快速幂模板 时间复杂度O(logb)
 * 
 */

using i64 = long long;
using i128 = __int128_t;

i64 power(i64 a, i64 b, i64 p) {
    i64 res = 1 % p;
    a %= p;
    for (; b; b >>= 1) {
        if (b & 1) res = (i128)res * a % p;
        a = (i128)a * a % p;
    }
    return res;
}
