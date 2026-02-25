/**
 * 稠密图
 * 使用邻接矩阵存图，以 N ^2 + M 的复杂度计算，思想与diji 基本一致。
 */

#include <cstring>
#include <iostream>

using namespace std;
const int N = 550, INF = 0x3f3f3f3f;
int n, m, g[N][N];

int d[N], v[N];
int prim() {
    memset(d, 0x3f, sizeof d); // 这里的d表示到“最小生成树集合”的距离
    int ans = 0;
    for (int i = 0; i < n; ++i) { // 遍历 n 轮
        int t = -1;
        for (int j = 1; j <= n; ++j)
            if (v[j] == 0 && (t == -1 || d[j] < d[t])) // 如果这个点不在集合内且当前距离集合最近
                t = j;
        v[t] = 1; // 将t加入“最小生成树集合”
        if (i && d[t] == INF)
            return INF; // 如果发现不连通，直接返回
        if (i)
            ans += d[t];
        for (int j = 1; j <= n; ++j)
            d[j] = min(d[j], g[t][j]); // 用t更新其他点到集合的距离
    }
    return ans;
}
int main() {
    memset(g, 0x3f, sizeof g);
    cin >> n >> m;
    while (m--)
    {
        int x, y, w;
        cin >> x >> y >> w;
        g[x][y] = g[y][x] = min(g[x][y], w);
    }
    int t = prim();
    if (t == INF)
        cout << "impossible" << endl;
    else
        cout << t << endl;
}