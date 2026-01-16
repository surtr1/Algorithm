/*
 * 动态开点李超线段树 (Dynamic Li Chao Tree)
 * * 应用场景:
 * 1. 维护平面上一组线段/直线的“下轮廓”或“上轮廓”。
 * 2. 解决斜率优化DP问题，特别是当查询坐标 x 或斜率 k 不单调，且值域很大时。
 * * 复杂度:
 * - 时间复杂度: 
 * - 插入直线: O(log V), V为横坐标值域
 * - 插入线段: O(log^2 V)
 * - 单点查询: O(log V)
 * - 空间复杂度: O(N log V), N为操作次数 (动态开点节省空间)
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


template<typename T = long long>
struct DynamicLiChao {
    const T INF_VAL = 4e18; // 极值，用于初始化
    
    struct Line {
        T k, b;
        Line(T _k = 0, T _b = 0) : k(_k), b(_b) {}
    };

    int tot;            // 节点计数
    int root;           // 根节点
    T min_x, max_x;     // 题目给定的横坐标范围 [min_x, max_x]
    
    vector<int> tr;     // 节点存储的线段ID (优势线段)
    vector<int> ls, rs; // 左右儿子编号
    vector<Line> lines; // 存储具体的线段信息

    // 构造函数
    // range_l, range_r: 横坐标的值域，如 -1e9, 1e9
    // max_ops: 预计最大操作次数（用于预分配内存，防止vector扩容带来的常数）
    DynamicLiChao(T range_l, T range_r, int max_ops = 200000) {
        min_x = range_l;
        max_x = range_r;
        tot = 0;
        root = 0;
        
        // 预分配内存，空间大约是 操作数 * log(值域)
        // 40 是 log2(2e9) + buffer
        int max_nodes = max_ops * 45; 
        tr.resize(max_nodes);
        ls.resize(max_nodes);
        rs.resize(max_nodes);
        // lines[0] 作为“空直线”或“默认最差情况”
        // 维护最小值时，p[0] 设为 INF；维护最大值时，p[0] 设为 -INF
        lines.push_back(Line(0, INF_VAL)); 
    }

    // 计算线段 id 在 x 处的 y 值
    T calc(int id, T x) {
        return lines[id].k * x + lines[id].b;
    }

    // 内部: 动态开点
    void new_node(int &u) {
        if (!u) u = ++tot;
    }

    // 内部: 插入直线
    // 维护区间最小值 (Min)
    void _add_line(int &u, T l, T r, int id) {
        new_node(u);
        if (tr[u] == 0) { // 如果该区间为空，直接覆盖
            tr[u] = id;
            return;
        }
        T mid = l + ((r - l) >> 1); 
        // 比较中点处的值
        // 如果新线段 id 在中点更优（更小），则交换，让 id 成为当前区间的优势线段
        if (calc(id, mid) < calc(tr[u], mid)) swap(tr[u], id);

        // 如果区间长度缩小到1，无法下放，结束
        if (l == r) return;
        // 比较两端点，看是否需要下放劣势线段
        // 注意：因为上面已经保证了 tr[u] 在 mid 处更优，
        // 所以交叉点只可能出现在 [l, mid] 或 [mid+1, r] 其中一边
        if (calc(id, l) < calc(tr[u], l)) _add_line(ls[u], l, mid, id);
        else if (calc(id, r) < calc(tr[u], r)) _add_line(rs[u], mid + 1, r, id);
    }

    // 内部: 插入线段
    void _add_seg(int &u, T l, T r, T ql, T qr, int id) {
        new_node(u);
        if (ql <= l && r <= qr) {
            _add_line(u, l, r, id);
            return;
        }
        T mid = l + ((r - l) >> 1);
        if (ql <= mid) _add_seg(ls[u], l, mid, ql, qr, id);
        if (qr > mid) _add_seg(rs[u], mid + 1, r, ql, qr, id);
    }

    // 内部: 查询
    T _query(int u, T l, T r, T x) {
        if (!u) return lines[0].k * x + lines[0].b; // 返回极值
        
        T res = calc(tr[u], x); // 当前节点的优势线段在 x 处的值
        
        if (l == r) return res;

        T mid = l + ((r - l) >> 1);
        if (x <= mid) res = min(res, _query(ls[u], l, mid, x));
        else res = min(res, _query(rs[u], mid + 1, r, x));
        
        return res;
    }

    // ================== 对外接口 ==================

    // 插入直线 y = kx + b
    void insert_line(T k, T b) {
        lines.push_back(Line(k, b));
        _add_line(root, min_x, max_x, lines.size() - 1);
    }

    // 插入线段 (仅在 x \in [l, r] 有效)
    void insert_segment(T k, T b, T l, T r) {
        lines.push_back(Line(k, b));
        _add_seg(root, min_x, max_x, l, r, lines.size() - 1);
    }

    // 查询 x 处的最小值
    T query(T x) {
        return _query(root, min_x, max_x, x);
    }
};

// 示例用法
int main() {
    // 优化 IO
    ios::sync_with_stdio(false);
    cin.tie(0);

    // 1. 初始化
    // 假设题目中 x 的范围是 -10^9 到 10^9，操作数大概 10^5
    // 注意：如果是求最大值，需修改模板中的 < 为 >, min 为 max, INF 为 -INF
    DynamicLiChao<long long> tree(-1e9, 1e9, 100000);

    int n;
    // 假设输入格式: 1 k b (插入直线), 2 x (查询)
    if (cin >> n) {
        while (n--) {
            int op;
            cin >> op;
            if (op == 1) {
                long long k, b;
                cin >> k >> b;
                tree.insert_line(k, b);
            } else if (op == 2) {
                long long k, b, l, r;
                cin >> k >> b >> l >> r;
                tree.insert_segment(k, b, l, r);
            } else if (op == 3) {
                long long x;
                cin >> x;
                cout << tree.query(x) << "\n";
            }
        }
    }
    return 0;
}