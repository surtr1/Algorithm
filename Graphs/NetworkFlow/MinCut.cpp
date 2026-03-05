/**
 * 基础模型：构筑二分图，左半部 n 个点代表盈利项目，右半部 m 个点代表材料成本，收益为盈利之
 * 和减去成本之和，求最大收益。
 * 建图：建立源点 S 向左半部连边，建立汇点 T 向右半部连边，如果某个项目需要某个材料，则新增
 * 一条容量 正无穷 的跨部边。
 * 割边：放弃某个项目则断开 S 至该项目的边，购买某个原料则断开该原料至 T 的边，最终的图一定
 * 不存在从 S 到 T 的路径，此时我们得到二分图的一个 S - T 割。此时最小割即为求解最大流，边权之和
 * 减去最大流即为最大收益。
 */
#include <iostream>
using namespace std;
template<class T>
struct MaxFlow {
    MaxFlow(int n);
    void add(int x,int y, int c);
    T work(int st, int ed);
    // 详细实现见MaxFlow
};

signed main() {
    int n, m;
    cin >> n >> m;
    int S = n + m + 1, T = n + m + 2;
    MaxFlow<long long> flow(T);
    for (int i = 1; i <= n; i++) {
        int w;
        cin >> w;
        flow.add(S, i, w);
    }
    int sum = 0;
    for (int i = 1; i <= m; i++) {
        int x, y, w;
        cin >> x >> y >> w;
        flow.add(x, n + i, 1E18);
        flow.add(y, n + i, 1E18);
        flow.add(n + i, T, w);
        sum += w;
    }
    cout << sum - flow.work(S, T) << endl;
}