// difficulty: 2
// 共线点
// 旋转卡壳启蒙。如果一条线穿过三个线段，则这条线通过调整一定可以靠到两条线段的端点。
// 枚举两条线段的端点组合连线，判断与第三条线段的交点是否在第三条线段内。
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<algorithm>
const double eps = 1e-8;
int dcmp(double x) {return (x > eps) - (x < -eps);}

struct Point
{
    double x, y;
    Point(){x = y = 0;}
    Point(double a, double b) {x = a, y = b;}
    inline Point operator-(const Point &b)const 
    {return Point(x - b.x, y - b.y);}
    inline double dot(const Point &b)const
    {return x * b.x + y * b.y;}
    inline double cross(const Point &b, const Point &c)const
    {return (b.x - x) * (c.y - y) - (c.x - x) * (b.y - y);}
    inline bool OnSeg(const Point &b, const Point &c)const //点在线段上，包括端点
    {return (*this - c).dot(*this - b) < eps;}
    inline Point operator+(const Point &b)const
    {return Point(x + b.x, y + b.y);}
};
Point LineCross(Point a, Point b, Point c, Point d)
{
    double u = a.cross(b, c), v = b.cross(a, d);
    return Point((c.x * v + d.x * u) / (u + v), (c.y * v + d.y * u) / (u + v));
}
Point p[3][2];
bool Judge()
{
    for(int i = 0; i < 3; i ++)
    for(int j = i + 1; j < 3; j ++)
    for(int k = 0; k < 3; k ++)
    {
        if(k == i || k == j) continue;
        for(int ii = 0; ii < 2; ii ++)
        for(int jj = 0; jj < 2; jj ++)
        {
            Point lc = LineCross(p[i][ii], p[j][jj], p[k][0], p[k][1]);
            if(lc.OnSeg(p[k][0], p[k][1])) return true;
        }
    }
    return false;
}
int main()
{
    int a, b, y;
    while(scanf("%d%d%d", &a, &b, &y) != EOF)
    {
        for(int i = 0; i < 3; i ++)
        {
            p[i][0] = Point(a, y);
            p[i][1] = Point(b, y);
            if(i < 2) scanf("%d%d%d", &a, &b, &y);
        }
        printf(Judge() ? "Yes\n" : "No\n");
    }
    return 0;
}