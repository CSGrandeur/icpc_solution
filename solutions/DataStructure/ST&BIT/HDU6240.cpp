// difficulty: 4
// Server
// 01分数规划：设Sum(ai) / Sum(bi)  的结果为 x，则 F(x) = Sum(ai) - x * Sum(bi) == 0
// 二分枚举 x， F(x) < 0 则 right = mid， 否则 left = mid
// 二分时，给定一个 x（mid），所有 ai - x * bi <= 0 的线段都用上
// 对 ai - x * bi > 0 的线段，利用树状数组更新保存 Ti 位置最优解
// 在树状数组中查询 >= Si-1 后缀最优解做状态转移。

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
const double dmin(double a, double b) {return  a < b ? a : b;}
const int maxn = 1e5 + 10;
struct Node
{
    int S, T, a, b;
    bool operator<(const Node &oth)const
    {
        return S < oth.S;
    }
};
int t, n, dayNum;
double di[maxn];
Node nd[maxn];

void Update(int x, double v)
{
    // 更新前缀最优解
    for(; x; x -= x & -x)
        di[x] = dmin(di[x], v);
}
double Search(int x)
{
    // 搜索后缀最优解
    if(x == 0) return 0;
    double res = 1e12;
    for(; x <= dayNum; x += x & -x)
        res = dmin(res, di[x]);
    return res;
}
bool BiTry(double mid)
{
    std::fill_n(di, dayNum + 1, 1e12);
    double sureCost = 0;
    for(int i = 0; i < n; i ++)
    {
        double cost = nd[i].a - mid * nd[i].b;
        if(cost < 0) 
        {
            sureCost -= cost;
            cost = 0;
        }
        Update(nd[i].T, Search(nd[i].S - 1) + cost); 
    }
    return Search(dayNum) < sureCost + 1e-6;
}
int main()
{
    for(scanf("%d", &t); t --; )
    {
        scanf("%d%d", &n, &dayNum);
        for(int i = 0; i < n; i ++)
            scanf("%d%d%d%d", &nd[i].S, &nd[i].T, &nd[i].a, &nd[i].b);
        std::sort(nd, nd + n);
        double left = 0, right = 1e5+10;
        while(right - left > 1e-5)
        {
            double mid = (left + right) * 0.5;
            if(BiTry(mid))
                right = mid;
            else
                left = mid;
        }
        printf("%.3f\n", left);
    }
    return 0;
}