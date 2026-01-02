#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>

using namespace std;

template<class T>
struct Floyd {
    int n;
    vector<vector<T>> dis;  // 最短路矩阵
    vector<vector<T>> val;  // 原图邻接矩阵 (用于记录边权和重置)
    vector<vector<int>> pos;// 路由表：记录 pos[i][j] = k，即 i->j 中间经过了 k
    vector<int> path_res;   // 存储结果路径
    T min_cycle;            // 存储最小环长度
    const T INF = numeric_limits<T>::max() / 3;

    Floyd(int _n) {
        init(_n);
    }

    void init(int _n) {
        n = _n;
        dis.assign(n + 1, vector<T>(n + 1, INF));
        val.assign(n + 1, vector<T>(n + 1, INF));
        pos.assign(n + 1, vector<int>(n + 1, 0));
        
        // 只有在标准 Floyd 寻路时，对自己距离才初始化为 0
        // 在具体求解最小环函数中，会根据是有向还是无向重新处理对角线
        for (int i = 1; i <= n; ++i) {
            val[i][i] = 0;
        }
    }

    void addEdge(int u, int v, T w) {
        val[u][v] = min(val[u][v], w);
    }

    void MinDis() {
        dis = val;
        for (int k = 1; k <= n; k++) {
            for (int x = 1; x <= n; x++) {
                for (int y = 1; y <= n; y++) {
                    dis[x][y] = min(dis[x][y], dis[x][k] + dis[k][y]);
                }
            }
        }
    }

    // 递归还原路径 i -> ... -> j (不含 i 和 j)
    void get_path(int i, int j) {
        int k = pos[i][j];
        if (k == 0) return; // 直连，无中间点
        get_path(i, k);
        path_res.push_back(k);
        get_path(k, j);
    }

    // 有向图最小环
    void DirMinCycle() {
        min_cycle = INF;
        path_res.clear();
        pos.assign(n + 1, vector<int>(n + 1, 0));
        // 对角线 dis[i][i] 必须设为 INF
        // 这样才能算出 i -> ... -> i 的回路
        dis = val; 
        for(int i = 1; i <= n; ++i) dis[i][i] = INF;

        for (int k = 1; k <= n; k++) {
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= n; j++) {
                    if (dis[i][k] != INF && dis[k][j] != INF) {
                        if (dis[i][k] + dis[k][j] < dis[i][j]) {
                            dis[i][j] = dis[i][k] + dis[k][j];
                            pos[i][j] = k; // 记录中间点
                        }
                    }
                }
            }
        }

        int start = -1;
        for (int i = 1; i <= n; i++) {
            if (dis[i][i] < min_cycle) {
                min_cycle = dis[i][i];
                start = i;
            }
        }
        if (start != -1) {
            path_res.push_back(start);       // 起点
            get_path(start, start);     // 递归找中间点
            // path_res 此时包含：起点 -> 中间点... 
            // 这是一个闭环序列，如 1 -> 2 -> 3 (意味着 1->2, 2->3, 3->1)
        }
    }

    // 无向图最小环
    void UndirMinCycle() {
        min_cycle = INF;
        path_res.clear();
        pos.assign(n + 1, vector<int>(n + 1, 0));
        //无向图对角线设为 0，因为要算两点间真实最短路
        dis = val;
        for(int i = 1; i <= n; ++i) dis[i][i] = 0;

        for (int k = 1; k <= n; k++) {
            // (1) 先判断环 (必须在更新 dis 之前)
            // 环由 i->j (最短路, 不经过k) + i->k (直接) + k->j (直接) 组成
            for (int i = 1; i < k; i++) {
                for (int j = 1; j < i; j++) {
                    if (dis[i][j] != INF && val[i][k] != INF && val[k][j] != INF) {
                        T cur_len = dis[i][j] + val[i][k] + val[k][j];
                        if (cur_len < min_cycle) {
                            min_cycle = cur_len;
                            // 记录路径
                            path_res.clear();
                            path_res.push_back(i);    // 环的一部分 i
                            get_path(i, j);           // i 到 j 之间的路
                            path_res.push_back(j);    // 环的一部分 j
                            path_res.push_back(k);    // 环的最大节点 k
                        }
                    }
                }
            }
            // (2) 后更新最短路
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= n; j++) {
                    if (dis[i][k] != INF && dis[k][j] != INF) {
                        if (dis[i][k] + dis[k][j] < dis[i][j]) {
                            dis[i][j] = dis[i][k] + dis[k][j];
                            pos[i][j] = k;
                        }
                    }
                }
            }
        }
    }
    vector<int> getPath() {
        return path_res;
    }
};


/*
https://www.luogu.com.cn/problem/B3647
*/
void solve1() {
    int n, m;
    cin >> n >> m;
    Floyd<long long> work(n);
    for (int i = 1; i <= m; i++){
        int u, v, w;
        cin >> u >> v >> w;
        work.addEdge(u, v, w);
        work.addEdge(v, u, w);
    } 
    work.MinDis();
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++)
            cout << work.dis[i][j] << " ";
        cout << "\n";
    }
}

/*
最小环：
https://www.luogu.com.cn/problem/P6175
*/
void solve2() {
    int n, m;
    cin >> n >> m;
    Floyd<long long> work(n);
    for (int i = 1; i <= m; i++){
        int u, v, w;
        cin >> u >> v >> w;
        work.addEdge(u, v, w);
        work.addEdge(v, u, w);
    } 
    work.UndirMinCycle();
    long long w = work.min_cycle;
    if (w == work.INF) cout << "No solution.\n";
    else cout << w << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while (t--) solve2();
    return 0;
}