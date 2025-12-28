#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits>
using namespace std;

/*
Bellman–Ford 算法是一种基于松弛（relax）操作的最短路算法,
可以求出有负权的图的最短路,并可以对最短路不存在的情况进行判断。
在最短路存在的情况下,由于一次松弛操作会使最短路的边数至少 +1,
而最短路的边数最多为 n - 1，因此整个算法最多执行 n - 1轮松弛操作。
故总时间复杂度为 𝑂(𝑛𝑚)。
但还有一种情况，如果从S 点出发，抵达一个负环时，
松弛操作会无休止地进行下去。注意到前面的论证中已经说明了，
对于最短路存在的图，松弛操作最多只会执行 n - 1 轮，
因此如果第 n 轮循环时仍然存在能松弛的边，说明从 S 点出发，能够抵达一个负环。
*/
template<class T>
struct BellmanFord {
    struct Edge {
        int u, v;
        T w;
    };

    int n;
    const T INF = numeric_limits<T>::max() / 2; // 防止松弛时溢出
    vector<Edge> edges;
    vector<T> dis, backup;

    BellmanFord(int _n) {
        init(_n);
    }

    void init(int _n) {
        n = _n;
        edges.clear();
        dis.assign(n + 1, INF);
    }

    // 加边: u->v 权重 w
    void addEdge(int u, int v, T w) {
        edges.push_back({u, v, w});
    }
    
    //返回值判是否有负环
    bool work(int st) {
        dis.assign(n + 1, INF);
        dis[st] = 0;

        // 1. 松弛 n-1 轮
        for (int i = 0; i < n - 1; i++) {
            bool relaxed = false;
            for (const auto& e : edges) {
                // 级联更新: e.u 刚被更新，立马可以用来更新 e.v
                if (dis[e.u] != INF && dis[e.v] > dis[e.u] + e.w) {
                    dis[e.v] = dis[e.u] + e.w;
                    relaxed = true;
                }
            }
            if (!relaxed) return false; // 提前收敛
        }

        // 2. 判负环 (第 n 轮)
        for (const auto& e : edges) {
            if (dis[e.u] != INF && dis[e.v] > dis[e.u] + e.w) {
                return true; // 存在负环
            }
        }
        return false;
    }
    //严格只走k条边
    void k_edges(int st, int k) {
        dis.assign(n + 1, INF);
        dis[st] = 0;
        // 严格循环 k 次
        for (int i = 0; i < k; i++) {
            //备份当前状态，防止级联更新
            backup = dis; 
            for (const auto& e : edges) {
                // 使用 backup[e.u] 来更新 dis[e.v]
                if (backup[e.u] != INF && dis[e.v] > backup[e.u] + e.w) {
                    dis[e.v] = backup[e.u] + e.w;
                }
            }
        }
    }

    T getDis(int t) {
        return dis[t];
    }
};

/*
最短路
https://www.luogu.com.cn/problem/P1629
*/
void solve() {
    int n, m;
    cin >> n >> m;
    BellmanFord<long long> Bell(n), revBell(n);
    for (int i = 1; i <= m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        Bell.addEdge(u, v, w);
        revBell.addEdge(v, u, w);
    }
    Bell.work(1);
    revBell.work(1);
    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        ans += Bell.getDis(i) + revBell.getDis(i);
    }
    cout << ans << "\n";
}
/*
判负环
https://www.luogu.com.cn/problem/P3385
*/
void solve2() {
    int n, m;
    cin >> n >> m ;
    BellmanFord<long long> Bell(n);

    for (int i = 1; i <= m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        if(w >= 0) {
            Bell.addEdge(u, v, w);
            Bell.addEdge(v, u, w);
        } else {
            Bell.addEdge(u, v, w);

        }
    }
    if(Bell.work(1)) cout << "YES\n";
    else cout << "NO\n";
    
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    cin >> t;
    while (t--) solve2();
    return 0;
}