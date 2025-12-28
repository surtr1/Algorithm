#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits>
using namespace std;

/*
不能处理有负权边的情况
可以解决无负权边图的最短路问题
n 为图上点的数目, m 为图上边的数目;
时间复杂度 (n + m) * log2(n)

*/
template<class T>
struct Dijkstra {
    struct Edge {
        int v;
        T w;
    };
    
    int n;
    const T INF = numeric_limits<T>::max(); 
    vector<vector<Edge>> g;
    vector<T> dis;
    vector<bool> vis;

    Dijkstra() {}
    Dijkstra(int _n) {
        init(_n);
    }

    void init(int _n) {
        n = _n;
        g.assign(n + 1, vector<Edge>());
        dis.assign(n + 1, INF);
        vis.assign(n + 1, false);
    }

    void addEdge(int u, int v, T w) {
        g[u].push_back({v, w});
    }

    void work(int st) {
        priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> pq;
        dis[st] = 0;
        pq.push({0, st});
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (vis[u]) continue;
            vis[u] = true;

            for (auto& e : g[u]) {
                if (dis[e.v] > dis[u] + e.w) {
                    dis[e.v] = dis[u] + e.w;
                    pq.push({dis[e.v], e.v});
                }
            }
        }
    }
    
    T getDis(int t) {
        return dis[t];
    }
};

/*
https://www.luogu.com.cn/problem/P4779
*/
void solve() {
    int n, m, s;
    cin >> n >> m >> s;
    Dijkstra<long long> Dijk(n);

    for (int i = 1; i <= m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        Dijk.addEdge(u, v, w);
    }
    Dijk.work(s);
    for (int i = 1; i <= n; i++) {
        cout << Dijk.getDis(i) << " ";
    }
    cout << "\n";
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while (t--) solve();
    return 0;
}