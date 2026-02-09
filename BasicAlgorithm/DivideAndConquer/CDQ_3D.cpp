/*
对一维进行排序，然后第二维进行归并，第三维用树状数组
时间复杂度 O(n (logn)^2)
*/
#include <bits/stdc++.h>
// #define int long long
#define all(x) (x).begin(), (x).end()

using namespace std;
typedef long long ll;
const int N = 1e5 + 10;
struct node {
    node() {
        a = b = c = w = res = 0;
    }
    int a, b, c, w, res;
} e[N];
struct cmp {
    bool operator()(const node &x, const node &y) {
        if (x.a != y.a)
            return x.a < y.a;
        if (x.b != y.b)
            return x.b < y.b;
        return x.c < y.c;
    }
};

struct Fenwick {
    vector<int> tr;
    int n;
    void init(int _n) {
        tr.resize(_n + 1);
        n = _n;
    };
    Fenwick(int _n = 1) { init(_n); };
    void add(int x, int c) {
        for (; x <= n; x += x & -x)
            tr[x] += c;
    }
    int qry(int x) {
        int res = 0;
        for (; x >= 1; x -= x & -x) {
            res += tr[x];
        }
        return res;
    }
};
Fenwick tr;

int n, k, cnt;

void CDQ(int l, int r) {
    int mid = l + r >> 1;
    if (l == r)
        return;
    CDQ(l, mid), CDQ(mid + 1, r);
    int p = l, q = mid + 1;
    vector<node> t;
    while (p <= mid && q <= r) {
        if (e[p].b <= e[q].b)
            tr.add(e[p].c, e[p].w), t.push_back(e[p++]);
        else
            e[q].res += tr.qry(e[q].c), t.push_back(e[q++]);
    }
    while (p <= mid)
        tr.add(e[p].c, e[p].w), t.push_back(e[p++]);
    while (q <= r)
        e[q].res += tr.qry(e[q].c), t.push_back(e[q++]);
    for (int i = l; i <= mid; i++) {
        tr.add(e[i].c, -e[i].w);
    }
    int cnt = 0;
    for (int i = l; i <= r; i++) {
        e[i] = t[cnt++];
    }
}

void solve() {
    cin >> n >> k;
    for (int i = 1; i <= n; i++) {
        cin >> e[i].a >> e[i].b >> e[i].c;
        e[i].w = 1;
    }
    sort(e + 1, e + 1 + n, cmp());
    for (int i = 1; i <= n; i++) {
        if ((e[i].a == e[cnt].a) &&
            (e[i].b == e[cnt].b) && (e[i].c == e[cnt].c)) {
            e[cnt].w++;
        }
        else
            e[++cnt] = e[i];
    }
    tr.init(k);
    CDQ(1, cnt);
    vector<int> d(n + 1);
    for (int i = 1; i <= cnt; i++) {
        d[e[i].res + e[i].w - 1] += e[i].w;
    }
    for (int i = 0; i < n; i++) {
        cout << d[i] << "\n";
    }
}

int32_t main()
{
    ios::sync_with_stdio(false), cin.tie(nullptr);
    int _ = 1;
    //     cin>>_;
    while (_--)
        solve();

    return 0;
}