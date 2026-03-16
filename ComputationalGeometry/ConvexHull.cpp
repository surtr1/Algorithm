// Andrew 算法
// 坐标排序 O(nlogn), 测试: 洛谷P2742；凸包稳定相关: POJ1228
// 这里有两种排序方法
// 1. 优先按 y 排序, 如果 y 相同, 按 x 排序, 起点是最下面的点, 如下面代码体现
// 2. 优先按 x 排序, 如果 x 相同, 按 y 排序, 起点是最左边的点
bool operator < (Point a, Point b) { 
	if (sgn(a.y - b.y) == 0) return a.x < b.x;
    return a.y < b.y;
}
int andrew(Point *p, Point *ch, int n) {  // 返回值是凸包的顶点数
    sort (p, p + n);   // 排序
    n = unique(p, p + n) - p; // 去除重复的点
    int v = 0;
    // 求下凸包, 如果 p[i] 是右拐弯的, 则这个点不在凸包中, 往回退
    for (int i = 0; i < n; i ++) {
        while (v > 1 && sgn(Cross(ch[v-2], ch[v-1], p[i])) <= 0) v --;
        ch[v ++] = p[i];
    }
    int j = v;
    // 求上凸包
    for (int i = n - 2; i >= 0; i --) {
        while (v > j && sgn(Cross(ch[v-2], ch[v-1], p[i])) <= 0) v --;
        ch[v ++] = p[i];
    }
    if (n > 1) v --;
    return v;
}


//Graham 算法
//极角排序O(nlogn), 测试: 洛谷P2742
// 1. 选择一个最左下的点作为基准点, 那么这点一定在凸包内。
// 2. 接着以这个基准点为基准, 将剩余的点按极角从小到大排序, 可以通过叉积实现。
// 3. 若极角相同, 则按照到基准点距离从小到大排序。
Point base; // 全局变量
bool cmp(Point a, Point b) {
    int c = sgn(Cross(base, a, b));
    if (c == 0) 
        return Dist(base, a) < Dist(base, b);
    return c < 0;
}
int graham(Point* p, Point* ch, int n) {
    base = p[0]; // 基准点不参与排序, 一般我们放在 p[0]
    sort (p + 1, p + n, cmp);
    int v = 0;
    ch[v ++] = p[0]; // 基准点一定参与构成凸包
    for (int i = 1; i < n; i ++) {
        while (v > 1 && sgn(Cross(ch[v-2], ch[v-1], p[i])) <= 0) v --;
        ch[v ++] = p[i];
    }
    return v;
}

// 是否构成凸包
// 顺逆时针都可以判断
// 判断是否是凸包, 方向不一定是逆时针
bool isconvex(Point* p, int n){
    int dir = 0;
    p[n] = p[0], p[n+1] = p[1];
    for(int i = 1; i <= n; i ++) {
        int u = sgn(Cross(p[i] - p[i-1], p[i+1] - p[i]));
        if(!dir) dir = u;
        if(u * dir < 0) return false;
    }
    return true;
}

// 将凸包变为逆时针
void change(Point* p, int n) {
    p[n] = p[0];
    for (int i = 0; i < n - 1; i ++) {
        int c = sgn (Cross (p[i], p[i+1], p[i+2]));
        if (c > 0) return ;  // 方向正确
        if (c < 0) {         // 方向错误, 将整个凸包反转
            for (int j = 0, k = n - 1; j < k; j ++, k --) 
                swap(p[j], p[k]);
            return ;
        }
    }
}

// 直线 u 与凸包左侧形成新凸包
// 单次时间复杂度 $O(n)$​​ , 测试: HDU3982
// 类似半平面交, 其实也可以用来求半平面交, 复杂度 O(n^2)
int convexcut(Line u, Point* p, int n) {
    int v = 0;
    p[n] = p[0];
    static Point ch[N];
    for (int i = 0; i < n; i ++) {
        int d1 = sgn(Cross(u.s, u.e, p[i]));
        int d2 = sgn(Cross(u.s, u.e, p[i+1]));
        if (d1 >= 0) ch[v ++] = p[i]; // 在左侧则压入点
        // 异号说明他穿过这条直线
        if (d1*d2 < 0) ch[v ++] = pcrossll(u, Line(p[i], p[i+1]));   
    }// 拷贝答案
    for (int i = 0; i < v; i ++) p[i] = ch[i]; 
    return v;
}

//点和凸包的关系
//2: 里边, 1: 上边, 0: 外边
int relation(Point* p, int n, Point pt) {
    // l 取 1 的原因是要以p[0]作为基准点, r 取 n-2 的原因是下面要进行mid+1
    int l = 1, r = n - 2; 
    while (l <= r) {
        int mid = (l + r) >> 1;
        int d1 = sgn(Cross(p[0], p[mid], pt));
        int d2 = sgn(Cross(p[0], p[mid+1], pt));
        if (d1 >= 0 && d2 <= 0) { // 找到 pt 所在的三角区域
            // 如果在左侧或在边上, 说明在内部
            int c = sgn(Cross(p[mid], p[mid+1], pt));
            if (c > 0) return 2;
            else if (c == 0) return 1;
            else return 0; // 否则不在
        }
        else if (d1 < 0) r = mid - 1; // 在右边
        else l = mid + 1; // 在左边
    }
    return 0;
}

//过凸包外一点求凸包的切线 时间复杂度O(logn)
// 二分查找斜率最值对应的点, w 等于 1 表示找最小值, w 等于 -1 表示找最大值
// l 到 r 为查找区间, pt 为凸包外一点
int minslope(Point* p, int l, int r, Point pt, int w) {
    while (l < r) {
        int mid = (l + r) >> 1;
        if (sgn(Cross(pt, p[mid], p[mid+1])) * w >= 0) r = mid;
        else l = mid + 1;
    }
    return l;
}
// w = 1:  二分查找第一个大于等于 x 对应的点
// w = -1: 二分查找第一个小于等于 x 对应的点
int border(Point* p, int l, int r, db x, int w) {
    while (l < r) {
        int mid = (l + r) >> 1;
        if (sgn(x - p[mid].x) * w <= 0) r = mid;
        else l = mid + 1;
    }
    return l;
}

// 使用该函数之前要判断点是否在凸包外
pair<int, int> pptangentpcon(Point* p, int n, Point pt, int Rx) {
    int L, R, Mid;
    if (sgn(pt.x - p[0].x) < 0) {  // 情况一: 点在凸包的左边
        L = minslope(p, 0, Rx, pt, 1), R = minslope(p, Rx, n, pt, -1);
    }
    else if (sgn(pt.x - p[Rx].x) > 0) {  // 情况二: 点在凸包的右边
        L = minslope(p, 0, Rx, pt, -1), R = minslope(p, Rx, n, pt, 1);
    }
    else if (Cross(pt, p[0], p[Rx]) > 0) { // 情况三: 点在凸包的上方
        Mid = border(p, Rx, n, pt.x, -1);
        L = Mid > Rx ? minslope(p, Rx, Mid-1, pt, -1) : Mid;
        R = minslope(p, Mid, n, pt, 1);
    }
    else { 								// 情况四: 点在凸包的下方
        Mid = border(p, 0, Rx, pt.x, 1);
        L = Mid > 0 ? minslope(p, 0, Mid-1, pt, -1) : 0;
        R = minslope(p, Mid, Rx, pt, 1);
    }
    return make_pair(L, R); // 返回找到的两个切点
}
