//半平面交
struct Plane {
    Point p; /// 平面上的一点
    Vector v; /// 方向向量
    db ang; /// 从 x 轴转向 v 方向的夹角
    Plane(Point p=o, Vector v=Vector(1,0)): p(p), v(v) { ang = atan2(v.y, v.x); }
    void calangle() { ang = atan2(v.y, v.x); }
    bool operator < (Plane r) { return sgn(ang - r.ang) > 0; }
};
/// 点 p 在线 L 的左边, 即点 p 在 L 的外边
bool onleft(Plane L, Point p) { return sgn(Cross(L.v, p - L.p)) > 0; }
/// 判断点是否在直线上
bool online(Plane L, Point pt) { return sgn(Cross(pt - L.p, L.v)) == 0; }
/// 判断点是否不在直线右边
bool notonright(Plane L, Point pt) { return sgn(Cross(L.v, pt - L.p)) >= 0;  }
/// 两直线交点
Point pcrossll(Plane a, Plane b) {
    db t = Cross(b.v, a.p - b.p)/Cross(a.v, b.v);
    return a.p + a.v * t;
}

/// 半平面交, 返回的凸包在 ch 数组
int HPI(Plane* L, Point *ch, int n) {
    sort(L, L + n); /// 极角排序
    int h = 0, t = 0, v = 0;
    static Plane q[N]; /// 双端队列
    static Point p[N]; /// 两个相邻半平面的交点
    q[h] = L[0];
    for(int i = 1; i < n; i ++) {
        /// 删除队尾的半平面
        while(h < t && ! onleft(L[i], p[t-1])) t --;
        /// 删除队首的半平面
        while(h < t && ! onleft(L[i], p[h])) h ++;
        q[++ t] = L[i]; /// 将当前的半平面加入双端队列的尾部
        /// 极角相同的两个半平面保留左边
        if(sgn(Cross(q[t].v, q[t-1].v)) == 0) {
            t --;
            if(onleft(q[t], L[i].p)) q[t] = L[i];
        }
        /// 计算队列尾部的半平面交点
        if(h < t) p[t - 1] = pcrossll(q[t-1], q[t]);
    }
    /// 删除队列尾部无用的半平面
    while(h < t && ! onleft(q[h], p[t-1])) t --;
    if(t - h <= 1) return 0; /// 空集
    p[t] = pcrossll(q[t], q[h]); /// 计算队列首尾部的交点
    for(int i = h; i <= t; i ++) ch[v ++] = p[i]; /// 复制
    return v; /// 返回凸多边形大小
}

//判断是否存在核
bool HPI_Core(Plane* L, int n){
    sort(L, L + n);
    int h = 0, t = 0;
    static Plane q[N];
    static Point p[N];
    q[0] = L[0];
    for(int i = 1; i < n; i ++) {
        while(h < t && ! onleft(L[i], p[t-1])) t --;
        while(h < t && ! onleft(L[i], p[h])) h ++;
        q[++ t] = L[i];
        if(sgn(Cross(q[t].v, q[t-1].v)) == 0) {
            t --;
            if(onleft(q[t], L[i].p)) q[t] = L[i];
        }
        if(h < t) p[t-1] = pcrossll(q[t], q[t-1]);
    }
    while(h < t && ! onleft(q[h], p[t-1])) t --;
    return t - h + 1 > 2;
}
