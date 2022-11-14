### `Mr. Main and Windmills`

> difficulty: 2

几何水题，求直线与线段交点排个序

此题坑点在精度，如果计算方式不当可能损失精度而WA

```cpp

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<algorithm>
#include<vector>
#include<functional>
const int maxn = 1100;
const double eps = 1e-8;
const double pi = acos(-1.0);
int dcmp(double x) {return (x > eps) - (x < -eps);}
struct Point
{
    double x, y;
    Point(){x = y = 0;}
    Point(double a, double b) {x = a, y = b;}
    inline Point operator-(const Point &b)const 
    {return Point(x - b.x, y - b.y);}
    inline Point operator+(const Point &b)const
    {return Point(x + b.x, y + b.y);}
    inline double dot(const Point &b)const //点积
    {return x * b.x + y * b.y;}
    inline double Dis(const Point &b)const //距离
    {return sqrt((*this - b).dot(*this - b));}
    inline double cross(const Point &b, const Point &c)const
    {return (b.x - x) * (c.y - y) - (c.x - x) * (b.y - y);}
};
bool Parallel(Point a, Point b, Point c, Point d)
{return !dcmp(a.cross(b, a + d - c));} 
bool LineInterSeg(const Point a, Point b, Point c, Point d)
{return !Parallel(a, b, c, d) &&  a.cross(b, c) * a.cross(b, d) < eps;}
Point LineCross(Point a, Point b, Point c, Point d)
{
    double u = a.cross(b, c), v = b.cross(a, d);
    return Point((c.x * v + d.x * u) / (u + v), (c.y * v + d.y * u) / (u + v));
}
int n, m, hth, kth;
Point p[maxn];
Point ps, pe;
std::vector<Point> allIs[maxn];
int comp(const Point &a, const Point &b)
{return a.Dis(ps) < b.Dis(ps);}
int main()
{
    while(scanf("%d%d", &n, &m) != EOF)
    {
        scanf("%lf%lf%lf%lf", &ps.x, &ps.y, &pe.x, &pe.y);
        for(int i = 1; i <= n; i ++)
        {
            allIs[i].clear();
            scanf("%lf%lf", &p[i].x, &p[i].y);
            for(int j = 1; j < i; j ++)
            {
                if(!LineInterSeg(p[i], p[j], ps, pe))
                    continue;
                Point is = LineCross(p[i], p[j], ps, pe);
                allIs[i].push_back(is);
                allIs[j].push_back(is);
            }
        }
        for(int i = 1; i <= n; i ++)
            std::sort(allIs[i].begin(), allIs[i].end(), comp);
        while(m --)
        {
            scanf("%d%d", &hth, &kth);
            if(kth <= allIs[hth].size())
                printf("%f %f\n", allIs[hth][kth - 1].x, allIs[hth][kth - 1].y);
            else
                printf("-1\n");
        }
    }
    return 0;
}
```

