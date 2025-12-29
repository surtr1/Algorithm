#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits>
using namespace std;

/*
很多时候我们并不需要那么多无用的松弛操作。
很显然，只有上一次被松弛的结点，所连接的边，才有可能引起下一次的松弛操作。
那么我们用队列来维护「哪些结点可能会引起松弛操作」，就能只访问必要的边了。

SPFA 也可以用于判断 s 点是否能抵达一个负环，只需记录最短路经过了多少条边，
当经过了至少 𝑛条边时，说明 𝑠 点可以抵达一个负环。

虽然在大多数情况下 SPFA 跑得很快，但其最坏情况下的时间复杂度为O(nm)，
将其卡到这个复杂度也是不难的，所以考试时要谨慎使用
（在没有负权边时最好使用 Dijkstra 算法，在有负权边且题目中的图没有特殊性质时，
若 SPFA 是标算的一部分，题目不应当给出 Bellman–Ford 算法无法通过的数据范围）。
*/

template<class T>
struct Spfa {
    int n;
    vector<vector<pair<int, T>>> g; 
    vector<T> dis;
    vector<bool> inQueue; // 记录是否在队列中
    vector<int> cnt;      // 记录最短路经过的边数，用于判负环
    const T INF = numeric_limits<T>::max() / 2;

    Spfa(int _n) {
        init(_n);
    }

    void init(int _n) {
        n = _n;
        g.assign(n + 1, vector<pair<int, T>>());
        dis.assign(n + 1, INF);
        inQueue.assign(n + 1, false);
        cnt.assign(n + 1, 0);
    }

    // 加边
    void addEdge(int u, int v, T w) {
        g[u].push_back({v, w});
    }

    // SPFA
    // 返回值: true 表示可以从发现负环，false 表示正常
    bool work(int st) {
        // 重置状态
        dis.assign(n + 1, INF);
        inQueue.assign(n + 1, false);
        cnt.assign(n + 1, 0);
        queue<int> q;
        dis[st] = 0;
        q.push(st);
        inQueue[st] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            inQueue[u] = false; // 出队后标记为不在队列

            for (auto [v, w] : g[u]) {
                if (dis[v] > dis[u] + w) {
                    dis[v] = dis[u] + w;
                    cnt[v] = cnt[u] + 1; // 路径边数 +1
                    
                    // 如果路径边数 >= n，说明经过了 n 个点，必然存在环，且是负环
                    if (cnt[v] >= n) {
                        return true; // 发现负环
                    }
                    if (!inQueue[v]) {
                        q.push(v);
                        inQueue[v] = true;
                    }
                }
            }
        }
        return false; // 无负环
    }

    T getDis(int t) {
        return dis[t];
    }
};

/*
最短路
https://www.luogu.com.cn/problem/P3371
*/
void solve() {
    int n, m, s;
    cin >> n >> m >> s;
    Spfa<long long> sf(n);
    for (int i = 1; i <= m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        sf.addEdge(u, v, w);
    }
    sf.work(s);
    for (int i = 1; i <= n; i++) {
        long long d = sf.getDis(i);
        if (d == sf.INF) cout << (1<<31) - 1 << " ";
        else cout << d << " ";
    }

}
/*
判负环
https://www.luogu.com.cn/problem/P3385
*/
void solve2() {
    int n, m;
    cin >> n >> m ;
    Spfa<long long> sf(n);

    for (int i = 1; i <= m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        if(w >= 0) {
            sf.addEdge(u, v, w);
            sf.addEdge(v, u, w);
        } else {
            sf.addEdge(u, v, w);

        }
    }
    if(sf.work(1)) cout << "YES\n";
    else cout << "NO\n";
    
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while (t--) solve();
    return 0;
}