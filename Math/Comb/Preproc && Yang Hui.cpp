/**   组合数（小范围预处理，逆元+杨辉三角）
 *    暴力求解组合数与小范围逆元处理
**/
constexpr int P = 1000000007;
constexpr int L = 10000;
const int N = 1000;
int fac[L + 1], invfac[L + 1];
int sumbinom[N + 1][N + 1];

int binom(int n, int m) {
    if (n < m || m < 0) {
        return 0;
    }
    return 1LL * fac[n] * invfac[m] % P * invfac[n - m] % P;
}

int power(int a, int b) {
    int res = 1;
    for (; b; b /= 2, a = 1LL * a * a % P) {
        if (b % 2) {
            res = 1LL * res * a % P;
        }
    }
    return res;
}

int main() {
    fac[0] = 1;
    for (int i = 1; i <= L; i++) {
        fac[i] = 1LL * fac[i - 1] * i % P;
    }
    invfac[L] = power(fac[L], P - 2);
    for (int i = L; i; i--) {
        invfac[i - 1] = 1LL * invfac[i] * i % P;
    }

    sumbinom[0][0] = 1;
    for (int i = 0; i <= N; i++) {
        for (int j = 0; j <= i; j++) {
            if (!j) sumbinom[i][j] = 1;
            else sumbinom[i][j] = (sumbinom[i - 1][j] + sumbinom[i - 1][j - 1]) % P;
        }
    }
}
