/*
一维排序，二维归并
数对是无序的
*/
#include <bits/stdc++.h>
#define int long long
#define all(x) (x).begin(), (x).end()

using namespace std;
typedef long long ll;
const int N = 1e5 + 10;
struct node {
    node() {
        a = b = w = res = 0;
    }
    ll a, b, w, res;
} e[N];
struct cmp {
    bool operator()(const node &x, const node &y) const
    {
        if (x.a != y.a)
            return x.a < y.a;
        else
            return x.b < y.b;
    }
};

int n, k, cnt, ans = 0;

void CDQ(int l, int r) {
    int mid = (l + r) >> 1;
    if (l == r)
        return;
    CDQ(l, mid), CDQ(mid + 1, r);
    int p = l, q = mid + 1;
    vector<node> t;
    int tmp = 0;
    while (p <= mid && q <= r) {
        if (e[p].b <= e[q].b)
            tmp += e[p].w, t.push_back(e[p++]);
        else
            e[q].res += tmp * e[q].w, t.push_back(e[q++]);
    }

    while (p <= mid)
        tmp += e[p].w, t.push_back(e[p++]);
    while (q <= r)
        e[q].res += tmp * e[q].w, t.push_back(e[q++]);
    int now = 0;
    for (int i = l; i <= r; i++) {
        e[i] = t[now++];
    }
    return;
}

void solve() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> e[i].a >> e[i].b;
        e[i].res = 0;
        e[i].w = 1;
    }
    sort(e + 1, e + 1 + n, cmp());
    for (int i = 1; i <= n; i++) {
        if ((cnt > 0) && (e[i].a == e[cnt].a) && (e[i].b == e[cnt].b)) {
            e[cnt].w++;
        }
        else
            e[++cnt] = e[i];
    }

    CDQ(1, cnt);
    for (int i = 1; i <= cnt; i++) {
        ans += e[i].res + (e[i].w - 1ll) * e[i].w / 2;
        // cout<<"("<<e[i].a<<" "<<e[i].b<<") "<<e[i].res<<" ";
    }
    // cout<<"\n";
    cout << ans;
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

/*
4
1 1
1 1
1 1
1 1
*/