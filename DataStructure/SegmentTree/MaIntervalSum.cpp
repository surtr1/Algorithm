#include <iostream>

#include <vector>

using namespace std;
const int N = 5e5 + 10;
int a[N];
struct Node {
    int l, r;
    int sum, lmax, rmax, mx;
} tr[4 * N];

void pushup(auto &u, auto &l, auto &r) {
    u.sum = l.sum + r.sum;
    u.lmax = max(l.lmax, l.sum + r.lmax);
    u.rmax = max(r.rmax, r.sum + l.rmax);
    u.mx = max(max(l.mx, r.mx), l.rmax + r.lmax);
}

void pushup(int u) {
    pushup(tr[u], tr[u << 1], tr[u << 1 | 1]);
}

void build(int u, int l, int r) {
    if (l == r)
        tr[u] = {l, r, a[l], a[l], a[l], a[l]};
    else {
        tr[u].l = l, tr[u].r = r;
        int mid = l + r >> 1;
        build(u << 1, l, mid), build(u << 1 | 1, mid + 1, r);
        pushup(u);
    }
}

void modify(int u, int x, int c) {
    if (tr[u].l == x && tr[u].r == x)
        tr[u] = {x, x, c, c, c, c};
    else {
        int mid = tr[u].l + tr[u].r >> 1;
        if (x <= mid)
            modify(u << 1, x, c);
        else
            modify(u << 1 | 1, x, c);
        pushup(u);
    }
}

Node query(int u, int l, int r) {
    if (tr[u].l >= l && tr[u].r <= r)
        return tr[u];
    else {
        int mid = tr[u].l + tr[u].r >> 1;
        Node res;
        if (r <= mid)
            res = query(u << 1, l, r);
        else if (l > mid)
            res = query(u << 1 | 1, l, r);
        else {
            auto left = query(u << 1, l, r);
            auto right = query(u << 1 | 1, l, r);
            pushup(res, left, right);
        }
        return res;
    }
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }
    build(1, 1, n);

    while (m--) {
        int op;
        int x, y;
        cin >> op >> x >> y;
        if (op == 1) {
            if (x > y)
                swap(x, y);
            cout << query(1, x, y).mx << '\n';
        }
        else {
            modify(1, x, y);
        }
    }
}
