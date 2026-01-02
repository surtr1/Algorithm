#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

template <class T>
struct Johnson {
    struct Edge {
        int to;
        T w;
    };
    int n;
    vector<vector<Edge>> adj; 
    vector<T> h;             
    vector<vector<T>> dis;  // dis[i][j] 表示 i 到 j 的最短路
    const T INF = numeric_limits<T>::max() / 2;

    Johnson(int _n) : n(_n) {
        adj.resize(n + 1);
        h.assign(n + 1, 0); 
        dis.assign(n + 1, vector<T>(n + 1, INF));
    }

    void addEdge(int u, int v, T w) {
        adj[u].push_back({v, w});
    }

    // SPFA: 计算势能 h[]
    // 返回 false 表示存在负环
    bool spfa() {
        vector<int> cnt(n + 1, 0);
        vector<bool> in_queue(n + 1, false);
        queue<int> q;

        // 隐式构建虚拟源点：将所有点入队，初始势能 h=0
        for (int i = 1; i <= n; i++) {
            h[i] = 0;
            in_queue[i] = true;
            q.push(i);
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            in_queue[u] = false;
            for (const auto& e : adj[u]) {
                int v = e.to;
                T w = e.w;
                if (h[v] > h[u] + w) {
                    h[v] = h[u] + w;
                    if (!in_queue[v]) {
                        q.push(v);
                        in_queue[v] = true;
                        cnt[v]++;
                        if (cnt[v] > n) return false; // 负环检测
                    }
                }
            }
        }
        return true;
    }

    // Dijkstra: 计算 s 到其他点的最短路
    void dijkstra(int s) {
        priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> pq;
        //d是引入势能后的新最短路
        vector<T> d(n + 1, INF);
        vector<bool> vis(n + 1, false);

        d[s] = 0;
        pq.push({0, s});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (vis[u]) continue;
            vis[u] = true;

            for (const auto& e : adj[u]) {
                int v = e.to;
                // w_new = w + h[u] - h[v]
                T new_w = e.w + h[u] - h[v];

                if (d[v] > d[u] + new_w) {
                    d[v] = d[u] + new_w;
                    pq.push({d[v], v});
                }
            }
        }

        // 还原真实距离：RealDist = NewDist + h[v] - h[s]
        for (int i = 1; i <= n; i++) {
            if (d[i] == INF) {
                dis[s][i] = INF;
            } else {
                dis[s][i] = d[i] + h[i] - h[s];
            }
        }
    }

    bool work() {
        // 1. 计算势能
        if (!spfa()) return false; 
        // 2. N 次 Dijkstra
        for (int i = 1; i <= n; i++) {
            dijkstra(i);
        }
        return true;
    }
};


/*
https://www.luogu.com.cn/problem/P5905
*/
void solve() {
    int n, m;
    cin >> n >> m;
    Johnson<long long> john(n);
    for (int i = 1; i <= m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        john.addEdge(u, v, w);
    }
    if(!john.work()) {
        cout << "-1\n";
        return;
    }
    for (int i = 1; i <= n; i++) {
        long long ans = 0;
        for (int j = 1; j <= n; j++) {
            long long d = john.dis[i][j];
            if(d == john.INF) ans += 1e9 * j;
            else ans += j*d;
        }
        cout << ans << "\n";
    }
}



int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while (t--) solve();
    return 0;
}