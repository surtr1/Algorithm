// https://www.luogu.com.cn/problem/P3904

#include <iostream>
#include <cmath>
using namespace std;
const int N = 55;
int S[N][N][100], L[N][N];
// s[i][j]=s[i-1][j-1]+j*s[i-1][j]
void add(int x, int y) {
    L[x][y] = max(L[x - 1][y - 1], L[x - 1][y]);
    for (int i = 0; i < L[x][y]; i++) {
        S[x][y][i] += S[x - 1][y - 1][i] + y * S[x - 1][y][i];
        S[x][y][i + 1] += S[x][y][i] / 10;
        S[x][y][i] %= 10;
    }
    while (S[x][y][L[x][y]]) {
        S[x][y][L[x][y] + 1] += S[x][y][L[x][y]] / 10;
        S[x][y][L[x][y]] %= 10;
        L[x][y]++;
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    S[0][0][0] = 1;
    L[0][0] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= i; j++)
            add(i, j);

    if (!L[n][m])
        cout << 0;
    for (int i = L[n][m] - 1; i >= 0; i--)
        cout << S[n][m][i];
}
