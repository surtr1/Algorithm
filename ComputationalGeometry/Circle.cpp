/*========================================================*\
| 
| 使用Circle类时
| 将Point类的 + - * / Cross Dot Dist 补全
| 将Line类的 基本结构 补全
| 							Circle 类
| 							类内函数
| Circle(P * 3) 								- 三角形的外接圆
| Circle(P * 3, bool t) 						- 三角形的内切圆
| input() 										- 读入一个圆, 圆心+半径
| operator == 									- 两个圆的相等关系, 同心同半径
| operator < 									- 用于排序, 先点后半径, 符号与<同向
| area() 										- 返回圆的面积
| circumference()								- 返回圆的周长
| point(db rad)									- 圆上夹角为rad的一点
| 							类外函数
| *几何关系
| relation(C c, P a) 							- 点和圆的关系 (0: 圆外, 1: 圆上, 2: 圆内) 
| relationseg(C c, L v)			 				- 线段和圆的关系, 返回交点个数
| relationline(C c, L v) 						- 直线和圆的关系, 返回交点个数
| relationcircle(C * 2) 						- 两圆的关系 (5: 相离, 4: 外切, 3: 相交, 2: 内切, 1: 内含, 0: 重合) 
| *与圆的交点
| ppcrosslc(C a, L b, P& * 2)					- 直线与圆的交点
| ppcrosscc(C a, C b, P& * 2) 					- 两圆相交的交点
| *与圆的面积
| areacircle(C * 2)								- 求两圆相交的面积
| *与圆的切线
| ltangentpc(P p, C a, int clock)				- 过一点作圆的切线
| ltangentcc(C * 2, P* p1, P* p2)				- 求两圆的公切线
| *特殊方法得到圆
| getcircle(P * 2, db r0, C& * 2)				- 得到过 a, b 两点, 半径为 r0 的两个圆
| getcircle(L u, P q, db r0, C& * 2)			- 得到与直线 u 相切, 过点 q, 半径为 r0 的圆
| getcircle(L u, L v, db r0, C& * 4)			- 得到与直线 u, v 相切, 半径为 r0 的圆
| getcircle(C cx, C cy, db r0, C& * 2)			- 得到同时与不相交圆 cx, cy 相切, 半径为 r0 的圆
| getcircle(P * 3, C& c);						- 三角形的外接圆
| getcircle(P * 3, bool t, C& c);				- 三角形的内切圆
\*========================================================*/
struct Circle {
    Point p; db r;
    Circle(Point p = Point(0, 0), db r = 0): p(p), r(r) {}
    // 三角形的外接圆, 利用两条边的中垂线得到圆心, 测试: UVA12304
    //1. Point: rotleft
    //2. Line: pcrossll
    Circle(Point a, Point b, Point c) {
        Point x = (a+b)/2, y = (b+c)/2;
        Line u = Line(x, x + (b-a).rotleft());
        Line v = Line(y, y + (c-b).rotleft());
        p = pcrossll(u, v);
        r = Dist(p, a);
    }
    // 三角形的内切圆, bool t 没有作用, 只是为了和上面区别, 测试: UVA12304
    //1. Line: pcrossll, dispts
    Circle(Point a, Point b, Point c, bool t) {
        Line u, v;
        db m = atan2(b.y-a.y, b.x-a.x), n = atan2(c.y-a.y, c.x-a.x);
        u.s = a, u.e = u.s + Point (cos((n+m)/2), sin((n+m)/2));		//a, u
        m = atan2(a.y-b.y, a.x-b.x), n = atan2(c.y-b.y, c.x-b.x);
        v.s = b, v.e = v.s + Point (cos((n+m)/2), sin((n+m)/2));		//b, v
        p = pcrossll(u, v);
        r = dispts(Line(a, b), p);
    }
    void input() { p.input(); scanf("%lf", &r); }
    // 1. Point → == =()
    bool operator == (Circle c) const { 
        return p == c.p && sgn(r-c.r) == 0; // == = (p, c.p){return sgn(p.x - c.p.x)==0 && sgn(p.y - c.p.y)==0;}
    } 
    bool operator < (Circle c) const { return p < c.p || (p == c.p && sgn(r-c.r) < 0); }
    // 面积
    db area() { return PI*r*r; }
    // 周长
    db circumference() { return 2*PI*r; }
    // 圆的参数方程
    Point point(db rad) {
        return Point(p.x+cos(rad)*r, p.y+sin(rad)*r);
    }
};
/******************************\
|			几何关系
\******************************/
// 点和圆的关系 (0: 圆外, 1: 圆上, 2: 圆内) 
int relation(Circle c, Point a) {
    db dst = Dist(a, c.p);
    if (sgn(dst - c.r) < 0) return 2;
    else if (sgn(dst - c.r) == 0) return 1;
    return 0;
}
// 线段和圆的关系, 比较的是圆心到线段的距离和半径的关系
// 1. Line: dispts
int relationseg(Circle c, Line v) {
    db dst = dispts(v, c.p);
    if (sgn(dst - c.r) < 0) return 2;
    else if (sgn(dst - c.r) == 0) return 1;
    return 0;
}
// 直线和圆的关系, 比较的是圆心到直线的距离和半径的关系
// 1. Line: disptl
int relationline(Circle c, Line v) {
    db dst = disptl(v, c.p);
    if (sgn(dst - c.r) < 0) return 2;
    else if (sgn(dst - c.r) == 0) return 1;
    return 0;
}
// 两圆的关系 (5: 相离, 4: 外切, 3: 相交, 2: 内切, 1: 内含, 0: 重合) 
int relationcircle(Circle a, Circle b) {
    db d = Dist(a.p, b.p);
    if (sgn(d - a.r - b.r) > 0) return 5;
    if (sgn(d - a.r - b.r) == 0) return 4;
    db l = fabs(a.r - b.r);
    if (sgn(d - a.r - b.r) < 0 && sgn(d - l) > 0) return 3;
    if (sgn(d - l) == 0) return 2;
    if (sgn(d - l) < 0) return 1;
    return 0;
}
/******************************\
| 			与圆的交点
\******************************/
// 直线与圆的交点: 利用圆中的直角三角形求解
// 1. Point: trunc
// 2. Line: propl
int ppcrosslc(Circle a, Line b, Point &p1, Point &p2) {
    Point p = propl(a.p, b);				//中点
    db d = Dist(p, a.p);
    if (sgn(d - a.r) > 0) return 0;			//相离
    if (sgn(d - a.r) == 0) {				//相切
        p1 = p2 = p;
        return 1;
    }
    Point dir = (b.s - b.e).trunc(1);		//相交
    db len = mysqrt(a.r*a.r - d*d);
    p1 = p - dir*len, p2 = p + dir*len;
    return 2;
} 
// 两圆相交的交点: 利用余弦定理
// 1. Point -> == = Fun()
// 2. Circle: point
int ppcrosscc(Circle a, Circle b, Point &p1, Point &p2) {
    db d = Dist(a.p, b.p);
    if (!sgn(d)) {
        if (!sgn(a.r - b.r)) return -1; // 两圆重合
        else return 0; // 无交点, 同心圆
    }
    if (sgn(a.r + b.r - d) < 0) return 0; // 外离
    if (sgn(fabs(a.r-b.r) - d) > 0) return 0;
    Vector dir = b.p - a.p;
    db ang = atan2(dir.y, dir.x);		//要得到一个 (-PI, PI] 内的角
    db rad = acos((a.r*a.r+d*d-b.r*b.r)/(2*a.r*d));
    p1 = a.point(ang - rad);
    p2 = a.point(ang + rad);
    return 1 + ! (p1 == p2);				//== = [](){return sgn(p1.x - p2.x) == 0 && sgn(p1.y - p2.y) == 0;}
}
/******************************\
| 			与圆的面积
\******************************/
// 求两圆相交的面积
// 1. Circle: relationcircle
// 1. Circle → area
db areacircle(Circle a, Circle b) {
    int rel = relationcircle(a, b);
    if (rel >= 4) return 0.0;
    if (rel <= 2) return min(a.area(), b.area());
    db d = Dist(a.p, b.p);
    db hf = (a.r + b.r + d) / 2.0; 
    db ss = 2 * sqrt(hf*(hf-a.r)*(hf-b.r)*(hf-d)); // 海伦公式
    db a1 = acos((a.r*a.r + d*d - b.r*b.r)/(2*a.r*d)); // 余弦定理, 求的是角度的一般
    db a2 = acos((b.r*b.r + d*d - a.r*a.r)/(2*b.r*d));
    a1 = a1*r*r, a2 = a2*r*r;   // 扇形面积, 这里不用乘以0.5
    return a1 + a2 - ss;
}
/******************************\
| 			与圆的切线
\******************************/
// 过一点作圆的切线(不用保证一定是圆外的点), 测试: POJ1375
// 1. Point: rotleft, rotright, trunc
// 2. Circle: relation
int ltangentpc(Circle c, Point p, Line &u,Line &v){
	int t = relation(c, p);
	Vector dir = p - c.p;
	if(t == 2) return 0;
	if(t == 1){
		u = Line(p, p + dir.rotleft());
		v = u;
		return 1;
	}
	db d = Dist(c.p, p);
	db l = c.r * c.r / d;
	db h = sqrt(c.r * c.r - l * l);
	u = Line(p, c.p + (dir.trunc(l) + dir.rotleft().trunc(h)));
	v = Line(p, c.p + (dir.trunc(l) + dir.rotright().trunc(h)));
	return 2;
}
// 求两圆的公切线, 测试: UVA10674
// 1. Circle: point
int ltangentcc(Circle a, Circle b, Point* p1, Point* p2) {
    if (a.r < b.r) swap(a, b), swap(p1, p2);
    db diff = a.r - b.r;
    db sum = a.r + b.r;
    db d2 = (a.p.x-b.p.x)*(a.p.x-b.p.x) + (a.p.y-b.p.y)*(a.p.y-b.p.y);
    // 情况一: 重合
    if (a.p == b.p && sgn(a.r - b.r) == 0)  return -1;
    // 情况二: 内含
    if (sgn(d2 - diff*diff) < 0) return 0;
    // 情况三: 内切
    int res = 0;
    db base = atan2(b.p.y-a.p.y, b.p.x-a.p.x);
    if (sgn(d2 - diff*diff) == 0) {
        p1[res] = a.point (base), p2[res] = b.point (base); res ++;
        return 1;
    }
    // 情况四: 相交 (有外公切线) 
    db ang = acos((a.r-b.r)/mysqrt(d2));
    p1[res] = a.point (base + ang), p2[res] = b.point (base + ang), res ++;
    p1[res] = a.point (base - ang), p2[res] = b.point (base - ang), res ++;
    // 情况五: 外切 (有一条内公切线) 
    if (sgn(sum*sum - d2) == 0) {
        p1[res] = p2[res] = a.point(base), res ++;
    }
    // 情况六: 外离 (有两条内公切线) 
    else if (sgn(sum*sum - d2) < 0) {
        ang = acos ((a.r + b.r) / mysqrt(d2));
        p1[res] = a.point (base + ang), p2[res] = b.point (PI + base + ang), res ++;
        p1[res] = a.point (base - ang), p2[res] = b.point (PI + base - ang), res ++;
    }
    return res;
}
/******************************\
| 			特殊方法得到圆
\******************************/
// 得到过 a, b 两点, 半径为 r0 的两个圆, 测试: UVA12304
// 1. Circle: ppcrosscc(...)
int getcircle(Point a, Point b, db r0, Circle &c1, Circle &c2) {
    Circle x(a, r0), y(b, r0);
    int t = ppcrosscc(x, y, c1.p, c2.p);
    if (! t) return 0;
    c1.r = c2.r = r0;
    return t;
}
// 得到与直线 u 相切, 过点 q, 半径为 r0 的圆, 测试: UVA12304
// 1. Point: rotleft, rotright, trunc, == = Fun()
// 2. Line: disptl(...)=Point: Dist
// 3. Circle: ppcrosslc(...)
int getcircle(Line u, Point p, db r0, Circle &c1, Circle &c2) {
    db dis = disptl(u, p);			// disptl=[](u, p){return fabs(Cross(u.s, p, u.e)) / Dist(u.s, u.e);}
    if (sgn(dis - r0*2) > 0) return 0;
    Point down = (u.e-u.s).rotleft().trunc(r0);
    Point up = (u.e-u.s).rotright().trunc(r0);	//两个对应点
    if (!sgn(dis)) {
        c1.p = p + down;
        c2.p = p + up;
        c1.r = c2.r = r0;
        return 2;
    }
    Line u1(u.s + down, u.e + down), u2(u.s + up, u.e + up);	//两条线
    Circle cc(p, r0);
    Point p1, p2;
    if (! ppcrosslc(cc, u1, p1, p2))	//只会与一条线交
        ppcrosslc(cc, u2, p1, p2);
    c1 = Circle(p1, r0), c2 = Circle(p2, r0);
    if (p1 == p2) return 1; 			//相切	== = [](){return sgn(p1.x - p2.x) == 0 && sgn(p1.y - p2.y) == 0;}
    return 2;							//相交
}
// 得到与直线 u, v 相切, 半径为 r0 的圆, 测试: UVA12304
// 1. Point: rotleft, rotright, trunc
// 2. Line: parallel=Fun(), pcrossll
int getcircle(Line u, Line v, db r0, Circle &c1, Circle &c2, Circle &c3, Circle &c4) {
    if (parallel(u, v) == 1) return 0;				// parallel=[](u, v){return sgn(Cross(u.e - u.s, v.e - v.s)) == 0}
    Point down1 = (u.e-u.s).rotleft().trunc(r0);
    Point down2 = (v.e-v.s).rotleft().trunc(r0);
    Point up1 = (u.e-u.s).rotright().trunc(r0);
    Point up2 = (v.e-v.s).rotright().trunc(r0);		//得到四个点
    Line u1(u.s + down1, u.e + down1), u2(u.s + up1, u.e + up1);
    Line v1(v.s + down2, v.e + down2), v2(v.s + up2, v.e + up2);	//得到四条平行线
    c1.r = c2.r = c3.r = c4.r = r0;
    c1.p = pcrossll(u1, v1);
    c2.p = pcrossll(u1, v2);
    c3.p = pcrossll(u2, v1);
    c4.p = pcrossll(u2, v2);										//四个交点对应四个圆心
    return 4;
}
// 得到同时与不相交圆(要保证不相交) cx, cy 相切, 半径为 r0 的圆, 测试: UVA12304
// 1. Circle: ppcrosscc(...)
int getcircle(Circle cx, Circle cy, db r0, Circle &c1, Circle &c2) {
    Circle x(cx.p, r0+cx.r), y(cy.p, r0+cy.r);	//两圆交点即为圆心
    int t = ppcrosscc(x, y, c1.p, c2.p);
    if (! t) return 0;		//无圆
    c1.r = c2.r = r0;
    return t;
}
// 三角形的外接圆, 利用两条边的中垂线得到圆心, 测试: UVA12304
//1. Point: rotleft
//2. Line: pcrossll
int getcircle(Point a, Point b, Point c, Circle &c1) {
    Point x = (a+b)/2, y = (b+c)/2;
    Line u = Line(x, x + (b-a).rotleft());
    Line v = Line(y, y + (c-b).rotleft());	//两条中垂线
    c1.p = pcrossll(u, v);					//交点为圆心
    c1.r = Dist(c1.p, a);
    return 1;
}
// 三角形的内切圆, bool t 只是为了和上面区别, 测试: UVA12304
//1. Line: pcrossll, dispts
int getcircle(Point a, Point b, Point c, bool t, Circle &c1) {
    Line u, v;
    db m = atan2(b.y-a.y, b.x-a.x), n = atan2(c.y-a.y, c.x-a.x);//两个角
    u.s = a, u.e = u.s + Point (cos((n+m)/2), sin((n+m)/2));		//a, u
    m = atan2(a.y-b.y, a.x-b.x), n = atan2(c.y-b.y, c.x-b.x);
    v.s = b, v.e = v.s + Point (cos((n+m)/2), sin((n+m)/2));		//b, v //两条角平分线
    c1.p = pcrossll(u, v);
    c1.r = dispts(Line(a, b), c1.p);
    return 1;
}
