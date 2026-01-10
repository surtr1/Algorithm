#include <vector>
#include <algorithm>

/**
 * 树链剖分 (HLD) 
 */
struct HLD {
    int n;
    // siz: 子树大小
    // top: 当前节点所在重链的链顶节点
    // dep: 深度
    // parent: 父节点
    // in, out: DFS 序的进入和离开时间戳（用于判断子树/祖先关系）
    // seq: DFS 序对应的节点编号 (seq[in[u]] == u)
    std::vector<int> siz, top, dep, parent, in, out, seq;
    std::vector<std::vector<int>> adj;
    int cur;
    
    HLD() {}
    HLD(int n) {
        init(n);
    }
    void init(int n) {
        this->n = n;
        siz.resize(n);
        top.resize(n);
        dep.resize(n);
        parent.resize(n);
        in.resize(n);
        out.resize(n);
        seq.resize(n);
        cur = 0;
        adj.assign(n, {});
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    void work(int root = 0) {
        top[root] = root;
        dep[root] = 0;
        parent[root] = -1;
        dfs1(root);
        dfs2(root);
    }
    
    // 第一次 DFS：处理子树大小、深度、父节点，并重新组织邻接表
    void dfs1(int u) {
        // 从邻接表中移除父节点，将无向图逻辑转为有向树逻辑
        // 这样后续遍历就不需要判断 if (v != parent[u]) 了
        if (parent[u] != -1) {
            adj[u].erase(std::find(adj[u].begin(), adj[u].end(), parent[u]));
        }
        
        siz[u] = 1;
        for (auto &v : adj[u]) {
            parent[v] = u;
            dep[v] = dep[u] + 1;
            dfs1(v);
            siz[u] += siz[v];
            if (siz[v] > siz[adj[u][0]]) {
                std::swap(v, adj[u][0]);
            }
        }
    }
    
    // 第二次 DFS：构建 DFS 序、记录链顶、连接重链
    void dfs2(int u) {
        in[u] = cur++;
        seq[in[u]] = u; // 记录 DFS 序 id 对应的节点
        for (auto v : adj[u]) {
            // 如果 v 是重儿子 (adj[u][0])，延续当前重链，top 不变
            // 否则 v 是轻儿子，开启一条新链，top 为 v 自己
            top[v] = v == adj[u][0] ? top[u] : v;
            dfs2(v);
        }
        out[u] = cur;
    }
    
    // 标准 LCA 查询
    int lca(int u, int v) {
        // 当两个点不在同一条重链上时
        while (top[u] != top[v]) {
            // 让链顶深度较大的那个点跳到其链顶的上方
            if (dep[top[u]] > dep[top[v]]) {
                u = parent[top[u]];
            } else {
                v = parent[top[v]];
            }
        }
        // 在同一条重链上，深度小的即为 LCA
        return dep[u] < dep[v] ? u : v;
    }
    
    // 计算两点距离
    int dist(int u, int v) {
        return dep[u] + dep[v] - 2 * dep[lca(u, v)];
    }
    
    // 求 u 向上第 k 个祖先 (K-th Ancestor)
    int jump(int u, int k) {
        if (dep[u] < k) {
            return -1; // 越过根节点
        }
        
        int d = dep[u] - k; // 目标深度
        
        // 当 u 所在链顶深度大于目标深度时，直接跳过整条链
        while (dep[top[u]] > d) {
            u = parent[top[u]];
        }
        
        // 此时 u 和目标节点在同一条重链上
        // 利用 DFS 序的连续性，通过数组下标直接计算出目标节点
        // seq[in[u] - (dep[u] - d)]
        return seq[in[u] - dep[u] + d];
    }
    
    // 判断 u 是否是 v 的祖先
    bool isAncester(int u, int v) {
        return in[u] <= in[v] && in[v] < out[u];
    }
    
    // 【换根查询核心】
    // 假设以 new_root 为根，求 target 的父节点
   int rootedParent(int new_root, int target) {
        // 1. 如果目标就是新根，那它没有父节点（或者由外部定义返回自己）
        if (new_root == target) {
            return target;
        }
        // 2. 如果 target 不是 new_root 的祖先
        if (!isAncester(target, new_root)) {
            return parent[target];
        }

        // 3. 如果 target 是 new_root 的祖先 (target -> ... -> new_root)
        // 说明 target 在 new_root 的"头顶"上。
        // 换根后，关系反转：target 通向 new_root 方向的那个直系子节点，变成了 target 的新父亲。
        
        // 我们需要在 target 的邻接表 (adj[target]) 中，找到包含 new_root 的那个子节点。
        // 利用 DFS 序进行二分查找：
        auto it = std::upper_bound(adj[target].begin(), adj[target].end(), new_root, 
            [&](int val, int node_in_adj) {
                return in[val] < in[node_in_adj];
            });
        
        // upper_bound 找到的是第一个 in 值 > in[new_root] 的节点
        // 它的前一个节点，就是包含 new_root 的那个子节点
        return *(it - 1);
    }
    /**
     * 换根后的子树大小查询
     **/
    int rootedSize(int new_root, int target) {
        if (new_root == target) {
            return n;
        }
        
        // 如果 target 不是 new_root 的祖先，换根不影响 target 的子树结构
        if (!isAncester(target, new_root)) {
            return siz[target];
        }
        
        // 如果 target 是 new_root 的祖先，换根后：
        // target 的子树 = 整棵树 - (target 通向 new_root 的那个分支)
        // 那个分支的根，正是 target 在换根后的"父节点"
        return n - siz[rootedParent(new_root, target)];
    }
    
    // 【换根 LCA】
    // 假设以 c 为根，求 a 和 b 的 LCA
    int rootedLca(int a, int b, int c) {
        // 三个点两两求 LCA，得到三个结果。
        // 其中肯定有两个是重合的（较浅的那个），剩下一个是独特的（较深的那个）。
        // 那个“独特的”点就是换根后的 LCA。
        // 因为 x^x = 0, 0^y = y，所以直接异或三个 LCA 的 ID 就能得到那个独特的 ID。
        return lca(a, b) ^ lca(b, c) ^ lca(c, a);
    }
};
