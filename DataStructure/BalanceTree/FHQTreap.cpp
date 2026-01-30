//https://www.luogu.com.cn/problem/P6136
#include <bits/stdc++.h>

using namespace std;

// 常量定义，根据题目要求调整大小
const int N = 2e6 + 10;
int read() {
    int x = 0, f = 1;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        if (ch == '-') f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    return x * f;
}

/**
 * FHQ Treap 类封装
 * 核心思想：无旋转，通过 Split (分裂) 和 Merge (合并) 维护平衡
 */
struct FHQTreap {
    // 树的节点定义
    struct Node {
        int l, r;   // 左右孩子索引
        int val;    // 节点数值
        int rnd;    // 随机优先级 (维护堆性质)
        int size;   // 子树大小
    } tr[N];

    int root;       // 根节点索引
    int idx;        // 节点分配计数器

    FHQTreap() {
        root = 0;
        idx = 0;
    }

    // 创建新节点
    void newnode(int &x, int v) {
        x = ++idx;
        tr[x].val = v;
        tr[x].rnd = rand(); // 随机优先级保证平衡
        tr[x].size = 1;
        tr[x].l = tr[x].r = 0;
    }

    // 更新节点信息 (主要是子树大小)
    void pushup(int u) {
        tr[u].size = tr[tr[u].l].size + tr[tr[u].r].size + 1;
    }

    /**
     * 核心操作：分裂 (Split)
     * @param p 当前节点
     * @param v 分裂的阈值 (按值分裂)
     * @param l 分裂后 <= v 的树的根引用
     * @param r 分裂后 > v 的树的根引用
     */
    void split(int p, int v, int &l, int &r) {
        if (!p) {
            l = r = 0;
            return;
        }
        if (tr[p].val <= v) {
            // 当前值 <= v，说明 p 及其左子树都属于左边的分裂树
            l = p;
            split(tr[l].r, v, tr[l].r, r); // 递归处理右子树
        } else {
            // 当前值 > v，说明 p 及其右子树都属于右边的分裂树
            r = p;
            split(tr[r].l, v, l, tr[r].l); // 递归处理左子树
        }
        pushup(p); // 递归回来后更新大小
    }

    /**
     * 核心操作：合并 (Merge)
     * @param l 左树根 (保证所有值 <= 右树)
     * @param r 右树根
     * @return 合并后的新根
     */
    int merge(int l, int r) {
        if (!l || !r) return l + r;
        // 按照 rnd 优先级维护 Heap 性质 (这里是大根堆还是小根堆无所谓，只要随机即可)
        if (tr[l].rnd < tr[r].rnd) {
            tr[l].r = merge(tr[l].r, r);
            pushup(l);
            return l;
        } else {
            tr[r].l = merge(l, tr[r].l);
            pushup(r);
            return r;
        }
    }

    // 插入数值 v
    void insert(int v) {
        int l, r, z;
        split(root, v, l, r);     // 按 v 分裂成 <=v 和 >v
        newnode(z, v);            // 新建节点
        // 重新合并：先合并不含 v 的左半部分和新节点，再合并右半部分
        root = merge(merge(l, z), r); 
    }

    // 删除数值 v (仅删除一个)
    void del(int v) {
        int x, y, z;
        split(root, v, x, z);      // 分裂成 <= v (x) 和 > v (z)
        split(x, v - 1, x, y);     // 再把 x 分裂成 < v (x) 和 == v (y)
        // 此时 y 树根即为要删除的 v。
        // merge(tr[y].l, tr[y].r) 相当于抛弃 y 的根节点，合并其左右孩子
        y = merge(tr[y].l, tr[y].r); 
        root = merge(merge(x, y), z); // 全部合并回去
    }

    // 查询 v 的排名 (小于 v 的数的个数 + 1)
    int getrank(int v) {
        int x, y;
        split(root, v - 1, x, y); // 分裂成 < v 和 >= v
        int ans = tr[x].size + 1;
        root = merge(x, y);       // 恢复树
        return ans;
    }

    // 查询排名为 k 的数值 (kth)
    int getval(int k) {
        return getval_recursive(root, k);
    }
    
    // 内部递归查询 kth
    int getval_recursive(int p, int k) {
        if (k == tr[tr[p].l].size + 1)
            return tr[p].val;
        else if (k <= tr[tr[p].l].size)
            return getval_recursive(tr[p].l, k);
        else
            return getval_recursive(tr[p].r, k - tr[tr[p].l].size - 1);
    }

    // 查询 v 的前驱 (小于 v 的最大值)
    int getpre(int v) {
        int x, y;
        split(root, v - 1, x, y); // x 树里全是 < v 的
        // 在 x 树里找最大的，即排名为 size 的
        int p = x;
        while (tr[p].r) p = tr[p].r; // 一直往右走找到最大值
        int ans = tr[p].val;
        root = merge(x, y);
        return ans;
    }

    // 查询 v 的后继 (大于 v 的最小值)
    int getnxt(int v) {
        int x, y;
        split(root, v, x, y);     // y 树里全是 > v 的
        // 在 y 树里找最小的，即排名为 1 的
        int p = y;
        while (tr[p].l) p = tr[p].l; // 一直往左走找到最小值
        int ans = tr[p].val;
        root = merge(x, y);
        return ans;
    }
} treap;


int main() {
    // 随机数种子，这对 Treap 的效率至关重要
    srand((unsigned int)time(0));

    int n, m;
    n = read(); 
    m = read();

    // 初始插入 n 个数
    for (int i = 1; i <= n; i++) {
        treap.insert(read());
    }

    int last = 0; // 上一次查询的答案
    int res = 0;  // 最终异或和
    
    // 处理 m 次操作
    for (int i = 1; i <= m; i++) {
        int op = read();
        int v = read();
        v ^= last; // 强制在线解码

        if (op == 1) {
            treap.insert(v);
        } else if (op == 2) {
            treap.del(v);
        } else if (op == 3) {
            last = treap.getrank(v);
            res ^= last;
        } else if (op == 4) {
            last = treap.getval(v);
            res ^= last;
        } else if (op == 5) {
            last = treap.getpre(v);
            res ^= last;
        } else {
            last = treap.getnxt(v);
            res ^= last;
        }
    }

    cout << res << endl;
    return 0;
}
