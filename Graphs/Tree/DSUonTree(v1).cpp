// https://www.luogu.com.cn/problem/U41492

#include <bits/stdc++.h>
/*
路径异或值等于 col[x]^col[y]
路径长度： dep[x]+dep[y]-2*dep[u]
*/

using namespace std;
const int N = 5e5 + 10;
vector<int> g[N];
int n, q;
int ans[N];
int sz[N], son[N], col[N], dep[N], rev[N], mx[1 << 24];
int L[N], R[N], cnt;
void dfs1(int u, int fa) {
    dep[u] = dep[fa] + 1, col[u] = col[fa] ^ col[u];
    sz[u] = 1, L[u] = ++cnt, rev[cnt] = u;
    for (auto v : g[u]) {
        if (v == fa)
            continue;
        dfs1(v, u);
        sz[u] += sz[v];
        if (sz[v] > sz[son[u]])
            son[u] = v;
    }
    R[u] = cnt;
}
void dfs2(int u, int fa, bool keep) {
    for (auto v : g[u]) {
        if (v == fa || v == son[u])
            continue;
        dfs2(v, u, 0);
        ans[u] = max(ans[u], ans[v]);
    }
    if (son[u]) {
        dfs2(son[u], u, 1);
        ans[u] = max(ans[u], ans[son[u]]);
    }
    if (mx[col[u]])
        ans[u] = max(ans[u], mx[col[u]] - dep[u]);
    for (int j = 23; j >= 0; j--) {
        int t = col[u] ^ (1 << j);
        if (mx[t])
            ans[u] = max(ans[u], mx[t] - dep[u]);
    }
    mx[col[u]] = max(mx[col[u]], dep[u]);

    for (auto v : g[u]) {
        if (v == fa || v == son[u])
            continue;
        for (int i = L[v]; i <= R[v]; i++) {
            if (mx[col[rev[i]]])
                ans[u] = max(ans[u], dep[rev[i]] + mx[col[rev[i]]] - 2 * dep[u]);
            for (int j = 23; j >= 0; j--) {
                int t = col[rev[i]] ^ (1 << j);
                if (mx[t])
                    ans[u] = max(ans[u], dep[rev[i]] + mx[t] - 2 * dep[u]);
            }
            // mx[col[rev[i]]]=max(mx[col[rev[i]]],dep[rev[i]]);
        }
        for (int i = L[v]; i <= R[v]; i++) {
            mx[col[rev[i]]] = max(mx[col[rev[i]]], dep[rev[i]]);
        }
    }
    if (!keep) {
        for (int i = L[u]; i <= R[u]; i++) {
            mx[col[rev[i]]] = 0;
        }
        return;
    }
}

void solve() {
    cin >> n;
    for (int i = 2; i <= n; i++) {
        int p;
        char c;
        cin >> p >> c;
        g[p].push_back(i), g[i].push_back(p);
        int nc = c - 'a';
        col[i] |= 1 << nc;
    }
    dfs1(1, 0);
    dfs2(1, 0, 1);
    for (int i = 1; i <= n; i++) {
        cout << ans[i] << " ";
    }
    cout << "\n";
}

int main()
{
    ios::sync_with_stdio(0), cin.tie(0);
    int _ = 1;
    // cin>>_;
    while (_--)
        solve();
    return 0;
}