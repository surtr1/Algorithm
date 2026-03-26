#include <bits/stdc++.h>
using namespace std;

class AStar {
public:
    using PII = pair<int, int>;

    struct Node {
        int x, y;
        int g;   // 起点到当前点的实际代价
        int h;   // 当前点到终点的估计代价
        int f;   // f = g + h

        bool operator < (const Node& other) const {
            // priority_queue 默认大根堆，这里反过来实现小根堆
            return f > other.f;
        }
    };

    // 4方向
    const vector<int> dx = {-1, 1, 0, 0};
    const vector<int> dy = {0, 0, -1, 1};

    int n, m;
    vector<vector<int>> grid;          // 0:可走, 1:障碍
    vector<vector<int>> gScore;        // 起点到每个点的最小代价
    vector<vector<bool>> closed;       // 是否已确定最优
    vector<vector<PII>> parent;        // 路径还原

    AStar(const vector<vector<int>>& mp) {
        grid = mp;
        n = grid.size();
        m = grid[0].size();
        gScore.assign(n, vector<int>(m, INT_MAX));
        closed.assign(n, vector<bool>(m, false));
        parent.assign(n, vector<PII>(m, {-1, -1}));
    }

    // 曼哈顿距离：适合4方向
    int heuristic(int x1, int y1, int x2, int y2) {
        return abs(x1 - x2) + abs(y1 - y2);
    }

    bool inBound(int x, int y) {
        return x >= 0 && x < n && y >= 0 && y < m;
    }

    vector<PII> reconstructPath(PII start, PII goal) {
        vector<PII> path;
        PII cur = goal;
        while (cur != make_pair(-1, -1)) {
            path.push_back(cur);
            if (cur == start) break;
            cur = parent[cur.first][cur.second];
        }
        reverse(path.begin(), path.end());

        if (path.empty() || path[0] != start) return {};
        return path;
    }

    vector<PII> search(PII start, PII goal) {
        // 起点/终点合法性判断
        if (!inBound(start.first, start.second) || !inBound(goal.first, goal.second)) return {};
        if (grid[start.first][start.second] == 1 || grid[goal.first][goal.second] == 1) return {};

        priority_queue<Node> pq;
        gScore[start.first][start.second] = 0;
        int h = heuristic(start.first, start.second, goal.first, goal.second);
        pq.push({start.first, start.second, 0, h, h});

        while (!pq.empty()) {
            Node cur = pq.top();
            pq.pop();

            int x = cur.x, y = cur.y;

            if (closed[x][y]) continue;
            closed[x][y] = true;

            if (make_pair(x, y) == goal) {
                return reconstructPath(start, goal);
            }

            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (!inBound(nx, ny)) continue;
                if (grid[nx][ny] == 1) continue;
                if (closed[nx][ny]) continue;

                int tentative_g = gScore[x][y] + 1; // 每走一步代价为1

                if (tentative_g < gScore[nx][ny]) {
                    gScore[nx][ny] = tentative_g;
                    parent[nx][ny] = {x, y};

                    int nh = heuristic(nx, ny, goal.first, goal.second);
                    pq.push({nx, ny, tentative_g, nh, tentative_g + nh});
                }
            }
        }

        return {}; // 无法到达
    }
};

//举例
int main() {
    vector<vector<int>> grid = {
        {0, 0, 0, 0, 0},
        {1, 1, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}
    };

    AStar astar(grid);

    pair<int, int> start = {0, 0};
    pair<int, int> goal  = {4, 4};

    vector<pair<int, int>> path = astar.search(start, goal);

    if (path.empty()) {
        cout << "No path found\n";
    } else {
        cout << "Path found:\n";
        for (auto [x, y] : path) {
            cout << "(" << x << ", " << y << ")\n";
        }
    }

    return 0;
}