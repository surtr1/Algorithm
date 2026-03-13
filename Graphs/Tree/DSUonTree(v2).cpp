// https://www.luogu.com.cn/problem/U41492

#include <bits/stdc++.h>

using namespace std;
const int N = 1e5 + 10;
vector<int> g[N];
int n, q;
int col[N], ans[N];
set<int> tr[N];
int sz[N], son[N];
void dfs1(int u, int fa) {
    sz[u] = 1;
    for (auto v : g[u]) {
        if (v == fa)
            continue;
        dfs1(v, u);
        sz[u] += sz[v];
        if (sz[v] > sz[son[u]])
            son[u] = v;
    }
}

void dfs2(int u, int fa) {
    if (son[u]) {
        dfs2(son[u], u);
        tr[u].swap(tr[son[u]]);
    }
    tr[u].insert(col[u]);
    for (auto v : g[u]) {
        if (v == fa || v == son[u])
            continue;
        dfs2(v, u);
        for (auto c : tr[v]) {
            tr[u].insert(c);
        }
    }
    ans[u] = tr[u].size();
}
void solve() {
    cin >> n;
    for (int i = 1; i <= n - 1; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }
    dfs1(1, 0);
    for (int i = 1; i <= n; i++)
        cin >> col[i];
    dfs2(1, 0);
    cin >> q;
    while (q--) {
        int w;
        cin >> w;
        cout << ans[w] << "\n";
    }
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int _ = 1;
    // cin>>_;
    while (_--)
        solve();
    return 0;
}