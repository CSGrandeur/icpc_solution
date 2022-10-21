// difficulty: 2
// [SCOI2009]生日快乐
// 每块面积相等，N只有10，于是可以枚举每刀横切竖切以及左右两边各几块来回溯
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<algorithm>
double DFS(double x, double y, int n)
{
    if(x < y) std::swap(x, y);
    if(n == 1) return x / y;
    double ret = 1e12;
    for(int i = 1; i <= n >> 1; i ++)
    {
        double ar = x * y / n * i;
        double nx = ar / y, ny = ar / x;
        ret = std::min(ret, std::max(DFS(nx, y, i), DFS(x - nx, y, n - i)));
        ret = std::min(ret, std::max(DFS(x, ny, i), DFS(x, y - ny, n - i)));
    }
    return ret;
}
int main()
{
    int n;
    double x, y;
    while(scanf("%lf%lf%d", &x, &y, &n) != EOF)
        printf("%.6f\n", DFS(x, y, n));
    return 0;
}