

/*========================================================*\
| Point 类
| 							类内函数
| Point(P x = 0, P y = 0)					- 默认构造函数
| input()									- 读入一个点
| operator == 								- NULL
| operator <								- 用于排序
| operator +								- 返回Point
| operator -								- 返回Point
| operator *								- 返回Point
| operator /								- 返回Point
| len()										- 返回长度
| len2()									- 返回长度平方		
| trunc(db len)								- 长度变为len
| rotleft()									- 逆时针转π/2, 返回Point
| rotright()								- 顺时针转π/2, 返回Point
| rotate(P p, db ang)						- 绕p逆时针转ang
| 							类外函数
| *基础功能
| Cross(P a, P b)							- NULL
| Cross(P pt, P a, P b)						- NULL
| Dot(P a, P b)								- NULL
| Dot(P pt, P a, P b)						- NULL
| Dist(P a, P b)							- NULL
| Len(a)									- 返回a的长度
| Len2(a) 									- 返回a的长度平方
| rad(P p, P a, P b) 						- 返回∠apb	
| Complex类补充
\*========================================================*/
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
    db len() { return mysqrt(x*x+y*y); }
    db len2() { return x*x + y*y; }
    // 化为长度为 r 的向量
    Point trunc(db len) {
        db l = mysqrt(x * x + y * y);
        if (! sgn(l)) return *this;
        len /= l;
        return Point (x*len, y*len);
    }
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
/******************************\
| 			基础功能
\******************************/
db Cross (Point a, Point b) { return a.x*b.y - a.y*b.x; }
db Cross (Point pt, Point a, Point b) { return Cross(a-pt, b-pt); }
db Dot (Point a, Point b) { return a.x*b.x + a.y*b.y; }
db Dot (Point pt, Point a, Point b) { return Dot (a-pt, b-pt); }
db Dist (Point a, Point b) { return mysqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)); }
db Len (Point a) { return mysqrt(a.x*a.x + a.y*a.y); }
db Len2 (Point a) { return a.x*a.x + a.y*a.y; }
// 计算 pa 和 pb 的夹角,  就是求这个点看 a, b 所成的夹角, 测试 LightOJ 1203
db rad(Point p, Point a, Point b) { 
    return fabs(atan2(fabs(Cross(p, a, b)), Dot(p, a, b) )); 
}
/******************************\
| 			Complex补充
\******************************/
struct Complex {
    db x, y;
    Complex(db x = 0, db y = 0) : x(x), y(y) {}
    Complex operator + (Complex b) {return Complex(x + b.x, y + b.y);}
    Complex operator - (Complex b) {return Complex(x - b.x, y - b.y);}
    Complex operator * (Complex b) {return Complex(x * b.x - y * b.y, x * b.y + y * b.x);}
};

