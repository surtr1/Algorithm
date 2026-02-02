#include <iostream>
#include <vector>
#include <cstring>
using namespace std;
using i64 = long long;
const int mod = 1e9 + 7;
const int N = 110;

struct matrix {
	i64 c[N][N];
	matrix() { memset(c, 0, sizeof c); }
} a, res;
i64 n, k;

matrix operator*(matrix &a, matrix &b) {
	matrix t;
	// k和j这两行互换有常数级别的优化
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) { // k和j这两行互换有常数级别的优化
			for (int k = 1; k <= n; k++) {
				t.c[i][j] = (t.c[i][j] + a.c[i][k] * b.c[k][j]) % mod;
			}
		}
	}
	return t;
}

matrix qpower(matrix &a, i64 k) {
	matrix res;
	for (int i = 1; i <= n; i++)
		res.c[i][i] = 1; // 先初始化为单位矩阵
	while (k) {
		if (k & 1)
			res = res * a;
		a = a * a;
		k >>= 1;
	}
	return res;
}

int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> a.c[i][j];
		}
	}
	res = qpower(a, k);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cout << res.c[i][j] << ' ';
		}
		cout << '\n';
	}
	return 0;
}
