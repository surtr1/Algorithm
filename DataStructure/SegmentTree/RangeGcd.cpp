#include <iostream>
#include <vector>

using namespace std;
using i64 = long long;
const int N = 5e5 + 10;
i64 w[N];
i64 gcd(i64 a, i64 b) {
    return b ? gcd(b, a % b) : a;
}
struct Node {
    int l, r;
    i64 sum, d;
} tr[4 * N];

void pushup(auto &u, auto &l, auto &r) {
    u.sum = l.sum + r.sum;
    u.d = gcd(l.d, r.d);
}

void pushup(int u) {
    pushup(tr[u], tr[u << 1], tr[u << 1 | 1]);
}

void build(int u, int l, int r) {
    if (l == r) {
        tr[u] = {l, r, w[l] - w[l - 1], w[l] - w[l - 1]};
    }
    else {
        tr[u].l = l, tr[u].r = r;
        int mid = l + r >> 1;
        build(u << 1, l, mid), build(u << 1 | 1, mid + 1, r);
        pushup(u);
    }
}

void modify(int u, int x, i64 c) {
    if (tr[u].l == x && tr[u].r == x) {
        i64 tmp = tr[u].sum + c;
        tr[u] = {x, x, tmp, tmp};
    }
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
    if (tr[u].l >= l && tr[u].r <= r) {
        return tr[u];
    }
    else {
        int mid = tr[u].l + tr[u].r >> 1;
        if (r <= mid)
            return query(u << 1, l, r);
        else if (l > mid)
            return query(u << 1 | 1, l, r);
        else {
            auto left = query(u << 1, l, r);
            auto right = query(u << 1 | 1, l, r);
            Node res;
            pushup(res, left, right);
            return res;
        }
    }
}

int main() {
    i64 n, m;
    cin >> n >> m;

    for (int i = 1; i <= n; i++)
        cin >> w[i];
    build(1, 1, n);
    while (m--) {
        char op;
        int l, r;
        cin >> op >> l >> r;
        if (op == 'Q') {
            auto left = query(1, 1, l);
            Node right({0, 0, 0, 0});
            if (l + 1 <= r)
                right = query(1, l + 1, r);
            cout << abs(gcd(left.sum, right.d)) << '\n';
        }
        else {
            i64 c = 0;
            cin >> c;
            modify(1, l, c);
            if (r + 1 <= n)
                modify(1, r + 1, -c);
        }
    }

    return 0;
}
