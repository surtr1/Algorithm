#include <bits/stdc++.h>
using namespace std;

class BranchStar {
public:
    struct Point {
        int x, y;
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };

    struct Node {
        int x, y;
        int g;          // 已走步数
        int parent;     // 父节点编号
        int lastDir;    // 上一步方向，-1 表示起点
    };

    struct State {
        int id;         // 对应 nodes[id]
        int f;          // g + h
        int tie;        // 平局时的优先级（更偏向直行/靠前分支）
        bool operator<(const State& other) const {
            if (f != other.f) return f > other.f;       // 小根堆效果
            return tie > other.tie;
        }
    };

    // 4方向：上右下左
    const int dx[4] = {-1, 0, 1, 0};
    const int dy[4] = {0, 1, 0, -1};

    int n, m;
    vector<vector<int>> grid; // 0=可走, 1=障碍
    vector<vector<int>> bestG;
    vector<Node> nodes;

    BranchStar(const vector<vector<int>>& mp) {
        grid = mp;
        n = (int)grid.size();
        m = (int)grid[0].size();
        bestG.assign(n, vector<int>(m, INT_MAX));
    }

    bool inBounds(int x, int y) const {
        return x >= 0 && x < n && y >= 0 && y < m;
    }

    bool passable(int x, int y) const {
        return inBounds(x, y) && grid[x][y] == 0;
    }

    int heuristic(int x, int y, const Point& goal) const {
        // 4方向常用：曼哈顿距离
        return abs(x - goal.x) + abs(y - goal.y);
    }

    int leftOf(int dir) const {
        return (dir + 3) % 4;
    }

    int rightOf(int dir) const {
        return (dir + 1) % 4;
    }

    int opposite(int dir) const {
        return (dir + 2) % 4;
    }

    // 判断 (x, y) 是否已经在当前分支路径上，防止绕圈
    bool onCurrentPath(int parentId, int x, int y) const {
        while (parentId != -1) {
            if (nodes[parentId].x == x && nodes[parentId].y == y) return true;
            parentId = nodes[parentId].parent;
        }
        return false;
    }

    // 根据“朝目标直冲；受阻则左右绕障”的思想，生成方向优先级
    vector<int> orderedDirs(const Node& cur, const Point& goal) const {
        int curH = heuristic(cur.x, cur.y, goal);

        // 1) 先找能让 h 下降的“前进方向”
        vector<pair<pair<int, int>, int>> greedy; 
        // {{新h, 转向代价}, dir}

        for (int d = 0; d < 4; ++d) {
            int nx = cur.x + dx[d];
            int ny = cur.y + dy[d];
            if (!passable(nx, ny)) continue;

            int nh = heuristic(nx, ny, goal);
            if (nh < curH) {
                int turnCost = (cur.lastDir == -1 || cur.lastDir == d) ? 0 : 1;
                greedy.push_back({{nh, turnCost}, d});
            }
        }

        if (!greedy.empty()) {
            sort(greedy.begin(), greedy.end());
            vector<int> dirs;
            for (auto& e : greedy) dirs.push_back(e.second);
            return dirs;
        }

        // 2) 如果直冲不行，开始“分支绕障”
        //    先确定一个主方向（更偏向目标的主轴）
        int ddx = goal.x - cur.x;
        int ddy = goal.y - cur.y;

        int primary = -1, secondary = -1;

        if (abs(ddx) >= abs(ddy)) {
            if (ddx > 0) primary = 2;      // 下
            else if (ddx < 0) primary = 0; // 上

            if (ddy > 0) secondary = 1;      // 右
            else if (ddy < 0) secondary = 3; // 左
        } else {
            if (ddy > 0) primary = 1;      // 右
            else if (ddy < 0) primary = 3; // 左

            if (ddx > 0) secondary = 2;      // 下
            else if (ddx < 0) secondary = 0; // 上
        }

        vector<int> cand;

        // Branch Star 风格：主方向受阻时，优先左右两个分支绕过去
        if (primary != -1) {
            cand.push_back(leftOf(primary));
            cand.push_back(rightOf(primary));
        }

        // 次方向也可以作为补充
        if (secondary != -1) cand.push_back(secondary);

        // 再考虑其他方向，避免卡死
        if (primary != -1) cand.push_back(opposite(primary));

        for (int d = 0; d < 4; ++d) cand.push_back(d);

        vector<int> dirs;
        vector<int> used(4, 0);
        for (int d : cand) {
            if (d < 0 || d >= 4 || used[d]) continue;
            int nx = cur.x + dx[d];
            int ny = cur.y + dy[d];
            if (!passable(nx, ny)) continue;
            used[d] = 1;
            dirs.push_back(d);
        }
        return dirs;
    }

    vector<Point> reconstructPath(int goalId) const {
        vector<Point> path;
        int cur = goalId;
        while (cur != -1) {
            path.push_back({nodes[cur].x, nodes[cur].y});
            cur = nodes[cur].parent;
        }
        reverse(path.begin(), path.end());
        return path;
    }

    // 返回一条可行路径；找不到返回空数组
    vector<Point> search(Point start, Point goal) {
        if (!passable(start.x, start.y) || !passable(goal.x, goal.y)) return {};

        priority_queue<State> pq;
        nodes.clear();
        for (auto& row : bestG) fill(row.begin(), row.end(), INT_MAX);

        nodes.push_back({start.x, start.y, 0, -1, -1});
        bestG[start.x][start.y] = 0;
        pq.push({0, heuristic(start.x, start.y, goal), 0});

        while (!pq.empty()) {
            State top = pq.top();
            pq.pop();

            Node cur = nodes[top.id];

            // 跳过过期状态
            if (cur.g != bestG[cur.x][cur.y]) continue;

            if (cur.x == goal.x && cur.y == goal.y) {
                return reconstructPath(top.id);
            }

            vector<int> dirs = orderedDirs(cur, goal);

            for (int rank = 0; rank < (int)dirs.size(); ++rank) {
                int d = dirs[rank];
                int nx = cur.x + dx[d];
                int ny = cur.y + dy[d];

                if (!passable(nx, ny)) continue;
                if (onCurrentPath(top.id, nx, ny)) continue; // 防止本分支成环

                int ng = cur.g + 1;

                // 剪枝：只保留更优到达
                if (ng >= bestG[nx][ny]) continue;
                bestG[nx][ny] = ng;

                int nid = (int)nodes.size();
                nodes.push_back({nx, ny, ng, top.id, d});

                int h = heuristic(nx, ny, goal);

                // tie 越小越优先：
                // 1) 更倾向不转弯
                // 2) 更倾向 orderedDirs 里排前面的方向
                int turnCost = (cur.lastDir == -1 || cur.lastDir == d) ? 0 : 1;
                int tie = turnCost * 10 + rank;

                pq.push({nid, ng + h, tie});
            }
        }

        return {};
    }
};

int main() {
    vector<vector<int>> grid = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0, 1, 0},
        {0, 0, 0, 1, 0, 1, 0},
        {1, 1, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0},
        {0, 1, 1, 0, 0, 0, 0}
    };

    BranchStar solver(grid);

    BranchStar::Point start{0, 0};
    BranchStar::Point goal{5, 6};

    auto path = solver.search(start, goal);

    if (path.empty()) {
        cout << "No path found\n";
    } else {
        cout << "Path length = " << (int)path.size() - 1 << "\n";
        for (auto& p : path) {
            cout << "(" << p.x << ", " << p.y << ")\n";
        }
    }

    return 0;
}