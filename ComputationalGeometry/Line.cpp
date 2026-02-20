/*==================================================*\
| 使用Line类时,先将Point类的 + - Cross Dot Dist 补全
| 							Line 类
| 							类内函数
| Line(P s = o, e = o) 						- 构造函数 默认
| operator ==								- 两条线段的关系, 需要同向
| Line(P s, db ang) 						- 构造函数 点 + 倾斜角
| Line(P a, P b, P c)						- 构造函数 ax + by + c = 0
| input()									- 读入一条直线
| operator &								- 返回两条直线的交点以及关系 pair<int, P> (0: 平行, 1: 重合, 2: 相交)
| adjust() 									- 改为方向, 直线指向上方
| length() 									- 返回线段长度
| 							类外函数
| *基础操作
| angle(L v)								- 返回直线倾斜角db 0 <= ang < PI
| *几何关系
| relation(P p, L v)						- 点和直线的关系 (1: 在左侧, 2: 在右侧, 3: 在直线上)
| segrelation(P p, L v)						- 点和线段的关系 (true: 在线段上, false: 不在线段上)? 
| parallel(L a, L b)						- 两直线的平行关系 (true: 平行 重合, false: 相交)
| segrelationseg(L a, L b)					- 两线段相交判断 (2: 规范相交, 1: 非规范相交, 0: 不相交)
| relationseg(L a, L b) 					- 直线和线段的相交判断 (2: 规范相交, 1: 非规范相交, 0: 不相交)
| relationline(L a, L b)					- 两直线的关系 (0: 平行, 1: 重合, 2: 相交)	
| *交点
| pcrossll(L a, L b) 						- 两直线的交点 (要保证两直线不平行或重合)
| pcrossll(L a, L b, P& p)					- 两直线的交点 (无需判断两直线的关系) (0: 平行, 1: 重合, 2: 相交)
| *距离
| disptl(L v, P p) 							- 点到直线的距离 db
| dispts(L v, P p) 							- 点到线段的距离 db
| dissts(L a, L b) 							- 线段到线段的距离 (保证两条线段不相交) db
| *点线关系
| propl(P p, L v) 							- 返回点在直线上的投影 P, 同时也是垂足
| symmetrypoint(L v, P p) 					- 返回点关于直线的对称点 P
\*==================================================*/
#include <cmath>
#include <cstdio>
using namespace std;

using db = double;
const db eps = 1e-8;
const db inf = 1e20;
const db PI = acos(-1.0);
inline int sgn(db x) {return x < -eps ? -1 : x > eps;}
inline db sqr(db x) { return x*x; }
inline db mysqrt(db x) { return sqrt(max(0.0, x)); }
//注意y1给系统占了
struct Point {
    db x, y;
    Point (db x=0, db y=0): x(x), y(y) {}
    void input() { scanf("%lf%lf", &x, &y); }
    bool operator == (Point r) { return sgn(x-r.x)==0 && sgn(y-r.y)==0; }
    bool operator < (Point r) { return sgn(x-r.x)==0 ? y < r.y : x < r.x; }
    Point operator + (Point r) { return Point (x+r.x, y+r.y); }
    Point operator - (Point r) { return Point (x-r.x, y-r.y); }
    Point operator * (db r) { return Point (x*r, y*r); }
    Point operator / (db r) { return Point (x/r, y/r); }

    // 逆时针旋转 90 度
    Point rotleft() { return Point (-y, x); }
    // 顺时针旋转 90 度
    Point rotright() { return Point (y, -x); }
    // 绕着 p 点逆时针旋转 ang
    Point rotate (Point p, db ang) {
        Point v = (*this) - p;
        db c = cos(ang), s = sin(ang);
        return Point (p.x + v.x*c - v.y*s, p.y + v.x*s + v.y*c);
    }
};

db Cross (Point a, Point b) { return a.x*b.y - a.y*b.x; }
db Cross (Point pt, Point a, Point b) { return Cross(a-pt, b-pt); }
db Dot (Point a, Point b) { return a.x*b.x + a.y*b.y; }
db Dot (Point pt, Point a, Point b) { return Dot (a-pt, b-pt); }
db Dist (Point a, Point b) { return mysqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)); }
db Len2 (Point a) { return a.x*a.x + a.y*a.y; }


struct Line {
    Point s, e;
    Line (Point s=Point(0,0), Point e=Point(0,0)): s(s), e(e) {}
    bool operator == (Line v) { return (s==v.s) && (e==v.e); }
    // 点 和 倾斜角
    Line (Point p, db ang) {
        s = p;
        if (sgn(ang - PI/2) == 0) e = s + Point (0, 1);
        else e = s + Point (1, tan(ang));
    }
    // 一般式 ax + by + c = 0
    Line (db a, db b, db c) {
        if (sgn(a) == 0) {
            s = Point (0, -c/b);
            e = Point (1, -c/b);
        }else if (sgn(b) == 0) {
            s = Point (-c/a, 0);
            e = Point (-c/a, 1);
        }else {
            s = Point (0, -c/b);
            e = Point (1, (-c-a)/b);
        }
    }
    void input() { s.input(), e.input(); }
    // 返回两条直线的交点以及关系
    pair<int, Point> operator & (Line r) {
        Point res = s;
        if(sgn(Cross(r.e - r.s, e - s)) == 0) { // 共线
            if(sgn(Cross(r.e - r.s, e - r.s)) == 0) return make_pair(1, res); // 1: 重合
            return make_pair(2, res); // 2: 平行
        }
        db t = Cross(r.s, s, r.e)/Cross(r.e-r.s, e-s);
        res.x += (e.x-s.x) * t;
        res.y += (e.y-s.y) * t;
        return make_pair(0, res); // 0: 相交
    }
    // 改为方向指向上方
    void adjust() { if (e < s) swap(s, e); }
    // 求线段的长度
   	db length() { return Dist (s, e); }
};
/******************************\
| 			基础操作
\******************************/
// 返回直线倾斜角 0 <= ang < PI
db angle(Line v) {
    db k = atan2(v.e.y-v.s.y, v.e.x-v.s.x);
    if (sgn(k) < 0) k += PI;
    if (!sgn(k - PI)) k -= PI;
    return k;
}
/******************************\
| 			几何关系
\******************************/
// 点和直线的关系 1: 在左侧,  2: 在右侧, 3: 在直线上
int relation(Point p, Line v) {
    int c = sgn(Cross(v.s, p, v.e));
    if (c < 0) return 1;
    else if (c > 0) return 2;
    else return 3;
}
// 点在线段上的判断
bool segrelation(Point p, Line v) {
    return sgn(Cross(v.s, p, v.e)) == 0 && sgn(Dot(p-v.s, p-v.e)) <= 0;
}
// 两向量平行  (对应直线平行或重合) 
bool parallel(Line a, Line b) { 
    return sgn(Cross(a.e-a.s, b.e-b.s)) == 0;
}
// 两线段相交判断 (2: 规范相交, 1: 非规范相交, 0: 不相交) 
int segrelationseg(Line a, Line b) {
    int d1 = sgn(Cross(a.s, a.e, b.s));
    int d2 = sgn(Cross(a.s, a.e, b.e));
    int d3 = sgn(Cross(b.s, b.e, a.s));
    int d4 = sgn(Cross(b.s, b.e, a.e));
    if ((d1^d2) == -2 && (d3^d4) == -2) return 2;
    return (d1 == 0 && sgn(Dot(b.s-a.s, b.s-a.e)) <= 0) ||
        (d2 == 0 && sgn(Dot(b.e-a.s, b.e-a.e)) <= 0) ||
        (d3 == 0 && sgn(Dot(a.s-b.s, a.s-b.e)) <= 0) ||
        (d4 == 0 && sgn(Dot(a.e-b.s, a.e-b.e)) <= 0);
}
// 直线和线段的相交判断 (2: 规范相交, 1: 非规范相交, 0: 不相交) , b 是线段
int relationseg(Line a, Line b) {
    int d1 = sgn(Cross(a.s, a.e, b.s));
    int d2 = sgn(Cross(a.s, a.e, b.e));
    if ((d1^d2) == -2) return 2;
    return (d1 == 0 || d2 == 0);
}
// 两直线的关系 (0: 平行, 1: 重合, 2: 相交) 
// 1. Line: parallel()
// 2. Line: relation()
int relationline(Line a, Line b) {
    if (parallel(a, b)) 
        return relation(a.s, b) == 3;
    return 2;
}
// 两直线的交点 (要保证两直线不平行或重合) , 测试: POJ1269 simply
// 如果要判断是否相交, 建议用下面那个
/******************************\
| 			交点
\******************************/
Point pcrossll(Line a, Line b) {
    db k = Cross(a.s, b.s, b.e)/Cross(a.e-a.s, b.e-b.s);
    return a.s + (a.e-a.s)*k;
}
// 两直线相交 (0: 平行, 1: 重合, 2: 相交) 
// 无需判断两直线的关系
int pcrossll(Line a, Line b, Point &p) {
    if (sgn(Cross(a.e - a.s, b.e - b.s)) == 0) 
        return sgn(Cross(b.s, a.s, b.e)) == 0;
    else {
        db k = Cross(a.s, b.s, b.e)/Cross(a.e-a.s, b.e-b.s);
        p = a.s + (a.e-a.s)*k;
        return 2;
    }
}
/******************************\
| 			距离
\******************************/
// 点到直线的距离
// 1. Line → length(...)=Point: Dist()
db disptl(Line v, Point p) {
    return fabs(Cross(v.s, p, v.e)) / v.length(); // length = [](){return Dist(v.s, v.e);}
}

// 点到线段的距离
// 1. Line: disptl(...)=Point: Dist()
db dispts(Line v, Point p) {
    if (sgn(Dot(p-v.s, v.e-v.s)) < 0 || sgn(Dot(p-v.e, v.s-v.e)) < 0)
        return min(Dist(p, v.s), Dist(p, v.e));
    return disptl(v, p);			// disptl = [](v, p){return fabs(Cross(v.s, p, v.e)) / Dist(v.s, v.e);}
}
// 线段到线段的距离, 前提是两条线段不相交
// 1. Line: dispts(...(...))
db dissts(Line a, Line b) {
    return min(dispts(a, b.s), dispts(a, b.e));
}
// 返回点 p 在直线上的投影
// 1. Point: Len2
/******************************\
| 			点线关系
\******************************/
// 返回点在直线上的投影 P
// 1. Point: Len2()=Point: Len2()
Point propl(Point p, Line v) {
    Point t = v.e - v.s;
    return v.s + t * Dot(t, p-v.s) / Len2(t);		//Len2=(t)[] { return （t.x*t.x + t.y*t.y); }	
}
// 返回点 p 关于直线的对称点
// 1. Line: propl(...)
Point symmetrypoint(Line v, Point p) {
    Point q = propl(p, v);
    return Point (2*q.x-p.x, 2*q.y-p.y);
}
