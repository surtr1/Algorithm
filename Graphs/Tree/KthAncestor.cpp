/**
 * 长链剖分解决 k 级祖先
 * O(n * logn) 预处理
 * O(1) 查询某节点的 k 级祖先是什么
**/



#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

struct KthAncestor {
    int n;
    int max_log;
    vector<vector<int>> adj;
    
    // 算法相关数组
    vector<vector<int>> parent; // 对应 f: 倍增数组
    vector<int> depth;          // 对应 d: 当前节点深度
    vector<int> max_depth;      // 对应 dep:以此节点为根的子树最大深度
    vector<int> son;            // 对应 son: 长儿子（重儿子）
    vector<int> top;            // 对应 top: 长链顶端
    vector<int> lg;             // 对应 g: log2数组
    
    // 长链剖分所需的阶梯数组
    // up[x] 存储 x 所在长链顶端向上跳 len 步的节点
    // down[x] 存储 x 所在长链顶端向下跳 len 步的节点
    vector<vector<int>> up;     // 对应 u
    vector<vector<int>> down;   // 对应 v

    KthAncestor(int _n) : n(_n) {
        // 初始化 log 数组，用于快速定位 highbit
        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
        
        max_log = lg[n];
        
        // 分配空间
        adj.resize(n + 1);
        depth.resize(n + 1);
        max_depth.resize(n + 1);
        son.assign(n + 1, 0);
        top.assign(n + 1, 0);
        up.resize(n + 1);
        down.resize(n + 1);
        
        // 倍增数组初始化
        parent.assign(n + 1, vector<int>(max_log + 2, 0));
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    // 第一次 DFS: 处理深度、倍增表、长儿子
    void dfs1(int x, int p) {
        depth[x] = max_depth[x] = depth[p] + 1;
        parent[x][0] = p;
        
        // 倍增预处理
        for (int i = 0; parent[x][i]; ++i) {
            parent[x][i + 1] = parent[parent[x][i]][i];
        }

        for (int y : adj[x]) {
            if (y == p) continue; // 针对无向图的保护
            
            dfs1(y, x);
            
            // 更新长儿子（基于树高，而非子树大小）
            if (max_depth[y] > max_depth[x]) {
                max_depth[x] = max_depth[y];
                son[x] = y;
            }
        }
    }

    // 第二次 DFS: 建立长链，填充 ladder 数组
    void dfs2(int x, int tp) {
        top[x] = tp;
        
        // 如果 x 是链头，预处理 ladder 数组
        if (x == tp) {
            int len = max_depth[x] - depth[x];
            // 向上存 len 长度
            for (int i = 0, o = x; i <= len && o; ++i) {
                up[x].push_back(o);
                o = parent[o][0];
            }
            // 向下存 len 长度
            for (int i = 0, o = x; i <= len && o; ++i) {
                down[x].push_back(o);
                o = son[o];
            }
        }

        if (son[x]) dfs2(son[x], tp); // 优先遍历长儿子，延续链
        for (int y : adj[x]) {
            if (y != son[x] && y != parent[x][0]) {
                dfs2(y, y); // 轻儿子开启新链
            }
        }
    }

    void build(int root = 1) {
        // 这里假设 0 是虚根，root 的父亲是 0
        depth[0] = 0; 
        dfs1(root, 0);
        dfs2(root, root);
    }

    // O(1) 查询 k-th 祖先
    int query(int x, int k) {
        if (k == 0) return x;
        if (k >= depth[x]) return 0; // 超出根节点，视情况返回0或root
        
        // 1. 利用倍增跳过 2^highbit(k) 步
        // 此时剩余步数 k' < 2^highbit(k) <= 链长
        int r = lg[k];
        x = parent[x][r];
        k -= (1 << r);
        
        // 2. 利用长链性质进行 O(1) 定位
        // 计算在链顶相对位置的偏移量
        // 如果 k >= 0，说明目标在链顶或链顶上方（Up数组）
        // 如果 k < 0，说明目标在链顶下方（Down数组），利用负负得正索引
        k -= depth[x] - depth[top[x]];
        
        return k >= 0 ? up[top[x]][k] : down[top[x]][-k];
    }
};
