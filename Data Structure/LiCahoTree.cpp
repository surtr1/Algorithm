/**
 * 李超树（静态版本）
 * 时间复杂度(O(n * logn))
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// T: Y坐标/斜率/截距的类型 (通常是 double 或 long long)
// P: X坐标的范围大小 (1e5, 1e9等，如果是离散化后的值则为 N)
template<typename T = double>
struct LiChaoTree {
    struct Line {
        T k, b;
        Line(T _k = 0, T _b = 0) : k(_k), b(_b) {}
    };

    int n; // X坐标的值域范围 [1, n]
    std::vector<Line> lines; // 存储所有线段，下标即 ID
    std::vector<int> tr;     // 线段树节点，存储的是线段的 ID

    // 构造函数
    // _n: 值域大小
    // min_val: 初始的极小值（用于 p[0]），如果是求最大值，设为 -INF
    LiChaoTree(int _n, T min_val = -1e18) {
        n = _n;
        tr.resize(4 * (n + 1));
        
        // 初始化第 0 条线段，作为空节点的默认值
        // 如果是求最大值，设为 -INF；如果是求最小值，设为 INF
        lines.push_back(Line(0, min_val)); 
    }

    // 计算线段 id 在 x 处的 y 值
    T calc(int id, int x) {
        return lines[id].k * x + lines[id].b;
    }

    // 内部递归插入：直线
    void _add_line(int u, int l, int r, int id) {
        int mid = (l + r) >> 1;
        // 核心逻辑：比较中点，优势大的留下，劣势的下放
        // 这里的 > 表示维护【最大值】(上凸包)，如果要维护最小值改成 <
        if (calc(id, mid) > calc(tr[u], mid)) std::swap(tr[u], id);
        
        if (l == r) return;
        
        // 只有可能在某一侧比当前优，递归下去
        if (calc(id, l) > calc(tr[u], l)) _add_line(u << 1, l, mid, id);
        else if (calc(id, r) > calc(tr[u], r)) _add_line(u << 1 | 1, mid + 1, r, id);
    }

    // 内部递归插入：线段
    void _add_seg(int u, int l, int r, int ql, int qr, int id) {
        if (ql <= l && r <= qr) {
            _add_line(u, l, r, id);
            return;
        }
        int mid = (l + r) >> 1;
        if (ql <= mid) _add_seg(u << 1, l, mid, ql, qr, id);
        if (qr > mid) _add_seg(u << 1 | 1, mid + 1, r, ql, qr, id);
    }

    // 内部递归查询
    T _query(int u, int l, int r, int x) {
        if (l == r) return calc(tr[u], x);
        int mid = (l + r) >> 1;
        T res = calc(tr[u], x); // 当前节点存储的线段在该点的值
        // 递归取 max
        if (x <= mid) res = std::max(res, _query(u << 1, l, mid, x));
        else res = std::max(res, _query(u << 1 | 1, mid + 1, r, x));
        return res;
    }

    // 插入直线 y = kx + b
    void insert_line(T k, T b) {
        lines.push_back(Line(k, b));
        _add_line(1, 1, n, lines.size() - 1);
    }

    // 插入线段，仅在 x \in [l, r] 有效
    void insert_segment(T k, T b, int l, int r) {
        lines.push_back(Line(k, b));
        _add_seg(1, 1, n, l, r, lines.size() - 1);
    }

    // 查询 x 处的最大值
    T query(int x) {
        return _query(1, 1, n, x);
    }
};


//使用示范
int main() {
    // 值域范围 1 ~ 50000
    // 初始化时传入 -INF (double类型)
    LiChaoTree<double> lct(50000, -1e18);

    int m;
    cin >> m;
    while (m--) {
        string op;
        cin >> op;
        if (op == "Project") {
            double k, b;
            cin >> b >> k; // 注意输入顺序，有些题是 y = b + kx
            // 假设这里的 Project 是插入一条直线  
            lct.insert_line(k, b);
        } else if (op == "Query") {
            int x;
            cin >> x;
            // 每次查询需要将时间设为当前（如果有点名机制），这里直接查询
            // 需要根据题目要求处理 x (比如 x-1)
            cout << (int)(lct.query(x) / 100) << endl; // 示例输出处理
        }
    }
    return 0;
}