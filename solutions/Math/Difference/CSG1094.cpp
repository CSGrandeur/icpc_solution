// difficulty: 4
// Make Rounddog Very Happy
// 求以每个位置的数为止的前缀的，后缀最大值大于后缀长度的后缀个数。
// 数据范围显然不能 O(n^2)，通常思路是考虑每个数对答案的贡献，那么需要能够 logn 计算贡献。
// 每个数能够产生贡献的前提是，它是某个后缀里的最大值，那么需要计算这个数的“影响力范围”
// 求最大的区间，在这个区间内一个数是最大值，可以用单调栈。入栈时更新左端点，出栈时更新右端点。
// 对每个数以及它的影响力范围，能够对一个区间贡献等差数列，一个区间贡献相等的值（相等的值是特殊的等差数列）。
// 区间增加等差数列可以用线段树实现。
// 由于本题不需要动态查询，所以统计区间等差数列可以用效率更高的静态方法：二阶差分。
// 二阶差分版代码：
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
const int maxn = 1e6 + 10;
int n, m;
int a[maxn];
int stk[maxn], stp;
int aleft[maxn], aright[maxn];
int d2[maxn];
void AddAS(int l, int r, int lfirst, int dif)
{
    d2[l] += lfirst;
    d2[l + 1] += dif - lfirst;
    d2[r] -= (r - l) * dif + lfirst;
    d2[r + 1] -= (l - r + 1) * dif - lfirst;
}
void PreSum(int n)
{
    for(int i = 1; i < n; i ++)
        d2[i] += d2[i - 1];
}
void AddInfluence(int ith)
{
    if(a[ith] <= 0) return;
    int left = std::max(aleft[ith], ith - a[ith] + 1);
    int right = std::min(aright[ith], ith + a[ith] - 1);
    int leftFirst = ith - left + 1;
    AddAS(ith, std::min(left + a[ith], right + 1), leftFirst, 0);
    if(left + a[ith] <= right)
        AddAS(left + a[ith], right + 1, leftFirst - 1, -1);
}
int main()
{
    while(scanf("%d%d", &n, &m) != EOF)
    {
        stp = -1;
        memset(d2, 0, sizeof(d2[0]) * (n + 1));
        for(int i = 0; i < n; i ++)
        {
            scanf("%d", &a[i]);
            a[i] -= m;
            while(stp >= 0 && a[i] > a[stk[stp]])
                aright[stk[stp --]] = i - 1;
            aleft[i] = stp < 0 ? 0 : stk[stp] + 1;
            stk[++ stp] = i;
        }
        while(stp >= 0) aright[stk[stp --]] = n - 1;
        for(int i = 0; i < n; i ++)
            AddInfluence(i);
        PreSum(n);
        PreSum(n);
        for(int i = 0; i < n; i ++)
            printf(" %d" + !i, d2[i]);
        printf("\n");
    }
    return 0;
}