/** * 最小费用最大流 (Min-Cost Max-Flow)
 * 算法: Primal-Dual (原始对偶算法)
 * 实现: SPFA 初始化势能 + Dijkstra 多路增广
 * * 适用场景: 
 * 1. 边的费用 (cost) 可以为负数。
 * 2. 图中不能存在负权环。
 * V: 节点数量 (Vertices)
 * E: 边数量 (Edges)
 * * 时间复杂度: O(F * E log V) 其中 F 为最大流量
**/

#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

// 模板参数 T: 流量和费用的类型，通常为 int 或 long long
template<class T>
struct MinCostFlow {
    // 内部边结构体
    struct _Edge {
        int to;     // 终点
        T cap;      // 剩余容量 (Residual Capacity)
        T cost;     // 单位流量费用 (Unit Cost)
        _Edge(int to_, T cap_, T cost_) : to(to_), cap(cap_), cost(cost_) {}
    };
    
    int n;                        // 节点数
    std::vector<_Edge> e;         // 边集
    std::vector<std::vector<int>> g; // 邻接表
    std::vector<T> h;             // 势能数组 (Potential)，用于重赋权
    std::vector<T> dis;           // Dijkstra 中的最短路数组
    std::vector<int> pre;         // 记录路径上前驱边的下标 (用于回溯)
    
    MinCostFlow() {}
    MinCostFlow(int n_) {
        init(n_);
    }
    
    void init(int n_) {
        n = n_;
        e.clear();
        g.assign(n, {});
    }
    
    void addEdge(int u, int v, T cap, T cost) {
        g[u].push_back(e.size());
        e.emplace_back(v, cap, cost);     // 正向边
        
        g[v].push_back(e.size());
        e.emplace_back(u, 0, -cost);      // 反向边：容量0，费用取反
    }

    /**
     * SPFA (Shortest Path Faster Algorithm)
     * 作用: 计算初始势能 h。
     * 只需要在最开始运行一次，用于处理图中初始存在的负权边。
     * 如果题目保证初始所有边权非负，这步其实可以用 Dijkstra 或直接设 h=0。
     */
    bool spfa(int s) {
        h.assign(n, std::numeric_limits<T>::max());
        std::vector<bool> in_queue(n, false);
        std::queue<int> que;
        
        h[s] = 0;
        que.push(s);
        in_queue[s] = true;
        
        while (!que.empty()) {
            int u = que.front();
            que.pop();
            in_queue[u] = false;
            
            for (int i : g[u]) {
                int v = e[i].to;
                T cap = e[i].cap;
                T cost = e[i].cost;
                // SPFA 使用原始费用进行松弛
                if (cap > 0 && h[v] > h[u] + cost) {
                    h[v] = h[u] + cost;
                    if (!in_queue[v]) {
                        que.push(v);
                        in_queue[v] = true;
                    }
                }
            }
        }
        // 只要 s 出发能更新到点，就返回 true 
        // (实际上通常不需要判断返回值，除非图不连通)
        return h[s] != std::numeric_limits<T>::max();
    }

    /**
     * Dijkstra
     * 作用: 在残量网络中寻找最短增广路。
     * 技巧: 使用势能 h 进行重赋权 (Reduced Cost)，保证边权非负。
     * 重赋权公式: w'(u,v) = w(u,v) + h[u] - h[v] >= 0
     */
    bool dijkstra(int s, int t) {
        dis.assign(n, std::numeric_limits<T>::max());
        pre.assign(n, -1);
        // {距离, 节点ID}
        std::priority_queue<std::pair<T, int>, std::vector<std::pair<T, int>>, std::greater<std::pair<T, int>>> que;
        
        dis[s] = 0;
        que.emplace(0, s);
        
        while (!que.empty()) {
            T d = que.top().first;
            int u = que.top().second;
            que.pop();
            
            if (dis[u] != d) continue; // 过期数据，跳过
            
            for (int i : g[u]) {
                int v = e[i].to;
                T cap = e[i].cap;
                T cost = e[i].cost;
                // 核心: 判断条件使用重赋权后的代价
                // dis[v] > dis[u] + cost + h[u] - h[v]
                if (cap > 0 && dis[v] > d + h[u] - h[v] + cost) {
                    dis[v] = d + h[u] - h[v] + cost;
                    pre[v] = i; // 记录前驱边
                    que.emplace(dis[v], v);
                }
            }
        }
        // 如果 dis[t] 仍为 INF，说明无法到达汇点，增广结束
        return dis[t] != std::numeric_limits<T>::max();
    }
    
    // 主函数: 计算从 s 到 t 的最小费用最大流
    // 返回 pair: {最大流量, 最小费用}
    std::pair<T, T> flow(int s, int t) {
        T flow = 0;
        T cost = 0;
        
        // 1. 先跑一遍 SPFA 处理可能的负权边，初始化势能 h
        spfa(s); 
        
        // 2. 循环运行 Dijkstra 直到没有增广路
        while (dijkstra(s, t)) {
            // 更新势能 h
            // 新的势能 = 旧势能 + 本次 Dijkstra 计算出的最短路 dis
            // 这保证了下一轮迭代时，重赋权的边权依然非负
            for (int i = 0; i < n; ++i) {
                if (dis[i] != std::numeric_limits<T>::max()) {
                    h[i] += dis[i];
                }
            }
            // 3. 回溯路径，寻找瓶颈容量 (aug)
            T aug = std::numeric_limits<T>::max();
            for (int i = t; i != s; i = e[pre[i] ^ 1].to) {
                aug = std::min(aug, e[pre[i]].cap);
            }
            // 4. 更新残量网络 (正向边减流，反向边加流)
            for (int i = t; i != s; i = e[pre[i] ^ 1].to) {
                e[pre[i]].cap -= aug;
                e[pre[i] ^ 1].cap += aug;
            }
            // 5. 累加流量和费用
            flow += aug;
            // 本次增广的单位费用正是 s 到 t 的势能差 (即真实最短路长度)
            cost += aug * h[t];
        }
        return std::make_pair(flow, cost);
    }
    
    // 辅助结构体: 用于外部获取边的具体信息
    struct Edge {
        int from;
        int to;
        T cap;  // 原始容量
        T cost; // 单价
        T flow; // 实际流量
    };
    
    // 用于输出方案和调试
    std::vector<Edge> edges() {
        std::vector<Edge> a;
        // 遍历所有正向边 (步长为2)
        for (int i = 0; i < e.size(); i += 2) {
            Edge x;
            x.from = e[i + 1].to; // 反向边的终点 = 正向边的起点
            x.to = e[i].to;
            x.cap = e[i].cap + e[i + 1].cap; // 原始容量 = 剩余 + 已流
            x.cost = e[i].cost;
            x.flow = e[i + 1].cap; // 流量 = 反向边的容量
            a.push_back(x);
        }
        return a;
    }
};
