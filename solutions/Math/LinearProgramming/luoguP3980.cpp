// difficulty: 3
// [NOI2008] 志愿者招募
// 单纯形法解线性规划入门，直观思路是求最小花费，约束条件是每类志愿者人数 x 为变量，保证每天志愿者人数大于等于要求
// 但用这个化成标准型后，会加入"剩余变量"，对应剩余变量稀疏矩阵得到的不是可行解（负数）
// 把它转成对偶问题，就把方程的形式由剩余变量变成了松弛变量，于是初始 α 矩阵、λ 检验数就直接对应了 a 矩阵和 c 向量，直接套单纯形模板
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<math.h>
using namespace std;
const int maxn = 1e3 + 10;
const int maxm = 1e4 + 10;
const double inf = 1e20;
struct Simplex
{
    // 标准型：min Σcx, s.t. ax=b, x>=0
    double a[maxm][maxn], b[maxm], c[maxn], z;
    int n, m;
    void Init(int n_, int m_)
    {
        n = n_, m = m_;
        memset(c, 0, sizeof(c));
        memset(a, 0, sizeof(a));
        z = 0;
    }
    void Pivot(int k, int l)
    {
        b[l] /= a[l][k];
        for(int j = 0; j < n; j ++)
            if(j != k) a[l][j] /= a[l][k];
        a[l][k] = 1 / a[l][k];
        for(int i = 0; i < m; i ++)
        {
            if(i != l && fabs(a[i][k]) > 0)
            {
                b[i] -= a[i][k] * b[l];
                for(int j = 0; j < n; j ++)
                    if(j != k) a[i][j] -= a[i][k] * a[l][j];
                a[i][k] = -a[i][k] * a[l][k];
            }
        }
        z += c[k] * b[l];
        for(int j = 0; j < n; j ++)
            if(j != k) c[j] -= c[k] * a[l][j];
        c[k] *= -a[l][k];
    }
    double Solve()
    {
        while(true)
        {
            int k = 0, l = 0;
            for(k = 0; k < n && c[k] >= 0; k ++);
            if(k == n) return z;
            double mn = inf;
            for(int i = 0; i < m; i ++)
            {
                if(a[i][k] > 0 && mn > b[i] / a[i][k])
                {
                    mn = b[i] / a[i][k];
                    l = i;
                }
            }
            if(mn == inf) return inf;
            Pivot(k, l);
        }
    }
};
int n, m;
Simplex spx;
int main()
{
    int s, t, c;
    while(scanf("%d%d", &n, &m) != EOF)
    {
        spx.Init(n, m);
        for(int i = 0; i < n; i ++)
            scanf("%lf", &spx.c[i]), spx.c[i] = -spx.c[i];
        for(int i = 0; i < m; i ++)
        {
            scanf("%d%d%d", &s, &t, &c);
            for(int j = s; j <= t; j ++)
                spx.a[i][j - 1] = 1;
            spx.b[i] = c;
        }
        printf("%d\n", (int)(-spx.Solve() + 0.5));
    }
    return 0;
}