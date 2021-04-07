# 计算几何

## SuperPoint，点线的基本定义

```cpp
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<algorithm>
const int maxn = 55;
const double eps = 1e-8;
const double pi = acos(-1.0);
int dcmp(double x) {return (x > eps) - (x < -eps);}
inline double min(double a, double b) {return a < b ? a : b;}
inline double max(double a, double b) {return a > b ? a : b;}
inline double Sqr(double x) {return x * x;}
```

### 点的定义

```cpp
struct Point
{
    /**********一般都要用的**********/
    double x, y;
    Point(){x = y = 0;}
    Point(double a, double b) {x = a, y = b;}

    inline Point operator-(const Point &b)const 
    {return Point(x - b.x, y - b.y);}

    inline bool operator<(const Point &b)const //重载<，排序要用
    {return dcmp(x - b.x) ? x < b.x : y < b.y;}

    inline double dot(const Point &b)const //点积
    {return x * b.x + y * b.y;}

    inline double Dis(const Point &b)const //距离
    {return sqrt((*this - b).dot(*this - b));}

    inline double cross(const Point &b, const Point &c)const
    // 三点叉积，(*this).cross(b, c)右手关系为正
    {return (b.x - x) * (c.y - y) - (c.x - x) * (b.y - y);}
    
    /**********点线关系有时候用的**********/
    /*
    bool Parallel()判断平行，struct外的函数
    LineCross()直线交点，struct外的函数
    SegCross()线段交点，struct外的函数
    ToSeg()点到线段距离，声明在struct里具体定义在struct外面
    这四个里，下面的一般会调用上面的。
    注意：当精度要求较高时，尽量用原始的、相互调用少的方式验证。比如
    判断直线与线段交点，用叉积判断是否有交点后计算交点，精度高于先把线段当直线计算直线交点后再用InSeg判断是否在线段上。
    */
    double ToSeg(const Point&, const Point&)const; //点到线段距离*/

    inline Point operator+(const Point &b)const
    {return Point(x + b.x, y + b.y);}
    inline Point operator*(const double &b)const //x、y扩大常数倍
    {return Point(x * b, y * b);}
    inline Point operator-()
    {return Point(-x, -y);}

    inline bool InLine(const Point &b, const Point &c)const //三点共线
    {return !dcmp(cross(b, c));}

    /* 注意：如果已知点与线段端点共线，只是判断在两端点范围内，最好去掉 `InLine(b, c) &&`以提高精度*/
    inline bool OnSeg(const Point &b, const Point &c)const //点在线段上，包括端点
    {return InLine(b, c) && (*this - c).dot(*this - b) < eps;}

    inline bool InSeg(const Point &b, const Point &c)const //点在线段上，不包括端点
    {return InLine(b, c) && (*this - c).dot(*this - b) < -eps;}

    /**********其他。需要了可添加**********/
    inline bool operator>(const Point &b)const
    {return b < *this;}

    inline bool operator==(const Point &b)const
    {return !dcmp(x - b.x) && !dcmp(y - b.y);}

    Point RotePoint(const Point &p, double ang) //p绕*this逆时针旋转ang弧度
    {
        return Point((p.x - x) * cos(ang) - (p.y - y) * sin(ang) + x,
                (p.x - x) * sin(ang) + (p.y - y) * cos(ang) + y);
    }
};
```

### 判断两直线平行

```cpp
bool Parallel(Point a, Point b, Point c, Point d)
{return !dcmp(a.cross(b, a + d - c));} 
```

### 判断直线与线段相交（包含端点）

```cpp
// a b 为直线， c d 为线段。行末改为 `< -eps;` 则不包含端点
bool LineInterSeg(const Point a, Point b, Point c, Point d)
{
    return !Parallel(a, b, c, d) &&  a.cross(b, c) * a.cross(b, d) < eps;
}
```

### 直线交点

```cpp
Point LineCross(Point a, Point b, Point c, Point d)
{
    double u = a.cross(b, c), v = b.cross(a, d);
    return Point((c.x * v + d.x * u) / (u + v), (c.y * v + d.y * u) / (u + v));
}
```

### 点到线段距离

```cpp
double Point::ToSeg(const Point &b, const Point &c)const
{
    Point t(x + b.y - c.y, y + c.x - b.x);
    if(cross(t, b) * cross(t, c) > eps)
        return min(Dis(b), Dis(c));
    return Dis(LineCross(*this, t, b, c));
}
```

### 线段交点，包括端点

```cpp
bool SegCross(Point a,  Point b,  Point c,  Point d, Point &p) 
{
    if(!Parallel(a, b, c, d) && a.cross(b, c) * a.cross(b, d) <= 0 && c.cross(d, a) * c.cross(d, b) <= 0)
    {
        p = LineCross(a, b, c, d);
        return true;
    }
    return false;
}
 
```

## 经典应用

### 半平面交

```cpp
struct Line
// 定义半平面
{
     //double a, b, c; //ax + by + c = 0
    Point s, e; //s->e向量表示有向直线，半平面在直线左侧
    double ang, d;
    inline void Read(Point s_, Point e_)
    {
        s = s_, e = e_;
        ang = atan2(e.y - s.y, e.x - s.x);
        if(dcmp(s.x - e.x)) d = (s.x * e.y - e.x * s.y) / fabs(s.x - e.x);
        else d = (s.x * e.y - e.x * s.y) / fabs(s.y - e.y);
        /**********/
         //如果需要a,b,c，在这里计算
        /**********/
    }
    /*void Read(double a_, double b_, double c_, bool up) //up表示半平面在直线方程上方
    {
         //a = a_, b = b_, c = c_;
        if(b_ < -eps) a_ = -a_, b_ = -b_, c_ = -c_, up ^= 1;
        else if(!dcmp(b_) && a_ < -eps) a_ = -a_, c_ = -c_, up ^= 1;
        if(!dcmp(a_) && !dcmp(b_))
        {
            if(-c_ > eps && !up || -c_ < -eps && up) Read(Point(inf, inf), Point(-inf, inf));
            else Read(Point(inf, -inf), Point(-inf, -inf));
            return;
        }
        else if(!dcmp(a_)) Read(Point(0, -c_ / b_), Point(1, -c_ / b_));
        else if(!dcmp(b_)) Read(Point(-c_ / a_, 1), Point(-c_ / a_, 0));
        else Read(Point(0, -c_ / b_), Point(1, (-c_ - a_) / b_));
        if(!up) {Read(e, s);}
    }*/
    Line(){}
    Line(Point s_, Point e_){Read(s_, e_);}
    // Line(double a_, double b_, double c_, bool up){Read(a_, b_, c_, up);}
    inline bool Parallel(const Line &l)
    {return !dcmp( (e.x - s.x) * (l.e.y - l.s.y) - (e.y - s.y) * (l.e.x - l.s.x) );}
    Point operator*(const Line &l)const //求两不平行不重合直线交点
    {
        double u = s.cross(e, l.s), v = e.cross(s, l.e);
        return Point( (l.s.x * v + l.e.x * u) / (u + v),
                    (l.s.y * v + l.e.y * u) / (u + v) );
    }
    bool operator<(const Line &l)const //排序函数，优先极角，“左”边直线靠前
    {return dcmp(ang - l.ang) ? ang < l.ang : d < l.d;}
};
bool HalfPanelCross(Line l[], int n, Point cp[], int &m) 
// 半平面交
{
    int i, tn;
    m = 0;
    std::sort(l, l + n);
    for(i = tn = 1; i < n; ++ i) if(dcmp(l[i].ang - l[i - 1].ang))l[tn ++] =l[i];
    n = tn;
    int front = 0, rear = 1;
    deq[0] = l[0], deq[1] = l[1];
    for(i = 2; i < n; ++ i)
    {
        if(deq[rear].Parallel(deq[rear - 1]) || deq[front].Parallel(deq[front + 1])) return false;
        while(front < rear && dcmp( l[i].s.cross(l[i].e, deq[rear] * deq[rear - 1]) ) < 0) -- rear;
        while(front < rear && dcmp( l[i].s.cross(l[i].e, deq[front] * deq[front + 1]) ) < 0) ++ front;
        deq[++ rear] = l[i];
    }
    while(front < rear && dcmp( deq[front].s.cross(deq[front].e, deq[rear] * deq[rear - 1]) ) < 0) -- rear;
    while(front < rear && dcmp( deq[rear].s.cross(deq[rear].e, deq[front] * deq[front + 1]) ) < 0) ++ front;
    if(rear <= front + 1) return false; //两条以下直线，没有围住
     //保存核
    for(i = front; i < rear; ++ i) cp[m ++] = deq[i] * deq[i + 1];
    if(front < rear + 1) cp[m ++] = deq[front] * deq[rear];
    m = std::unique(cp, cp + m) - cp; //去掉重复点
    for(i = 0; i < m; ++ i) cp[i].x = pz(cp[i].x), cp[i].y = pz(cp[i].y); //负0误差修复
    return true;
}

```

### 各边平移求新核

```cpp
Point ParallelMove(Point a, Point b, Point ret, double L) //将ret沿a->b方向左侧垂直平移L
{
    Point tmp;
    double len = a.Dis(b);
    return ret + Point((a.y - b.y) / len * L, (b.x - a.x) / len * L);
}
void MakeNewPanels(Point p[], int n, Line l[], double L) //生成多边形的边向内平移L后的半平面集
{
    p[n] = p[0];
    for(int i = 0; i < n; ++ i)
        l[i].Read(ParallelMove(p[i], p[i + 1], p[i], L),
            ParallelMove(p[i], p[i + 1], p[i + 1], L));
}

```

### Graham凸包

```cpp
int Graham(Point p[], int n, Point res[], int &top) //求凸包,结果为逆时针顺序
{
    int len, i;
    top = 1;
    if(n < 2) {res[0] = p[0]; return 1;}
    std::sort(p, p + n);
    res[0] = p[0], res[1] = p[1];
    for(i = 2; i < n; ++ i)
    {
        while(top && res[top - 1].cross(res[top], p[i]) <= 0)
            -- top;
        res[++ top] = p[i];
    }
    len = top;
    res[++ top] = p[n - 2];
    for(i = n - 3; i >= 0; -- i)
    {
        while(top != len && res[top - 1].cross(res[top], p[i]) <= 0)
            -- top;
        res[++ top] = p[i];
    }
    return top;
}

```

### 简单多边形面积

```cpp
double PolygonArea(Point p[], int n)
{
    if(n < 3) return 0.0;
    double s = p[0].y * (p[n - 1].x - p[1].x);
    p[n] = p[0];
    for(int i = 1; i < n; ++ i)
        s += p[i].y * (p[i - 1].x - p[i + 1].x);
    return fabs(s * 0.5); //顺时针方向s为负
}

```

### 凸多边形最远点对

```cpp
// 需要求点的时候把max换成判断更新。
double CPFMP(Point p[], int n) //ConvexPolygonFarMostPoints
{
    int i, j;
    double ans = 0;
    p[n] = p[0];
    for(i = 0, j = 1; i < n; ++ i)
    {
        while(p[i].cross(p[i + 1], p[j + 1]) > p[i].cross(p[i + 1], p[j]))
                j = (j + 1) % n;
        ans = max(ans, p[i].Dis(p[j]), p[i + 1].Dis(p[j + 1]));
    }
    return ans;
}

 
```

### 两不相交凸多边形最近点对

```cpp
// 需要求点的时候把min换成判断更新。
// 计算方法：double ans = min(CPMDTCP(p, n, q, m), CPMDTCP(q, m, p, n)));
inline double min(double a, double b, double c) 
{return min(a, min(b, c));}
double CPMDTCP(Point p[], int n, Point q[], int m) //ConvexPolygonMinDistanceToConvexPolygon
{
    int i, tp, tq;
    p[n] = p[0], q[m] = q[0];
    for(i = tp = 0; i < n; ++ i) if(p[i].y < p[tp].y) tp = i;
    for(i = tq = 0; i < m; ++ i) if(q[i].y > q[tq].y) tq = i;
    double tmp, mindis = p[tp].Dis(q[tq]);
    for(i = 0; i < n; ++ i)
    {
        while((tmp = p[tp].cross(p[tp + 1], q[tq]) - p[tp].cross(p[tp + 1], q[tq + 1])) < -eps)
            tq = (tq + 1) % m;
        if(tmp > eps) mindis = min(mindis, q[tq].ToSeg(p[tp], p[tp + 1]));
        else mindis = min(mindis,
                          min(p[tp].ToSeg(q[tq], q[tq + 1]), p[tp + 1].ToSeg(q[tq], q[tq + 1])),
                          min(q[tq].ToSeg(p[tp], p[tp + 1]), q[tq + 1].ToSeg(p[tp], p[tp + 1])));
        tp = (tp + 1) % n;
    }
    return mindis;
}
/*角度判断版
double CPMDTCP(Point p[], int n, Point q[], int m) //ConvexPolygonMinDistanceToConvexPolygon
{
    int i, j, inex, jnex, tp0, tq0;
    p[n] = p[0], q[m] = q[0];
    for(i = tp0 = 0; i < n; ++ i) if(p[i].y > p[tp0].y) tp0 = i;
    for(i = tq0 = 0; i < m; ++ i) if(q[i].y < q[tq0].y) tq0 = i;
    double mindis = p[tp0].Dis(q[tq0]);
    double angp = pi, angq = 0, angpnex, angqnex;
    bool flag = false;
    for(i = tp0, j = tq0;; )
    {
        inex = (i + 1) % n, jnex = (j + 1) % m;
        angpnex = atan2(p[inex].y - p[i].y, p[inex].x - p[i].x);
        angqnex = atan2(q[jnex].y - q[j].y, q[jnex].x - q[j].x);
        switch(dcmp(CADis(angp, angpnex) - CADis(angq, angqnex)))
        {
            case 0: mindis = min(mindis,
                                 min(p[i].ToSeg(q[j], q[jnex]), p[inex].ToSeg(q[j], q[jnex])),
                                 min(q[j].ToSeg(p[i], p[inex]), q[jnex].ToSeg(p[i], p[inex])));
                    i = inex, j = jnex; break;
            case 1: mindis = min(mindis,
                                 min(p[i].ToSeg(q[j], q[jnex]), p[inex].ToSeg(q[j], q[jnex])));
                    j = jnex; break;
            case -1:mindis = min(mindis,
                                 min(q[j].ToSeg(p[i], p[inex]), q[jnex].ToSeg(p[i], p[inex])));
                    i = inex; break;
        }
        if(!flag && i != tp0 && j != tq0) flag = true;
        if(flag && (i == tp0 || j == tq0)) break;
    }
    while(i != tp0 && j == tq0)
    {
        inex = (i + 1) % n;
        mindis = min(mindis, min(q[j].ToSeg(p[i], p[inex]), q[jnex].ToSeg(p[i], p[inex])));
        i = inex;
    }
    while(j != tq0 && i == tp0)
    {
        jnex = (j + 1) % m;
        mindis = min(mindis, min(p[i].ToSeg(q[j], q[jnex]), p[inex].ToSeg(q[j], q[jnex])));
        j = jnex;
    }
    return mindis;
}
*/

```

### 凸多边形最大面积三角形

```cpp
double CPFMP(Point p[], int n) //ConvexPolygonFarMostPoints
{
    int i, j, k;
    double ans = 0;
    p[n] = p[0];
    for(i = 0, j = 1, k = 2 % n; i < n; ++ i)
    {
        while(p[i].cross(p[j + 1], p[k]) > p[i].cross(p[j], p[k]))
                j = (j + 1) % n;
        while(p[i].cross(p[j], p[k + 1]) > p[i].cross(p[j], p[k]))
                k = (k + 1) % n;
        ans = max(ans, p[i].cross(p[j], p[k]));
    }
    return ans * 0.5;
}
```

### 给定半径圆和散点集求圆覆盖最多点的个数

```cpp
/*
这里添加角度区间模版
struct Cov
int ctp;
int AScomp(const void *a, const void *b) //角度区间排序
void AngManage(double &x) //角度区间修正，(-pi, pi]
void AddAnSeg(double start, double end)
*/
int CircleCoverPoing(Point p[], int n, double R)
{
    int i, j, ans = 0, cnt;
    double dis, ang, ac, RR = R + R;
    for(i = 0 ; i < n; ++ i)
    {
        for(j = ctp = 0; j < n; ++ j)
            if(j != i && (dis = p[i].Dis(p[j])) < RR + eps) //包不包括圆上来确定 +/- eps
            {
                ang = atan2((double)p[j].y - p[i].y, (double)p[j].x - p[i].x);
                ac = acos(dis * 0.5 / R);
                AddAnSeg(ang - ac, ang + ac);
            }
        qsort(cover, ctp, sizeof(Cov), AScomp);
        for(j = cnt = 0; j < ctp; ++ j)
            ans = std::max(ans, cnt += cover[j].se);
    }
    return ans + 1;
}

```

### 多边形有向边顺逆时针判断及反转

```cpp
// 支持简单多边形
void MakeCounterClock(Point p[], int n) //顺时针则反转多边形的有向边 
{
    int i, id = 0;
    p[n] = p[0];
    for(i = 0; i < n; ++ i) if(p[i].x > p[id].x) id = i;
    if(p[id].cross(p[id + 1], p[(id + n - 1) % n]) > eps) return;
    Point tmp;
    for(i = n - 1 >> 1; i >= 0; -- i)
        tmp = p[i], p[i] = p[n - i - 1], p[n - i - 1] = tmp;
}
```

### 判断点在简单多边形内

```cpp
/*****lrj版（推荐用此版本）*****/ 
bool InSimplePolygon(Point u, Point p[], int n)
{
  int flag = 0;
  for(int i = 0; i < n; i++)
  {
    Point p1 = p[i];
    Point p2 = p[(i+1)%n];
    if(u.OnSeg(p1, p2)) return false; // 在边界上
    int k = dcmp(p1.cross(p2, u));
    int d1 = dcmp(p1.y - u.y);
    int d2 = dcmp(p2.y - u.y);
    if(k > 0 && d1 <= 0 && d2 > 0) flag++;
    if(k < 0 && d2 <= 0 && d1 > 0) flag--;
  }
  return flag != 0;
}

/*****某论文版*****/ 
double AngCounterClock(double start, double end)
// start逆时针旋转到end的弧度 
{return end - start + (end > start - eps ? 2 * pi : 0);}

bool InSimplePolygon(Point u, Point p[], int n/*double neg_inf*/)
// 判断点在简单多边形内，不包括边界，多边形点为逆时针
{
    double neg_inf = -1e20, angvu, angvp1, angvp2;
    Point v(0, u.y), p1, p2, tmp;
    int i, id; //距离u最近交点对应线段起始点id
    bool flag = false;
    p[n] = p[0];
    //设u->v为水平负向射线
    /*for(i = 0, neg_inf = 0; i < n; ++ i) neg_inf = min(neg_inf, p[i].x);
    neg_inf -= 100.0;    */
    v.x = neg_inf;
    for(i = 0; i < n; ++ i)
    {
        if(u.OnSeg(p[i], p[i + 1])) return false;
        if(!SegCross(u, v, p[i], p[i + 1], tmp)) continue;
        flag = true;
        if(tmp.x - v.x > eps) v.x = tmp.x, id = i;
    }
    if(!flag) return false;
    p1 = v == p[id] ? p[(id + n - 1) % n] : p[id];
    p2 = v == p[id + 1] ? p[(id + 2) % n] : p[id + 1];
    angvu = atan2(u.y - v.y, u.x - v.x);
    angvp1 = atan2(p1.y - v.y, p1.x - v.x);
    angvp2 = atan2(p2.y - v.y, p2.x - v.x);
    return AngCounterClock(angvu, angvp1) < AngCounterClock(angvp2, angvp1) - eps;
}
```

### 简单多边形、凸多边形面积并

```cpp
// 不需要正规的三角剖分，用求多边形面积的思想，从一点出发连接多边形的边得到很多三
// 角形，三角形有向边方向决定有向面积有正有负，相加得到多边形面积的正值或负值。
// 把两个多边形都分成若干这样的三角形，求每对三角形的交，根据两三角形有向边顺逆时
// 针关系确定相交面积的正负号，最后两多边形面积和减去相交面积。 
double CPIA(Point a[], Point b[], int na, int nb)
// ConvexPolygonIntersectArea
{
    Point p[maxisn], tmp[maxisn];
    int i, j, tn, sflag, eflag;
    a[na] = a[0], b[nb] = b[0];
    memcpy(p, b, sizeof(Point) * (nb + 1));
    for(i = 0; i < na && nb > 2; ++ i)
    {
        sflag = dcmp(a[i].cross(a[i + 1], p[0]));
        for(j = tn = 0; j < nb; ++ j, sflag = eflag)
        {
            if(sflag >= 0) tmp[tn ++] = p[j];
            eflag = dcmp(a[i].cross(a[i + 1], p[j + 1]));
            if((sflag ^ eflag) == -2)
                tmp[tn ++] = LineCross(a[i], a[i + 1], p[j], p[j + 1]);
        }
        memcpy(p, tmp, sizeof(Point) * tn);
        nb = tn, p[nb] = p[0];
    }
    if(nb < 3) return 0.0;
    return PolygonArea(p, nb);
}
double SPIA(Point a[], Point b[], int na, int nb)
// SimplePolygonIntersectArea，要调用CPIA
// 一般两个都写上，只调用SPIA
{
    int i, j;
    Point t1[4], t2[4];
    double res = 0, if_clock_t1, if_clock_t2;
    a[na] = t1[0] = a[0], b[nb] = t2[0] = b[0];
    for(i = 2; i < na; ++ i)
    {
        t1[1] = a[i - 1], t1[2] = a[i];
        if_clock_t1 = dcmp(t1[0].cross(t1[1], t1[2]));
        if(if_clock_t1 < -eps) std::swap(t1[1], t1[2]);
        for(j = 2; j < nb; ++ j)
        {
            t2[1] = b[j - 1], t2[2] = b[j];
            if_clock_t2 = dcmp(t2[0].cross(t2[1], t2[2]));
            if(if_clock_t2 < -eps) std::swap(t2[1], t2[2]);
            res += CPIA(t1, t2, 3, 3) * if_clock_t1 * if_clock_t2;
        }
    }
    return PolygonArea(a, na) + PolygonArea(b, nb) - res;
    // res是面积交，CPIA可同理求交
}
```

### 简单多边形与圆面积交

```cpp
// 要用到Sqr(double x)，同时重载了Sqr(Point p)，两个都要。
inline double Sqr(const Point &p) {return p.dot(p);}
double LineCrossCircle(const Point &a, const Point &b, const Point &r, double R, Point &p1, Point &p2)
{
    Point fp = LineCross(r, Point(r.x + a.y - b.y, r.y + b.x - a.x), a, b);
    double rtol = r.Dis(fp), rtos = fp.OnSeg(a, b) ? rtol : min(r.Dis(a), r.Dis(b)), atob = a.Dis(b);
    double fptoe = sqrt(R * R - rtol * rtol) / atob;
    if(rtos > R - eps) return rtos;
    p1 = fp + (a - b) * fptoe;
    p2 = fp + (b - a) * fptoe;
    return rtos;
}
double SectorArea(const Point &r, const Point &a, const Point &b, double R) //不大于180度扇形面积，r->a->b逆时针
{
    double A2 = Sqr(r - a), B2 = Sqr(r - b), C2 = Sqr(a - b);
    return R * R * acos((A2 + B2 - C2) * 0.5 / sqrt(A2) / sqrt(B2)) * 0.5;
}
double TACIA(const Point &r, const Point &a, const Point &b, double R) //TriangleAndCircleIntersectArea，逆时针，p[0]为圆心
{
    double adis = r.Dis(a), bdis = r.Dis(b);
    if(adis < R + eps && bdis < R + eps) return r.cross(a, b) * 0.5;
    Point ta, tb;
    if(r.InLine(a, b)) return 0.0;
    double rtos = LineCrossCircle(a, b, r, R, ta, tb);
    if(rtos > R - eps) return SectorArea(r, a, b, R);
    if(adis < R + eps) return r.cross(a, tb) * 0.5 + SectorArea(r, tb, b, R);
    if(bdis < R + eps) return r.cross(ta, b) * 0.5 + SectorArea(r, a, ta, R);
    return r.cross(ta, tb) * 0.5 + SectorArea(r, a, ta, R) + SectorArea(r, tb, b, R);
}
double SPICA(Point p[], int n, Point r, double R)
// SimplePolygonIntersectCircleArea
// 一般两个都写上，只调用SPICA
{
    int i;
    Point a, b;
    double res = 0, if_clock_t;
    p[n] = p[0];
    for(i = 1; i <= n; ++ i)
    {
        a = p[i - 1], b = p[i];
        if_clock_t = dcmp(r.cross(a, b));
        if(if_clock_t < 0) std::swap(a, b);
        res += TACIA(r, a, b, R) * if_clock_t;
    }
    return fabs(res);
}

```

### 平面最近点对距离

```cpp
Point p[maxn], lp[maxn];
double ClosestPoints(Point p[], int l, int r) //[l, r)
// x有序，y无序 
{
    if(r - l <= 1) return inf;
    int mid = l + r >> 1;
    double mx = p[mid].x;
    double ms = min(ClosestPoints(p, l, mid), ClosestPoints(p, mid, r));
     //先递归，相当于对y的归并排序，这样对x是稳定的。 
    int L, R, i, j, k;
    for(i = L = l, R = mid; i < r; i ++)  //取中心左右x距离在ms内的区间 
    {
        if(i < mid && mx - p[i].x <= ms) lp[L ++] = p[i];
        else if(i >= mid && p[i].x - mx <= ms) lp[R ++] = p[i];
    }
    for(i = l, j = mid; i < L && j < R; )
     //可证明对于左侧一个点，右侧y距离在ms以内的至多6个点，反之亦然 
    {
        if(lp[i].y - lp[j].y > ms) {j ++; continue;}
        if(lp[j].y - lp[i].y > ms) {i ++; continue;}
        for(k = i; k < L && fabs(lp[k].y - lp[j].y <= ms); k ++)
            ms = min(lp[k].Dis(lp[j]), ms);
        for(k = j; k < R && fabs(lp[k].y - lp[i].y <= ms); k ++)
            ms = min(lp[k].Dis(lp[i]), ms);
        i ++, j ++;
    }
    for(i = k = l, j = mid; i < mid && j < r;) //以下是对y的归并排序，很普通的归并。 
    {
        if(p[i].y > p[j].y) lp[k ++] = p[j ++];
        else lp[k ++] = p[i ++];
    }
    while(i < mid) lp[k ++] = p[i ++];
    while(j < r) lp[k ++] = p[j ++];
    memcpy(p + l, lp + l, sizeof(Point) * (r - l));
    return ms;
}

```

### 最小圆覆盖

```cpp
// 覆盖散点集的半径最小的圆
Point CC(Point &a, Point &b, Point &c)    //外接圆
{
    Point ab = (a + b) * 0.5, ac = (a + c) * 0.5;
    return LineCross(ab, Point(ab.x + a.y - b.y, ab.y + b.x - a.x), 
                    ac, Point(ac.x + a.y - c.y, ac.y + c.x - a.x));
}
// 随机化版
struct RP
{
    int rdnum;
    int i;
    bool operator<(const RP &b)const
    {return rdnum < b.rdnum;}
}rp[maxn];
#define P(x) (p[rp[x].i])
double SC(Point p[], int n, Point &r) //SmallestCircle
{
    int i, j, k;
    for(i = 0; i < n; ++ i) rp[i].i = i, rp[i].rdnum = rand();
    sort(rp, rp + n);
    r = P(0);
    double R = 0;
    for(i = 1; i < n; i ++)
        if(P(i).Dis(r) > R)
        {
            r = P(i);
            R = 0;
            for(j = 0; j < i; j ++)
                if(P(j).Dis(r) > R)
                {
                    r = (P(i) + P(j)) * 0.5;
                    R = P(i).Dis(P(j)) * 0.5;
                    for(k = 0; k < j; k ++)
                        if(P(k).Dis(r) > R)
                        {
                            r = CC(P(i), P(j), P(k));
                            R = P(i).Dis(r);
                        }
                }
        }
    return R;
}
```

### 非随机化版

```cpp
double SC(Point p[], int n, Point &r) //SmallestCircle
{
    int i, j, k;
    r = p[0];
    double R = 0;
    for(i = 1; i < n; i ++)
        if(p[i].Dis(r) > R)
        {
            r = p[i];
            R = 0;
            for(j = 0; j < i; j ++)
                if(p[j].Dis(r) > R)
                {
                    r = (p[i] + p[j]) * 0.5;
                    R = p[i].Dis(p[j]) * 0.5;
                    for(k = 0; k < j; k ++)
                        if(p[k].Dis(r) > R)
                        {
                            r = CC(p[i], p[j], p[k]);
                            R = p[i].Dis(r);
                        }
                }
        }
    return R;
}
```

### 求一系列不相切不相交的圆最深嵌套

```cpp
int LineNow, ltp, n, cnt[maxn];
struct Cir //圆
{
    int x;
    int y;
    int r;
}c[maxn];
struct Line //从左向右扫描节点
{
    int id;
    bool in;
    void Read(int id_, bool in_){id = id_, in = in_;}
    inline int GetSite()const{return c[id].x + (in ? -c[id].r : c[id].r);}
    bool operator<(const Line &b)const{return GetSite() < b.GetSite();}
}l[maxn << 1];
struct Node //从上至下排序节点
{
    int id;
    bool up;
    Node(){}
    Node(int id_, bool up_){id = id_, up = up_;}
    bool operator<(const Node &b)const
    {
        double y1 = c[id].y + sqrt(Sqr(c[id].r) - Sqr(LineNow - c[id].x)) * (up ? 1 : -1);
        double y2 = c[b.id].y + sqrt(Sqr(c[b.id].r) - Sqr(LineNow - c[b.id].x)) * (b.up ? 1 : -1);
        return dcmp(y1 - y2) ? y1 > y2 : up > b.up;
    }
};
set<Node> s;
set<Node>::iterator iti, itn;
void ReadData(int n)
{
    int i;
    for(ltp = i = 0; i < n; ++ i)
    {
        scanf("%d%d%d", &c[i].x, &c[i].y, &c[i].r);
        l[ltp ++].Read(i, true);
        l[ltp ++].Read(i, false);
    }
}
int MakeAns()
{
    int i, ans = 0;
    sort(l, l + ltp);
    s.clear();
    for(i = 0; i < ltp; ++ i)
    {
        LineNow = l[i].GetSite();
        if(!l[i].in)
        {
            s.erase(Node(l[i].id, true));
            s.erase(Node(l[i].id, false));
        }
        else
        {
            iti = itn = s.insert(Node(l[i].id, true)).first;
            itn ++;
            if(iti == s.begin() || itn == s.end()) cnt[l[i].id] = 1;
            else
            {
                iti --;
                if((*iti).id == (*itn).id) cnt[l[i].id] = cnt[(*iti).id] + 1;
                else cnt[l[i].id] = max(cnt[(*iti).id], cnt[(*itn).id]);
            }
            ans = max(ans, cnt[l[i].id]);
            s.insert(Node(l[i].id, false));
        }
    }
    return ans;
}
```

### 角度区间模版

```cpp
// atan2  (-pi,pi]转区间覆盖处理
int ctp;
struct Cov
{
    double site;
    int se;
    bool operator<(const Cov &b)const
    {
        if(!dcmp(site - b.site)) return se > b.se;
        return site < b.site;
    }
} cover[maxn <<2];
void AngManage(double &x) //角度区间修正，(-pi, pi]
{
    while(x + pi < eps) x += 2 * pi;
    while(x - pi > eps) x -= 2 * pi;
}
void AddAnSeg(double start, double end) //圆心角转区间
{
    AngManage(start), AngManage(end);
    if(start - end > eps) AddAnSeg(start, pi), AddAnSeg(-pi + eps * 2, end);
    else
    {
        cover[ctp].site = start, cover[ctp].se = 1;++ ctp;
        cover[ctp].site = end, cover[ctp].se = -1;++ ctp;
    }
}
int SumCov(Cov cover[], int ctp)
{
    int i, cnt, ans;
    for(i = cnt = ans = 0; i < ctp; ++ i)
    {
        cnt += cover[i].se;
        ans = max(ans, cnt);
    }
    return ans;
}

```

### 判断圆i在圆j中（包括内切）

```cpp
inline bool IinJ(int i, int j, double ijdis)
{return dcmp(ra[i].r + ijdis - ra[j].r) <= 0;}
```

### 判断圆i与圆j相交（包括外切）

```cpp
inline bool IcutJ(int i, int j, double ijdis)
{return dcmp(ijdis - ra[i].r + ra[j].r) <= 0;}
void CalCirCutCir(int i, int j)
// 左圆i与右圆j的公切线，计算的反正切皆为-pi~pi。
{
    double ijdis = CalDis(ra[i].x - ra[j].x, ra[i].y - ra[j].y);
    double xlij = atan2(ra[i].y - ra[j].y, ra[i].x - ra[j].x);
    // 右心->左心反正切
    double xlji = atan2(ra[j].y - ra[i].y, ra[j].x - ra[i].x);
    // 左心->右心反正切
    double asimj = asin((ra[i].r - ra[j].r) / ijdis);
    double asipj = asin((ra[i].r + ra[j].r) / ijdis);
    double at1 = AngManage(xlji - asipj), at2 = AngManage(xlji - asimj);
    //左上->右下切线的反正切、左上->右上切线的反正切
    double at3 = AngManage(xlji + asipj), at4 = AngManage(xlji + asimj);
    //左下->右上切线的反正切、左下->右下切线的反正切
    //ji即左->右，+pj即下->上，+mj即下->下，反之则反。 Manage(at1), Manage(at2);
    //以第一象限左右为例推出计算切点切线的方法，可以推广到其他方位和象限。
}
```

### 圆外点到圆切线

```cpp
void TLTP(Point p, Point r, double R, Point &p1, Point &p2)
// p->p1 p->p2为切线向量，但非切点。p1->p2逆时针，绕点旋转法
{
    double ang = asin(R / p.Dis(r));

    if(!dcmp(R)) {p1 = p2 = r; return;}
    p1 = p.RotePoint(r, 2 * pi - ang);
    p2 = p.RotePoint(r, ang);
}
/*
void TLTP(Point p, Point r, double R, Point &p1, Point &p2)
// 圆外点到圆切线，p1~p2逆时针，定比分点法，p1、p2为切点
{
    double tc = Sqr(R) / p.Dis(r);
    Point tmp = r + (p - r) * (tc / p.Dis(r));
    Point tx(tmp.x + p.y - r.y, tmp.y + r.x - p.x);
    double lin = (sqrt(Sqr(R) - Sqr(tc)) / tx.Dis(tmp));
    p1 = tmp - (tx - tmp) * lin;
    p2 = tmp + (tx - tmp) * lin;
}
*/
```

### 两圆面积交

```cpp
// 其实就是余弦定理
double CircleInsectArea(Point a, double Ra, Point b, double Rb)
{
    double dis = a.Dis(b);
    if(dis > Ra + Rb - eps) return 0;
    if(Ra > Rb + dis - eps) return pi * Sqr(Rb);
    if(Rb > Ra + dis - eps) return pi * Sqr(Ra);
    double coa = acos((Sqr(dis) + Sqr(Ra) - Sqr(Rb)) * 0.5 / Ra / dis);
    double cob = acos((Sqr(dis) + Sqr(Rb) - Sqr(Ra)) * 0.5 / Rb / dis);
    return Sqr(Ra) * coa + Sqr(Rb) * cob - sin(pi - coa - cob) * Ra * Rb;
}

```

### 皮克定理

```cpp
// 给定顶点座标均是整点（或正方形格点）的简单多边形，皮克定理说明了其面积
// area和内部格点数目 I 、边上格点数目 cnt 的关系：area = I + cnt/2 - 1 。
```

### 模拟退火样例

```cpp
// 范围内距离点集最近距离最远点
// 模拟退火基本都是这样的格式，清楚原理之后可自由发挥
const int maxsam = 20;       //采样点个数
const int faillimit = 20;    //测试次数限制
const double leps = 1e-2;     //步长限制
const double depace = 0.9;   //步长缩减率
Point sam[maxsam];           //采样点
void SA()
{
    int i, j;
    double pace; //初始步长要足够大 
    Point tmp;
    for(i = 0; i < maxsam; ++ i)
    {
        sam[i].x = rand() / 32767.0 * X;
        sam[i].y = rand() / 32767.0 * Y;
        sam[i].mindis = CalMinDis(sam[i]);
    }
    for(pace = sqrt((double)X * Y); pace > leps; pace *= depace) //缩减步长 
        for(i = 0; i < maxsam; ++ i) //枚举采样点 
            for(j = 0; j < faillimit; ++ j) //采样点扩展 
            {
                tmp.x = sam[i].x + cos((double)rand()) * pace;
                tmp.y = sam[i].y + cos((double)rand()) * pace;
                if(tmp.x <= X && tmp.y <= Y && tmp.x >= 0 && tmp.y >= 0)
                {
                    tmp.mindis = CalMinDis(tmp);
                    if(tmp.mindis > sam[i].mindis/* || rand() / 30.0 < exp((tmp.mindis - sam[i].mindis) / pace)*/)
                        sam[i] = tmp;
                }
            }
    for(i = j = 0; i < maxsam; ++ i)
        if(sam[i].mindis > sam[j].mindis) j = i;
    printf("The safest point is (%.1f, %.1f).\n", sam[j].x, sam[j].y);
}
 
```

## 三维几何

### 定义三维点

```cpp
struct Point3
{
    double x, y, z;
    Point3() {x = y = z = 0;}
    Point3(double a, double b, double c){x = a, y = b, z = c;}

    Point3 antop(double lat, double lng, double R) //经纬度转三维坐标点
    {
        lat += 0.5 * pi;
        return Point3(R * sin(lat) * cos(lng), R * sin(lat) * sin(lng), R * cos(lat));
    }
    Point3 cross(Point3 p)
    {
        return Point3(y * p.z - p.y * z,
                      z * p.x - x * p.z,
                      x * p.y - y * p.x);
}

double dot(Point3 p){return x * p.x + y * p.y + z * p.z;}

Point3 operator-(const Point3 &p)const
{return Point3(x - p.x, y - p.y, z - p.z);}

Point3 operator-()const
{return Point3(-x, -y, -z);}

Point3 operator+(const Point3 &p)const
{return Point3(x + p.x, y + p.y, z + p.z);}

Point3 operator*(const double b)const
{return Point3(x * b, y * b, z * b);}

bool operator==(const Point3 &p)const
{return !dcmp(x - p.x) && !dcmp(y - p.y) && !dcmp(z - p.z);}

bool operator!=(const Point3 &p)const
{return !(*this == p);}

    bool operator<(const Point3 &p)const
    {
        if(!dcmp(x - p.x))
        {
            if(!dcmp(y - p.y))
                return dcmp(z - p.z) < 0;
            return dcmp(y - p.y) < 0;
        }
        return dcmp(x - p.x) < 0;
}

    bool operator>(const Point3 &p)const{return p < *this;}
    bool operator>=(const Point3 &p)const{return !(*this < p);}
    bool operator<=(const Point3 &p)const{return !(*this > p);}

    // 法向量
    Point3 fxl(Point3 b, Point3 c){return (b - *this).cross(c - b);}
    // 两点距离
    double Dis(Point3 b){return sqrt((*this - b).dot(*this - b));}
    // 向量绝对长度
    double vlen(){return sqrt(dot(*this));}

    bool PinLine(Point3 b, Point3 c) //三点共线
    {return fxl(b, c).vlen() < eps;}
    bool PonPlane(Point3 b, Point3 c, Point3 d) //四点共面
    {return !dcmp(fxl(b, c).dot(d - *this));}
    bool PonSeg(Point3 b, Point3 c) //点在线段上，包括端点
    {
        return !dcmp((*this - b).cross(*this - c).vlen()) &&
               (*this - b).dot(*this - c) <= 0;
    }
    bool PinSeg(Point3 b, Point3 c) //点在线段上，不包括端点
    {return PonSeg(b, c) && *this != b && *this != c;}
    double PtoLine(Point3 b, Point3 c) //点到直线距离
    {return (*this - b).cross(c - b).vlen() / b.Dis(c);}
    double PtoPlane(Point3 b, Point3 c, Point3 d) //点到平面距离
    {return fabs(b.fxl(c, d).dot(*this - b)) / b.fxl(c, d).vlen();}
};

```

### 面面交线

```cpp
bool InterLine(Plane3 u, Plane3 v, Line &l)
{
    if(u.Parallel(v)) return false;
    Line ret;
    ret.a = u.Paral(v.a, v.b) ? u.PcrossPlane(v.b, v.c) : u.PcrossPlane(v.a, v.b);
    ret.b = u.Paral(v.c, v.a) ? u.PcrossPlane(v.b, v.c) : u.PcrossPlane(v.c, v.a);
    l = ret;
    return true;
}
```

### 三维凸包

```cpp
// 需要多个凸包时，可以把三维凸包相关变量函数封装为类，vector代替数组。
int be[maxn][maxn], m;
struct Plane
{
    int a, b, c, del;
    Point3 fxl; //法向量
    Plane(){}
    Plane(int a_, int b_, int c_)
    {
        a = a_, b = b_, c = c_, del = 0;
        fxl = p[a].fxl(p[b], p[c]);
    }
}f[maxn * 100];

bool PisOut(int a, int b)  //p[a]在f[b]外
{
    double s = f[b].fxl.dot(p[a] - p[f[b].a]);
    return s < -eps;
}
void AddF(int a, int b, int c) //增加面
{
    be[a][b] = be[b][c] = be[c][a] = ++ m;
    f[m] = Plane(a, b, c);
}
double CalVal(Point3 a, Point3 b, Point3 c, Point3 d) //体积
{
    Point3 A, B, C;
    A = b - a, B = c - a, C = d - a;
    return fabs(A.cross(B).dot(C) / 6);
}
void InitC(int x1, int x2, int x3, int x4) //凸包初始化
{
    f[0] = Plane(x1, x2, x3);
    if(PisOut(x4, 0)) swap(f[0].a, f[0].b);
    AddF(f[0].a, f[0].b, f[0].c);
}
void Cal3DConvex(int n) //三维凸包，f[i].del表示是否是已删除面
{
    m = 0;
    memset(be, 0, sizeof(be));
    int pos = 3;
    while(CalVal(p[0], p[1], p[2], p[3]) < eps)
        ++ pos, swap(p[pos], p[3]);
    InitC(0, 1, 2, 3), InitC(0, 1, 3, 2);
    InitC(0, 2, 3, 1), InitC(1, 2, 3, 0);
    for(int i = 4; i < n; ++ i)
    {
        for(int j = 1; j <= m; ++ j)
            if(!f[j].del && PisOut(i, j))
                f[j].del = 1;
        for(int j = 1; j <= m; ++ j)
            if(f[j].del == 1)
            {
                int a = f[j].a, b = f[j].b, c = f[j].c;
                if(!f[be[b][a]].del) AddF(a, b, i);
                if(!f[be[c][b]].del) AddF(b, c, i);
                if(!f[be[a][c]].del) AddF(c, a, i);
                f[j].del = -1;
            }
    }
}
```

### 计算三维凸包重心

```cpp
Point3 CalCenter(int n)  
{
    Point3 dtmp = p[0], zc, center;
    double v = 0, vt;
    center = Point3(0, 0, 0);
    for(int i = 1; i <= m; ++ i)
        if(!f[i].del)
        {
            zc = (dtmp + p[f[i].a] + p[f[i].b] + p[f[i].c]) * 0.25;
            vt = CalVal(dtmp, p[f[i].a], p[f[i].b], p[f[i].c]);
            center = center + zc * vt;
            v += vt;
        }
    center = center * (1 / v);
    return center;
}
```

### 三维坐标平移、旋转、倍增变换

```cpp
// Matrix结构体中进行每种类型操作的成员函数都返回一个Matrix值，并不改变自身。
// x、y、z分别为Mt[0][0]、Mt[0][1]、Mt[0][2]，运算前Mt[0][3]置1. 
const double pi = acos(-1.0);
const double eps = 1e-6;
inline double pz(double x)
{return fabs(x) < eps ? 0.0 : x;}
struct Matrix
{
    double Mt[4][4];
    void init0(){memset(Mt, 0, sizeof(Mt));} //初始化0
    void init1() //初始化单位阵
    {init0(), Mt[0][0] = Mt[1][1] = Mt[2][2] = Mt[3][3] = 1;}
    Matrix(){init0();} //默认初始化0
    Matrix(int num) //按数值初始化为对角阵:Matrix a(1);
    {init0();Mt[0][0] = Mt[1][1] = Mt[2][2] = Mt[3][3] = num;}    
    Matrix operator *(const Matrix &b) //重载乘法
    {
        int i, j, k;Matrix res;
        for(i = 0; i < 4; ++ i)
            for(j = 0; j < 4; ++ j)
                for(k = 0; k < 4; ++ k)
                    res.Mt[i][j] += Mt[i][k] * b.Mt[k][j];
        return res;
    }
    void read(double x, double y, double z) //赋值
    {init0(), Mt[0][0] = x, Mt[0][1] =y, Mt[0][2] = z, Mt[0][3] = 1;}
    void scan()     //输入
    {double x, y, z;scanf("%lf%lf%lf", &x, &y, &z);read(x, y, z);}
    void prin()     //输出
    {printf("%.2f %.2f %.2f\n", pz(Mt[0][0]), pz(Mt[0][1]), pz(Mt[0][2]));}
    Matrix Trans(double x, double y, double z) //坐标平移
    {
        Matrix b(1);
        b.Mt[3][0] = x, b.Mt[3][1] = y, b.Mt[3][2] = z;
        return *this * b;
    }
    Matrix Scale(double x, double y, double z) //坐标加倍
    {
        Matrix b(1);
        b.Mt[0][0] = x, b.Mt[1][1] = y, b.Mt[2][2] = z;
        return *this * b;
    }
    Matrix Rotate(double x, double y, double z, double deg)
     //绕轴旋转，从x、y、z看向原点逆时针，deg为弧度
    {
        Matrix b(1);
        double len = sqrt(x * x + y * y + z * z);
        double co = cos(deg), si = sin(deg);
        x /= len, y /= len, z /= len;  //归一化处理
        b.Mt[0][0] = (1 - co) * x * x + co;
        b.Mt[0][1] = (1 - co) * x * y + si * z;
        b.Mt[0][2] = (1 - co) * x * z - si * y;
        b.Mt[1][0] = (1 - co) * y * x - si * z;
        b.Mt[1][1] = (1 - co) * y * y + co;
        b.Mt[1][2] = (1 - co) * y * z + si * x;
        b.Mt[2][0] = (1 - co) * z * x + si * y;
        b.Mt[2][1] = (1 - co) * z * y - si * x;
        b.Mt[2][2] = (1 - co) * z * z + co;
        return *this * b;
    }
    Matrix Rep(int p) //矩阵连乘快速p次幂 
    {
        Matrix b = *this, res(1);
        if(p == 0) return res;
        if(p == 1) return b;
        while(p > 1)
        {
            if(p & 1) res = res * b;
            b = b * b;
            p >>= 1;
        }
        return b * res;
    }
};
```

### 分数表示的点线距离

```cpp
struct Point3     //三维坐标点 
{
    Point3(){x = y = z = 0;}
    Point3(LL a, LL b, LL c)
    {x = a, y = b, z = c;}
    Point3 operator*(const Point3 &p)const //叉积 
    {
        return Point3(y * p.z - z * p.y,
                    z * p.x - x * p.z,
                    x * p.y - y * p.x);
    }
    Point3 operator-(const Point3 &p)const
    {return Point3(x - p.x, y - p.y, z - p.z);}
    Point3 operator+(const Point3 &p)const
    {return Point3(x + p.x, y + p.y, z + p.z);}
    Point3 operator-()const
    {return Point3(-x, -y, -z);}
    LL dot(const Point3 &p)const     //点积 
    {return x * p.x + y * p.y + z * p.z;}
    LL x, y, z;
};
struct Dis                 //能用分数表示距离的距离结构体 
{
    LL fz, fm; //分子分母 
    void yf()
    {
        if(fz == 0) {fm = 1;return;}
        LL t = gcd(fz, fm);
        fz /= t;
        fm /= t;
    }
    bool operator<(const Dis &p)const
    {return fz * p.fm < p.fz * fm;}
};
bool Paral(Point3 a, Point3 b, Point3 c, Point3 d) //判平行 
{
    Point3 tmp = (b - a) * (d - c);
    return !tmp.dot(tmp);
}
bool JudgeCZ(Point3 a, Point3 b, Point3 c, Point3 d)
// 判垂足是否在线段上，点到线的情况传入两相同点即可，否则为线段公垂线情况 
{
    LL A1, B1, C1, A2, B2, C2;
    A1 = (b - a).dot(b - a);
    B1 = -(d - c).dot(b - a);
    C1 = -(a - c).dot(b - a);
    A2 = (b - a).dot(d - c);
    B2 = -(d - c).dot(d - c);
    C2 = -(a - c).dot(d - c);
    double bl1 = dcmp(A2 * B1 - A1 * B2) ? ((double)C2 * B1 - C1 * B2) / (A2 * B1 - A1 * B2) : (A1 ? (double)C1 / A1 :(A2 ? (double)C2 / A2 : 0));
    double bl2 = dcmp(B2 * A1 - B1 * A2) ? ((double)C2 * A1 - C1 * A2) / (B2 * A1 - B1 * A2) : (B1 ? (double)C1 / B1 :(B2 ? (double)C2 / B2 : 0));
    return bl1 > -eps && bl1 < 1 + eps && bl2 > -eps && bl2 < 1 + eps;
}
Dis CalPtoL(Point3 p, Point3 a, Point3 b) //点线距离 
{
    Point3 t = (a - p) * (b - a);
    Dis tmp;
    tmp.fz = t.dot(t);
    tmp.fm = (b - a).dot(b - a);
    tmp.yf();
    return tmp;
}
Dis CalPtoP(Point3 a, Point3 b) //两点距离 
{
    Dis tmp;
    tmp.fz = (b - a).dot(b - a);
    tmp.fm = 1;
    tmp.yf();
    return tmp;
}
Dis CalLtoL(Point a, Point b, Point c, Point d) //两线距离 
{
    // if(!Paral(a, b, c, d) && JudgeCZ(a, b, c, d))
    Point3 t = (b - a) * (d - c);
    ans.fz = Sqr((c - a).dot(t));
    ans.fm = t.dot(t);
    ans.yf();
    return ans;
}
```
