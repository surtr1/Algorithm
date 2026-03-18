
//平面最近点对
//O(n log^2(n))
// 用lambda表达式, 时间上快了一些
auto cmpy = [](Point A, Point B)->bool { 
    return sgn(A.y - B.y) < 0; 
};
auto cmpxy = [](Point A, Point B)->bool { 
    return sgn(A.x - B.x) < 0 || (sgn(A.x - B.x) == 0 && sgn(A.y - B.y) < 0); 
};

Point p[N], tmp_p[N];
db closetpair(int l, int r) {  // 分治前先排序
    db dis = INF;
    if (l == r) return dis; 		// 只剩一个点  
    if (l + 1 == r) return Dist(p[l], p[r]); // 只剩两个点
    int mid = (l + r) >> 1;         // 分治
    db d1 = closetpair(l, mid);   // 求 S1 内的最近点对
    db d2 = closetpair(mid+1, r); // 求 S2 内的最近点对
    dis = min(d1, d2);
    int k = 0;
    for (int i = l; i <= r; i ++)      // 在 S1 和 S2 中间附近找可能的最小点对
        if (fabs(p[mid].x - p[i].x) <= dis)   // 按 x 坐标来找
            tmp_p[k ++] = p[i];
    sort (tmp_p, tmp_p + k, cmpy);    // 按 y 坐标排序, 用于剪枝. 这里不能按 x 坐标排序
    for (int i = 0; i < k; i ++) 
        for (int j = i + 1; j < k ;j ++) {
            if (tmp_p[j].y - tmp_p[i].y >= dis) break;  // 剪枝
            dis = min(dis, Dist(tmp_p[i], tmp_p[j]));
        }
    return dis; // 返回最小值
}


//凸包的直径 ： 平面最远点对 O(N)
// RC: rotatingcalipers 旋转卡壳
db rotatingcalipers(Point* p, int n) {
    db res = 0;
    if (n == 2) return Dist(p[0], p[1]);
    p[n] = p[0];
    for (int i = 0, r = 2; i < n; i ++) {
        while (sgn (Cross (p[i], p[i+1], p[r]) - Cross (p[i], p[i+1], p[r+1])) < 0) 
            if (++ r >= n) r -= n;
        res = max(res, max(Dist(p[r], p[i]), Dist(p[r], p[i+1])));
    }
    return res;
}

//凸包的宽度 O(N)
// 算法和求两凸包间的最近距离类似, 只需做一些小的改变
db rotatingcalipers2(Point* p, int n) {
    db res = 2e18;
    if (n == 2) return 0;
    p[n] = p[0];
    for (int i = 0, r = 2; i < n; i ++) {
        while (sgn (Cross (p[i], p[i+1], p[r]) - Cross (p[i], p[i+1], p[r+1])) < 0) 
            if (++ r >= n) r -= n;   // 寻找对踵点
        res = min(res, disptl(p[i], p[i+1], p[r])); // 改成点到直线的距离
    }
    return res;
}

//两凸包的最近距离 O(N)
// 由于两凸包的位置关系不确定, 我们要交换 A 和 B 再求一遍最近距离, 两次取最小
db rotatingcalipers(Point* A, int n, Point* B, int m) {
    int u = 0, v = 0;
    db tmp, res = 1e18;
    A[n] = A[0], B[m] = B[0];
    for (int i = 1; i < n; i ++)
        if (sgn(A[i].y - A[u].y) < 0) u = i; // 寻找 A 最低点
    for (int i = 1; i < m; i ++) 
        if (sgn(B[i].y - B[v].y) > 0) v = i; // 寻找 B 最高点
    for (int i = 0; i < n; i ++) {
        while (sgn (tmp = Cross(A[u], A[u+1], B[v]) - Cross(A[u], A[u+1], B[v+1])) < 0) 
            if (++ v >= m) v -= m;
        if (sgn (tmp) == 0) res = min(res, dissts(A[u], A[u+1], B[v], B[v+1]));
        else res = min(res, dispts(A[u], A[u+1], B[v]));
        if (++ u >= n) u -= n;
    }
    return res;
}
