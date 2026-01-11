/** 最大流
 * 基于 Dinic 算法实现
 * 包含了当前弧优化
**/

#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

constexpr int inf = 1E9;

template<class T>
struct MaxFlow {
    //存储目标点和剩余容量
    struct _Edge {
        int to;     // 终点
        T cap;      // 剩余容量 (Residual Capacity)
        _Edge(int to, T cap) : to(to), cap(cap) {}
    };
    
    int n;                          // 节点数量
    std::vector<_Edge> e;           // 存储所有边的列表（正向边和反向边）
    std::vector<std::vector<int>> g; // 邻接表，g[u] 存的是 e 数组的下标
    std::vector<int> cur;           // 当前弧优化数组，记录当前节点遍历到了哪条边
    std::vector<int> h;             // 高度/层级数组 (Level Graph)，记录从源点 s 到该点的距离
    
    MaxFlow() {}
    MaxFlow(int n) {
        init(n);
    }
    
    void init(int n) {
        this->n = n;
        e.clear();
        g.assign(n, {});
        cur.resize(n);
        h.resize(n);
    }
    
    // BFS 构建分层图 (Level Graph)
    // 返回 true 表示汇点 t 在残量网络中仍然可达
    bool bfs(int s, int t) {
        h.assign(n, -1); // 初始化高度为 -1，表示未访问
        std::queue<int> que;
        h[s] = 0;        // 源点高度为 0
        que.push(s);
        while (!que.empty()) {
            const int u = que.front();
            que.pop();
            for (int i : g[u]) {
                auto [v, c] = e[i];
                // 如果还有剩余容量(c > 0) 且 v 未被访问过
                if (c > 0 && h[v] == -1) {
                    h[v] = h[u] + 1; // 更新层级
                    if (v == t) {
                        return true; // 只要能到达汇点，就可以结束 BFS
                    }
                    que.push(v);
                }
            }
        }
        return false; // 汇点不可达，算法结束
    }
    
    // DFS 在分层图中寻找增广路并推送流量
    // u: 当前节点, t: 汇点, f: 当前路径上允许的最大流量
    T dfs(int u, int t, T f) {
        if (u == t) {
            return f; // 到达汇点，返回实际推流大小
        }
        auto r = f; // r 表示当前节点剩余可以推送的流量
        
        // 当前弧优化：从 cur[u] 开始遍历，跳过已经满流的边
        for (int &i = cur[u]; i < int(g[u].size()); ++i) {
            const int j = g[u][i];
            auto [v, c] = e[j];
            
            // 仅在分层图中向下一层走 (h[v] == h[u] + 1) 且有剩余容量时尝试推流
            if (c > 0 && h[v] == h[u] + 1) {
                // 递归寻找路径，推流量为 min(当前剩余 r, 边容量 c)
                auto a = dfs(v, t, std::min(r, c));
                
                // 更新正向边剩余容量
                e[j].cap -= a;
                // 更新反向边剩余容量 (利用异或性质：0^1=1, 1^1=0, 2^3=3...)
                // e[j] 和 e[j^1] 互为反向边
                e[j ^ 1].cap += a;
                
                r -= a; // 减少当前节点剩余可用流量
                
                // 如果 r 为 0，说明当前节点的入流已经全部推完了，无需继续遍历邻居
                if (r == 0) {
                    return f;
                }
            }
        }
        // 返回实际能够推送的流量 (总需求 f - 剩余没推出去的 r)
        return f - r;
    }
    
    // 添加边：有向边 u -> v，容量为 c
    void addEdge(int u, int v, T c) {
        g[u].push_back(e.size()); // 记录正向边下标
        e.emplace_back(v, c);     // 正向边容量 c
        
        g[v].push_back(e.size()); // 记录反向边下标
        e.emplace_back(u, 0);     // 反向边初始容量 0 (用于退流)
    }
    
    // 计算从 s 到 t 的最大流
    T flow(int s, int t) {
        T ans = 0;
        // 当 BFS 还能找到增广路径时
        while (bfs(s, t)) {
            cur.assign(n, 0); // 重置当前弧
            // DFS 多路增广，初始流量设为无穷大
            ans += dfs(s, t, std::numeric_limits<T>::max());
        }
        return ans;
    }
    
    // 获取最小割 (Min-Cut)
    // 跑完最大流后，返回一个布尔数组，
    // true 表示该节点属于源点集 S (即 bfs 能到达的点)
    std::vector<bool> minCut() {
        std::vector<bool> c(n);
        for (int i = 0; i < n; i++) {
            c[i] = (h[i] != -1);
        }
        return c;
    }
    
    // 辅助结构体：用于输出边的状态
    struct Edge {
        int from;
        int to;
        T cap;  // 原始容量
        T flow; // 实际流量
    };
    
    // 返回所有边的状态 用来调试或输出结果
    std::vector<Edge> edges() {
        std::vector<Edge> a;
        // 步长为 2，只遍历正向边
        for (int i = 0; i < e.size(); i += 2) {
            Edge x;
            x.from = e[i + 1].to; // 反向边的终点即为正向边的起点
            x.to = e[i].to;
            x.cap = e[i].cap + e[i + 1].cap; // 原始容量 = 剩余容量 + 反向边容量(即流走的量)
            x.flow = e[i + 1].cap;           // 实际流量 = 反向边的容量
            a.push_back(x);
        }
        return a;
    }
};
