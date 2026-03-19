// 判断点在多边形内部 1, 测试: POJ1584
// 1. Point -> == = Fun()
int relation(Point pt, Point *p, int n) {
    p[n] = p[0];
    for (int i = 0; i < n; i ++) 
        if (pt == p[i]) // == = [](pt, p[i]){return sgn(pt.x - p[i].x) == 0 && sgn(pt.y - p[i].y) == 0;}
            return 3; 	// 3: 点在多边形的顶点上
    for (int i = 0; i < n; i ++) 
        if (segrelation(pt, Line(p[i], p[i+1]))) return 2;   // 2: 点在多边形的边上 
    int num = 0;
    for (int i = 0; i < n; i ++) {
        int j = i + 1;
        int c = sgn(Cross(pt-p[j], p[i]-p[j])); // 用来判断点和向量的位置关系
        int u = sgn(p[i].y - pt.y);    // 判断水平直线有没有穿过当前线段
        int v = sgn(p[j].y - pt.y);   
        if (c > 0 && u < 0 && v >= 0) num ++;
        if (c < 0 && u >= 0 && v < 0) num --;
    }
    return num != 0;   // 1: 点在内部;  0: 点在外部
}
// 求多边形的周长
db polygonperimeter(Point* p, int n) {
    p[n] = p[0];
    db ans = 0;
    for(int i = 0; i < n; i ++)
        ans += Dist(p[i], p[i+1]);
    return ans;
}
// 求多边形的面积 (三角剖分) 
db polygonarea(Point* p, int n) {
    db ans = 0;
    p[n] = p[0];
    for (int i = 0; i < n; i ++) 
        ans += Cross(p[i], p[i+1]);
    return ans / 2;           // 面积有正负, 需根据题意来判断要不要取绝对值
}
// 求多边形的重心 (三角剖分) 
Point polygoncenter(Point* p, int n) {
    Point ans = O;
    p[n] = p[0];
    db area = polygonarea(p, n);
    if (sgn(area) == 0) return ans;
    for (int i = 0; i < n; i ++) // 重心: (p[i]+p[i+1])/3, 有向面积: Cross(p[i], p[i+1])/2
        ans = ans + (p[i]+p[i+1])*Cross(p[i], p[i+1]); 
    return ans/area/6;
}
