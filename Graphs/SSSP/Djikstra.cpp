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
struct Djikstra {
    using PII = pair<T, int>;
    int n;
    vector<vector<pair<int, T>>> g;
    vector<T> dis;
    vector<bool> vis;
    priority_queue<PII, vector<PII>, greater<PII>> pq;

    Djikstra() {}
    Djikstra(int _n) {
        init(_n);
    }
    void init(int _n) {
        n = _n;
        g.resize(_n + 1);
        dis.resize(_n + 1, numeric_limits<T>::max());
        vis.resize(_n + 1, false);
    }
    //加边
    void addEdge(int u, int v, int w) {
        g[u].emplace_back(v, w);
    }

    //运行
    void work(int st) {
        pq.emplace(0, st);
        dis[st] = 0;
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            if (vis[u]) continue;
            vis[u] = 1;
            for (auto [v, w] : g[u]) {
                if (dis[v] > dis[u] + w) {
                    dis[v] = dis[u] + w;
                    pq.emplace(dis[v], v);
                }
            }
        }
    }

    T getDis(int t) {
        return dis[t];
    }
};

void solve() {
    int n, m, s;
    cin >> n >> m >> s;
    Djikstra<long long> Dji(n);

    for (int i = 1; i <= m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        Dji.addEdge(u, v, w);
    }
    Dji.work(s);
    for (int i = 1; i <= n; i++) {
        cout << Dji.getDis(i) << " ";
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
